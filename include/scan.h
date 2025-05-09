#ifndef SCAN_H
#define SCAN_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>

int tcp_scan(char* host_str, int port_num);

#endif
