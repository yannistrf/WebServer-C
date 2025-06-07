#include "request.h"

void* HandleRequest(void* data)
{
    ClientConn* client_conn = (ClientConn*) data;
    printf("Client connected: %s:%hu\n",
            client_conn->addr, client_conn->port);


    recv(client_conn->fd, client_conn->req.buf, sizeof(client_conn->req.buf), 0);
    // printf("********************************\n");
    // printf("%s\n", client_conn->req.buf);
    // printf("--------------------------------\n");

    ParseRequest(&client_conn->req);

    char* res = "HTTP/1.1 200 OK\r\n" \
                "Content-Type: text/plain\r\n" \
                "Content-Length: 13\r\n" \
                "\r\n" \
                "Hello, world!";

    send(client_conn->fd, res, strlen(res), 0);

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