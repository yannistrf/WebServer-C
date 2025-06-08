#include "common.h"
#include "server.h"
#include "routes.h"

int main()
{
    ParseRoutesFile();
    Server* server = CreateWebServer("0.0.0.0", 2207);
    if (!server)
    {
        printf("Could not create server\n");
        return 1;
    }

    RunWebServer(server);
    DestroyWebServer(server);
    DestroyMapRoutes();
}