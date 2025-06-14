#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "scan.h"

int main(int argc, char* argv[])
{
    char* host_str = NULL;
    char* port_str = NULL;
    char* nthread_str = NULL;
    int opt;
    opterr = 0;

    int option_index = 0;
    static struct option long_options[] = {
        {"host", required_argument, NULL, 'i'},
        {"port", required_argument, NULL, 'p'},
        {"nthreads", required_argument, NULL, 't'},
        {NULL, 0, NULL,  0 }
    };

    while ((opt = getopt_long(argc, argv, "-:i:p:nt:", long_options, &option_index)) != -1)
    {
        switch (opt)
        {
            case 0:
                // printf("Long option %s", long_options[option_index].name);
                if (optarg) host_str = optarg;
                break;

            case 1:
                // printf("Long option %s", long_options[option_index].name);
                if (optarg) port_str = optarg;
                break;

            case 'i':
                host_str = optarg;
                break;

            case 'p':
                port_str = optarg;
                break;

            case 't':
                nthread_str = optarg;
                break;

            case ':':
                fprintf(stderr, "Missing option for %c\n", optopt);
                break;

            case '?':
                if (isprint(optopt))
                    fprintf (stderr, "Unknown option %c\n", optopt);
                else
                    fprintf(stderr, "Unknown option character \\x%x\n", optopt);
                return 1;

            default:
                abort();
        }
    }

    if (port_str != NULL)
    {
        char* endptr;
        int port_num = strtol(port_str, &endptr, 10);

        printf("Scanning %s:%d\n", host_str, port_num);
        if (tcp_scan(host_str, port_num) != 0)
            printf("Port is closed\n");
        else
            printf("Port is open\n");
    }
    else
    {
        // lnode* head = vanilla_scan(host_str);

        // lnode* curr_node = head;
        // while (curr_node != NULL)
        // {
        //     printf("Open port: %d\n", curr_node->num);
        //     curr_node = curr_node->next;
        // }

        // free_ll(head);

        pthread_mutex_init(&list_lock, NULL);

        char* endptr;
        int n_threads = strtol(nthread_str, &endptr, 10);
        lnode* head = threaded_scan(host_str, n_threads);

        lnode* curr_node = head;
        while (curr_node != NULL)
        {
            printf("Open port: %d\n", curr_node->num);
            curr_node = curr_node->next;
        }

        free_ll(head);
    }

    return 0;
}
