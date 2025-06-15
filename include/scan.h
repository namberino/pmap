#ifndef SCAN_H
#define SCAN_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include "ll.h"

typedef struct
{
    char* host;
    int start_port;
    int end_port;
    int timeout_ms;
} scan_args;

// open ports list
static pnode* head = NULL;
static pnode* tail = NULL;
static pthread_mutex_t list_lock = PTHREAD_MUTEX_INITIALIZER;

int tcp_scan(char* host_str, int port_num);
int tcp_scan_timeout(char* host_str, int port_num, int timeout_ms);
pnode* vanilla_scan(char* host_str);
pnode* threaded_scan(char* host_str, int n_threads, int timeout_ms);

#endif
