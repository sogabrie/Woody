#ifndef __WOODY_H__
#define __WOODY_H__

#include <stdio.h>
#include <elf.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct s_elf_file
{
    char *filename;
    int fd;
    size_t filesize;
    void *map;
    Elf64_Ehdr *ehdr64;
    Elf64_Phdr *phdr64;
    Elf32_Ehdr *ehdr32;
    Elf32_Phdr *phdr32;
} Elf_t;

//parsing

//errors


#endif