#include "communication.h"
#include "routes.h"

void* HandleCommunication(void* data)
{
    ClientConn* client_conn = (ClientConn*) data;
    printf("Client connected: %s:%hu\n",
            client_conn->addr, client_conn->port);


    recv(client_conn->fd, client_conn->req.buf, sizeof(client_conn->req.buf), 0);
    // printf("********************************\n");
    // printf("%s\n", client_conn->req.buf);
    // printf("--------------------------------\n");

    ParseRequest(&client_conn->req);

    int free_res = 1;
    char* res = ConstructResponse(&client_conn->req);

    if (!res)
    {
        res = "HTTP/1.1 404 File not found\r\n" \
                "Content-Type: text/html\r\n" \
                "\r\n" \
                "<h1> 404 Page not found </h1>";
        free_res = 0;
    }
    send(client_conn->fd, res, strlen(res), 0);

    if (free_res)
        free(res);
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

char* ConstructResponse(Request* req)
{
    char* headers = "HTTP/1.1 200 OK\r\n" \
                "Content-Type: text/html\r\n" \
                "\r\n";

    const char* file_name = NULL;

    file_name = GetRouteTemplate(req->uri);
    if (!file_name)
        return NULL;

    printf("File = %s\n", file_name);
    FILE* fp = fopen(file_name, "r");
    if (!fp)
    {
        perror("fopen");
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    printf("HEADER SIZE = %ld\n", strlen(headers));
    printf("FILE SIZE = %ld\n", file_size);
    rewind(fp);

    char* res = malloc(strlen(headers) + file_size + 1);
    strcpy(res, headers);
    fread(res + strlen(headers), sizeof(char), file_size, fp);
    res[strlen(headers) + file_size] = '\0';
    printf("PACKET SIZE = %ld\n", strlen(res));
    fclose(fp);

    printf("%s\n", res);
    return res;
}