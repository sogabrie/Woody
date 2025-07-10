#ifndef __WOODY_H__
#define __WOODY_H__

#include "libft.h"
#include <stddef.h>
#include <stdio.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <elf.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdint.h>

typedef struct s_elf_file
{
    char *filename;
    int fd;
    off_t filesize;
    void *map;
    Elf64_Ehdr *ehdr64;
    Elf64_Phdr *phdr64;
    Elf32_Ehdr *ehdr32;
    Elf32_Phdr *phdr32;
} Elf_t;

//parsing
    //pareser.c
    void main_pars(Elf_t elf, char *filename, char *key);

//errors
    //prerror.c
    void error_file(const char *msg);


#endif