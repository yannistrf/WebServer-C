#include "communication.h"
#include "routes.h"

void* HandleCommunication(void* data)
{
    ClientConn* client_conn = (ClientConn*) data;
    printf("\n--------------------------------\n");
    printf("Client connected: %s:%hu\n",
            client_conn->addr, client_conn->port);


    recv(client_conn->fd, client_conn->req.buf, sizeof(client_conn->req.buf), 0);

    ParseRequest(&client_conn->req);

    Response res;
    BuildResponseHeader(&client_conn->req, &res);
    ConstructResponsePkt(&res);

    send(client_conn->fd, res.pkt, strlen(res.pkt), 0);
    free(res.pkt);

    close(client_conn->fd);
    free(client_conn);
    return NULL;
}

void ParseRequest(Request* req)
{
    req->type = strtok(req->buf, " ");
    printf("Request type: %s\n", req->type);
    req->uri = strtok(NULL, " ");
    printf("URI: %s\n", req->uri);
    req->protocol = strtok(NULL, "\r\n");
    printf("Protocol: %s\n", req->protocol);
}

void BuildResponseHeader(Request* req, Response* res)
{
    if (!strncmp(req->type, "POST", 4) ||
        !strncmp(req->type, "DELETE", 6) ||
        !strncmp(req->type, "PUT", 3))
    {
        res->status_code = STATUS_METHOD_NOT_ALLOWED;
        res->desc = STATUS_METHOD_NOT_ALLOWED_DESC;
        return;
    }

    if (strncmp(req->type, "GET", 3))
    {
        res->status_code = STATUS_BAD_REQUEST;
        res->desc = STATUS_BAD_REQUEST_DESC;
        return;
    }

    if (strncmp(req->protocol, "HTTP/1.1", 8))
    {
        res->status_code = STATUS_BAD_REQUEST;
        res->desc = STATUS_BAD_REQUEST_DESC;
        return;
    }

    res->resource_location = GetRouteTemplate(req->uri);
    if (!res->resource_location) // then this route was not defined
    { 
        res->status_code = STATUS_NOT_FOUND;
        res->desc = STATUS_NOT_FOUND_DESC;
        return;
    }

    if (access(res->resource_location, R_OK))
    {
        res->status_code = STATUS_INTERNAL_ERROR;
        res->desc = STATUS_INTERNAL_ERROR_DESC;
        return;
    }

    res->status_code = STATUS_OK;
    res->desc = STATUS_OK_DESC;
}


void ConstructResponsePkt(Response* res)    // must free res.pkt !!!
{
    if (res->status_code != STATUS_OK)
    {
        //                                 <status_code>                  %d  %s  \0        
        size_t pkt_size = strlen(PKT_TEMPLATE) + 3 + strlen(res->desc)*2 - 2 - 2 + 1;
        res->pkt = malloc(pkt_size);
        sprintf(res->pkt, PKT_TEMPLATE, res->status_code, res->desc);
        strcpy(res->pkt + strlen(res->pkt), res->desc);
        return;
    }

    printf("File = %s\n", res->resource_location);
    FILE* fp = fopen(res->resource_location, "r");  // file existance is checked in BuildResponseHeader
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    printf("FILE SIZE = %ld\n", file_size);
    rewind(fp);
    
    //                                  <status_code>                           %d  %s  \0
    size_t pkt_size = strlen(PKT_TEMPLATE) + 3 + strlen(res->desc) + file_size - 2 - 2 + 1;
    res->pkt = malloc(pkt_size);
    sprintf(res->pkt, PKT_TEMPLATE, res->status_code, res->desc);
    printf("HEADER SIZE = %ld\n", strlen(res->pkt));

    fread(res->pkt + strlen(res->pkt), sizeof(char), file_size, fp);
    res->pkt[pkt_size - 1] = '\0';
    printf("PACKET SIZE = %ld\n", strlen(res->pkt));
    fclose(fp);
}