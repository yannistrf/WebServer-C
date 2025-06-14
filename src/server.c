#include <stdlib.h>

#include "server.h"
#include "communication.h"

int server_online = 0;
int threads_running = 0;

Server* CreateWebServer(char* addr, uint16_t port)
{
    Server* server = malloc(sizeof(Server));
    strncpy(server->addr, addr, sizeof(server->addr));
    server->port = port;
    server->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server->fd < 0)
    {
        perror("socket");
        DestroyWebServer(server);
        return NULL;
    }

    setsockopt(server->fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));

    struct sockaddr_in server_addr = {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port); // Port number
    inet_aton(addr, (struct in_addr*) &server_addr.sin_addr.s_addr); // Bind to all interfaces

    if (bind(server->fd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind");
        DestroyWebServer(server);
        return NULL;
    }

    if (listen(server->fd, 5) < 0)
    {
        perror("listen");
        DestroyWebServer(server);
        return NULL;
    }

    server_online = 1;
    printf("[SERVER LISTENING %s:%hu]\n", server->addr, server->port);

    return server;
}

void RunWebServer(Server* server)
{
    while (server_online)
    {
        struct sockaddr_in client_addr = {};
        socklen_t client_addr_len = sizeof(client_addr);
        int client_sockfd = accept(server->fd, (struct sockaddr*) &client_addr, &client_addr_len);
        if (client_sockfd < 0)
        {
            perror("accept");
            continue;
        }

        ClientConn* client_conn = malloc(sizeof(ClientConn)); // must be freed inside thread!
        strncpy(client_conn->addr, inet_ntoa(client_addr.sin_addr), 16);
        client_conn->port = ntohs(client_addr.sin_port);
        client_conn->fd = client_sockfd;

        pthread_t tid;
        if (pthread_create(&tid, NULL, HandleCommunication, (void*) client_conn) != 0)
        {
            perror("pthread_create");
            free(client_conn);
            continue;
        }
        pthread_detach(tid);
    }    
}

void DestroyWebServer(Server* server)
{

    while (atomic_load(&threads_running) > 0)
        sleep(1);

    if (server)
    {
        if (server->fd)
            close(server->fd);
        free(server);
    }
}


