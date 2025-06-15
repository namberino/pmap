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

int tcp_scan_timeout(char* host_str, int port_num, int timeout_ms)
{
    int status, client_fd;
    struct sockaddr_in serv_addr;
    struct timeval tv;
    fd_set wfd_set;

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        // printf("Cannot create socket\n");
        close(client_fd);
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_num);
    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;

    // current client fd status flags
    int flags = fcntl(client_fd, F_GETFL, 0);

    // switch to non-blocking mode (add non-block bits to flags)
    if (fcntl(client_fd, F_SETFL, flags | O_NONBLOCK))
    {
        printf("Cannot switch to non-blocking mode\n");
        close(client_fd);
        return -1;
    }

    if (inet_pton(AF_INET, host_str, &serv_addr.sin_addr) <= 0)
    {
        // printf("Invalid host address: %s\n", host_str);
        close(client_fd);
        return -1;
    }

    if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0 && errno != EINPROGRESS)
    {
        // printf("Cannot connect to %s:%d\n", host_str, port_num);
        close(client_fd);
        return -1;
    }

    // wait for socket to become writable or timeout
    FD_ZERO(&wfd_set);
    FD_SET(client_fd, &wfd_set);

    int sel_stat = select(client_fd + 1, NULL, &wfd_set, NULL, &tv);
    if (sel_stat == 0) // timeout
    {
        // printf("Port %d connect timeout\n", port_num);
        close(client_fd);
        return -1;
    }
    else if (sel_stat < 0) // select error
    {
        // printf("Cannot select socket file descriptor\n");
        close(client_fd);
        return -1;
    }

    int so_error;
    socklen_t so_len = sizeof(so_error);

    // check for connection error
    if (getsockopt(client_fd, SOL_SOCKET, SO_ERROR, &so_error, &so_len) < 0)
    {
        close(client_fd);
        return -1;
    }

    // connection failed
    if (so_error != 0)
    {
        close(client_fd);
        return -1;
    }

    fcntl(client_fd, F_SETFL, flags);
    close(client_fd);
    return 0;
}

pnode* vanilla_scan(char* host_str)
{
    pnode* head = NULL;
    pnode* tail = NULL;
    
    // scan every port on specified host (port 0 is reserved)
    for (int port = 1; port <= 65535; port++)
    {
        int scan_status = tcp_scan(host_str, port);
    
        if (scan_status == 0)
        {
            pnode* valid_port = malloc(sizeof(pnode));
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

void threaded_append_port(int port)
{
    pnode* valid_port = malloc(sizeof(pnode));
    valid_port->num = port;
    valid_port->next = NULL;

    pthread_mutex_lock(&list_lock);
    if (!head)
    {
        head = valid_port;
        tail = valid_port;
    }
    else
    {
        tail->next = valid_port;
        tail = valid_port;
    }
    pthread_mutex_unlock(&list_lock);
}

void* scan_thread(void* args)
{
    scan_args* a = args;

    for (int i = a->start_port; i <= a->end_port; i++)
    {
        if (tcp_scan_timeout(a->host, i, a->timeout_ms) == 0)
            threaded_append_port(i);
    }
    
    return NULL;
}

pnode* threaded_scan(char* host_str, int n_threads, int timeout_ms)
{
    pthread_t threads[n_threads];
    scan_args args[n_threads];

    // start threads with args
    int ports_per_thread = 65535 / n_threads;
    for (int i = 0; i < n_threads; i++)
    {
        args[i].host = host_str;
        args[i].start_port = i * ports_per_thread + 1;
        args[i].end_port = (i == n_threads - 1) ? 65535 : (i + 1) * ports_per_thread;
        args[i].timeout_ms = timeout_ms;

        pthread_create(&threads[i], NULL, scan_thread, &args[i]);
    }

    // wait for all threads to finish
    for (int i = 0; i < n_threads; i++)
        pthread_join(threads[i], NULL);

    return head;
}
