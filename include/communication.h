#pragma once

#include "common.h"

typedef struct
{
    char buf[1024];
    char* type;
    char* uri;
    char* protocol;
} Request;

typedef struct
{
    int fd;
    char addr[16];
    uint16_t port;
    Request req;
} ClientConn;

void* HandleCommunication(void* data);
void ParseRequest(Request* req);
char* ConstructResponse(Request* req);
