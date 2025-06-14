#ifndef SCAN_H
#define SCAN_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "ll.h"

typedef struct
{
    char* host;
    int start_port;
    int end_port;
} scan_args;

// open ports list
static lnode* head = NULL;
static lnode* tail = NULL;
static pthread_mutex_t list_lock = PTHREAD_MUTEX_INITIALIZER;

int tcp_scan(char* host_str, int port_num);
lnode* vanilla_scan(char* host_str);
lnode* threaded_scan(char* host_str, int n_threads);

#endif
