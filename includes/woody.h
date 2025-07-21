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

# define KEY_SIZE 32

typedef struct s_elf_file
{
    void        *map;
    void        *map_end;
    void        *text;
    uint32_t	text_size;
    off_t       size;
    char        key[KEY_SIZE];
    uint16_t    arch; 
    Elf64_Ehdr *ehdr64;
    Elf64_Phdr *phdr64;
    Elf32_Ehdr *ehdr32;
    Elf32_Phdr *phdr32;
} Elf_t;

//parsing
    //pareser.c
    void check_ELF(Elf_t *elf);
    void check_key(Elf_t *elf, char *key);
    void main_pars(Elf_t *elf, char *filename, char *key);

//errors
    //prerror.c
    void error_file(const char *msg);
    void error_file_fd(const char *msg, int clos_fd);
    void error_file_map(const char *msg, void *map, size_t size);
    //encryption.c
    uint64_t get_phdr_offset(Elf_t *elf);
    uint32_t get_phdr_filesz(Elf_t *elf);
    void set_phdr_flags(Elf_t *elf, uint32_t flags);
    void encryption_text(Elf_t *elf);
    //encrypy.s
    // void encrypt_xor_rol_64(uint8_t *data, size_t len, uint8_t *key, size_t keylen);
    // void decrypt_xor_rol_64(uint8_t *data, size_t len, uint8_t *key, size_t keylen);
    void _decrypt_xor_rol_32(uint8_t *data, size_t len, uint8_t *key, size_t keylen);
    //get_sections.c
    int is_text_section(void *phdr, uint8_t arch);
    int is_data_section(void *phdr, uint8_t arch);
    void *get_section(Elf_t *elf, int (*f)(void *, uint8_t));

#endif