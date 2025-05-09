#include "scan.h"

int tcp_scan(char* host_str, int port_num)
{
    int status, client_fd;
    struct sockaddr_in serv_addr;

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        // printf("Cannot create socket\n");
        close(client_fd);
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_num);

    if (inet_pton(AF_INET, host_str, &serv_addr.sin_addr) <= 0)
    {
        // printf("Invalid host address: %s\n", host_str);
        close(client_fd);
        return -1;
    }

    if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0)
    {
        // printf("Cannot connect to %s:%d\n", host_str, port_num);
        close(client_fd);
        return -1;
    }

    close(client_fd);
    return 0;
}

lnode* vanilla_scan(char* host_str)
{
    lnode* head = NULL;
    lnode* tail = NULL;
    
    // scan every port on specified host (port 0 is reserved)
    for (int port = 1; port <= 65535; port++)
    {
        int scan_status = tcp_scan(host_str, port);
    
        if (scan_status == 0)
        {
            lnode* valid_port = malloc(sizeof(lnode));
            valid_port->num = port;
            valid_port->next = NULL;

            if (head == NULL && tail == NULL)
            {
                head = valid_port;
                tail = valid_port;
                continue;
            }
            
            tail->next = valid_port;
            tail = valid_port;
        }
    }

    return head;
}
