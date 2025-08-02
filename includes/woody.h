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
// text_size = 17 ; text = 24 ; key = 174; end = 114;
# define DESTROY "\x55\x48\x89\xe5\x53\x41\x54\x41\x55\x41\x56\x41\x57\xeb\x67\x41\xbf\x2b\x00\x00\x00\x4c\x8d\x35\x0e\x00\x00\x00\x41\xbd\x20\x00\x00\x00\x4d\x31\xc0\xe9\x7f\x00\x00\x00\x41\x5c\x4d\x39\xf8\x73\x36\x4c\x89\xc0\x4c\x01\xe8\x41\xb9\x1f\x00\x00\x00\x48\x31\xd2\x49\xf7\xf1\x88\xd1\x4f\x0f\xb6\x14\x06\x41\xd2\xca\x4c\x89\xc0\x48\x31\xd2\x49\xf7\xf5\x4d\x0f\xb6\x0c\x14\x45\x30\xca\x47\x88\x14\x06\x49\xff\xc0\xeb\xc5\x41\x5f\x41\x5e\x41\x5d\x41\x5c\x5b\x5d\xe9\x26\x00\x00\x00\x50\x57\x56\x52\xb8\x01\x00\x00\x00\xbf\x01\x00\x00\x00\x48\x8d\x35\x10\x00\x00\x00\xba\x0e\x00\x00\x00\x0f\x05\x5a\x5e\x5f\x58\xe9\x74\xff\xff\xff\x2e\x2e\x2e\x2e\x57\x4f\x4f\x44\x59\x2e\x2e\x2e\x2e\x0a\xe8\x7c\xff\xff\xff"
// text_size = 9; text = 28 ; key = 152; end = 96 ;
# define DESTROY_32 "\x55\x89\xe5\x53\x56\x57\xeb\x5c\xb9\x2b\x00\x00\x00\xbe\x20\x00\x00\x00\xe8\x00\x00\x00\x00\x8b\x14\x24\x81\xea\x00\x00\x01\x00\xc3\x31\xdb\xeb\x6e\x5b\x39\xcb\x73\x2f\x50\x52\x89\xd8\x01\xf0\x31\xd2\xbf\x1f\x00\x00\x00\xf7\xf7\x88\xd1\x5a\x58\x0f\xb6\x04\x1a\xd2\xc8\x50\x52\x89\xd8\x31\xd2\xf7\xf1\x0f\xb6\x3c\x13\x5a\x58\x30\xd0\x88\x04\x1a\x43\xeb\xcd\x5f\x5e\x5b\x89\xec\x5d\xe9\x26\x00\x00\x00\x50\x53\x51\x52\xe8\x00\x00\x00\x00\x59\xb8\x04\x00\x00\x00\xbb\x01\x00\x00\x00\xba\x0e\x00\x00\x00\xcd\x80\x5a\x59\x5b\x58\xeb\x83\x2e\x2e\x2e\x2e\x57\x4f\x4f\x44\x59\x2e\x2e\x2e\x2e\x0a\xe8\x8d\xff\xff\xff"

typedef struct s_elf_file
{
    void        *map;
    void        *map_end;
    void        *text;
    uint32_t	text_size;
    off_t       size;
    char        key[KEY_SIZE];
    uint16_t    arch; 
    // Elf64_Ehdr *ehdr64;
    // Elf64_Phdr *phdr64;
    // Elf32_Ehdr *ehdr32;
    // Elf32_Phdr *phdr32;
} Elf_t;

typedef struct s_elf_file_new
{
    void		*str;
    size_t      str_size;
    char		*new_text;
    size_t      new_text_size;
    size_t      destroy_size;
    Elf64_Phdr	*data;
	Elf64_Phdr	*last_sect;
	Elf32_Phdr	*data_32;
	Elf32_Phdr	*last_sect_32;
} New_Elf_t;

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
    void encrypt_xor_rol_64(uint8_t *data, size_t len, uint8_t *key, size_t keylen);
    // void decrypt_xor_rol_64(uint8_t *data, size_t len, uint8_t *key, size_t keylen);
    // void _decrypt_xor_rol_32(uint8_t *data, size_t len, uint8_t *key, size_t keylen);
    //get_sections.c
    int is_text_section(void *phdr, uint8_t arch);
    int is_data_section(void *phdr, uint8_t arch);
    void *get_section(Elf_t *elf, int (*f)(void *, uint8_t));
    Elf64_Phdr	*get_last_seqcion(Elf_t *elf);
    Elf32_Phdr	*get_last_seqcion_32(Elf_t *elf);


// chreat
    //chreat_elf.c
    void chreat_elf(Elf_t *elf);
    // chreat_destroy.c
    void chreat_destroy(Elf_t *elf);
    void chreat_destroy_32(Elf_t *elf);


    // void print_hex(const uint8_t *data, size_t len);
    // void print_hex_fd(int fd, const uint8_t *data, size_t len);
    // void print_hex_map(void *map, size_t size);

#endif