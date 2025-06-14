#include "common.h"
#include "server.h"
#include "routes.h"

int main()
{
    set_sig_handler();

    ParseRoutesFile();
    Server* server = CreateWebServer("0.0.0.0", 2207);
    if (!server)
    {
        log_msg(LOG_ERR, "Could not create server");
        return 1;
    }

    RunWebServer(server);
    DestroyWebServer(server);
    DestroyMapRoutes();
}