#include "woody.h"

void error_file(const char *msg) 
{
    if (msg)
        perror(msg);
    else
        perror("Error");
    exit(EXIT_FAILURE);
}