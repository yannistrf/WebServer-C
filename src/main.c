#include "common.h"
#include "server.h"


int main()
{
    Server* server = CreateWebServer("0.0.0.0", 2207);
    if (!server)
    {
        printf("Could not create server\n");
        return 1;
    }

    RunWebServer(server);
    DestroyWebServer(server);
}