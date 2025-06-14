#pragma once
#define _GNU_SOURCE // need to use gnu standard for signal handling

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdatomic.h>
#include <signal.h>
#include <stdarg.h>

extern int server_online;
extern int threads_running;

void set_sig_handler();

typedef enum {
    LOG_DBG,
    LOG_SUCCESS,
    LOG_WARN,
    LOG_ERR,
    LOG_LEVEL_LEN
} LOG_LEVEL;

void log_msg(LOG_LEVEL level, const char* fmt, ...);