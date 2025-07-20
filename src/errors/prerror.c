#include "woody.h"

void error_file(const char *msg) 
{
    if (msg)
        perror(msg);
    else
        perror("Error");
    exit(EXIT_FAILURE);
}

void error_file_fd(const char *msg, int clos_fd) 
{
    if (clos_fd >= 0)
        close(clos_fd);
    if (msg)
        perror(msg);
    else
        perror("Error");
    exit(EXIT_FAILURE);
}

void error_file_map(const char *msg, void *map, size_t size) 
{
    if (map && size > 0)
        munmap(map, size);
    if (msg)
        perror(msg);
    else
        perror("Error");
    exit(EXIT_FAILURE);
}