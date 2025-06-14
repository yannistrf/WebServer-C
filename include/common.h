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

extern int server_online;
extern int threads_running;

void set_sig_handler();
