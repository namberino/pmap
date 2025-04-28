#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>

int main(int argc, char* argv[])
{
    char* host_str = NULL;
    char* port_str = NULL;
    int opt;
    opterr = 0;

    int option_index = 0;
    static struct option long_options[] = {
        {"host",    required_argument, NULL,  'i'},
        {"port",    required_argument, NULL,  'p'},
        {NULL,      0,                 NULL,   0 }
    };

    while ((opt = getopt_long(argc, argv, "-:i:p:", long_options, &option_index)) != -1)
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

    printf ("host = %s, port = %s\n", host_str, port_str);

    return 0;
}
