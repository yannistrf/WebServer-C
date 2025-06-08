#include "common.h"
#include "response.h"
#include "routes.h"

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