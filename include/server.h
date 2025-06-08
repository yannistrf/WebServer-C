#pragma once

#include "common.h"
#include "communication.h"

typedef struct
{
    int fd;
    char addr[16];
    uint16_t port;
} Server;


Server* CreateWebServer(char* addr, uint16_t port);
void RunWebServer(Server* server);
void DestroyWebServer(Server* server);

