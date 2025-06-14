#include "common.h"

void terminate_handler(int signum)
{
    log_msg(LOG_WARN, "Terminating server (SIGNUM: %d)", signum);
    fflush(stdout);
    server_online = 0;
}

void set_sig_handler()
{
    struct sigaction sigterm_act = {};
    sigterm_act.sa_handler = terminate_handler;
    sigfillset(&sigterm_act.sa_mask);   // block all signals during handler execution
    sigaction(SIGINT, &sigterm_act, NULL);
}