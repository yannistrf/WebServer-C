#pragma once

#include "common.h"

#define STATUS_OK 200
#define STATUS_BAD_REQUEST 400
#define STATUS_NOT_FOUND 404
#define STATUS_METHOD_NOT_ALLOWED 405
#define STATUS_INTERNAL_ERROR 500

#define STATUS_OK_DESC "OK";
#define STATUS_BAD_REQUEST_DESC "Bad Request";
#define STATUS_NOT_FOUND_DESC "Not Found";
#define STATUS_METHOD_NOT_ALLOWED_DESC "Method Not Allowed";
#define STATUS_INTERNAL_ERROR_DESC "Internal Server Error";

#define PKT_TEMPLATE "HTTP/1.1 %d %s\r\n" \
                    "Content-Type: text/html\r\n" \
                    "\r\n"

typedef struct
{
    char buf[1024];
    char* type;
    char* uri;
    char* protocol;
} Request;

typedef struct
{
    int status_code;
    char* desc;
    char* pkt;
    const char* resource_location;
} Response;

typedef struct
{
    int fd;
    char addr[16];
    uint16_t port;
    Request req;
} ClientConn;

void* HandleCommunication(void* data);
void ParseRequest(Request* req);
void BuildResponseHeader(Request* req, Response* res);
void ConstructResponsePkt(Response* res);
