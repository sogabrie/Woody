#include "woody.h"

void main_pars(Elf_t elf, char *filename, char *key) {
    if(cheack_key(key)) {
        fprintf(stderr, "Invalid key provided.\n");
        exit(EXIT_FAILURE);
    }
    elf.fd = open(filename, O_RDWR);
    if (elf.fd < 0)
        error_file(filename);

    elf.filesize = lseek(elf.fd, 0, SEEK_END);
    if (elf.filesize < 0) {
        close(elf.fd);
        error_file("lseek");
    }
    if (lseek(elf.fd, 0, SEEK_SET) < 0) {
        close(elf.fd);
        error_file("lseek");
    }
    elf.map = mmap(NULL, elf.filesize, PROT_READ | PROT_WRITE, MAP_SHARED, elf.fd, 0);
    if (elf.map == MAP_FAILED) {
        close(elf.fd);
        error_file("Error mapping file");
    }

    close(elf.fd);
    unsigned char *bytes = (unsigned char *)elf.map;
    
    if (ft_memcmp(bytes, ELFMAG, SELFMAG) != 0) {
        munmap(elf.map, elf.filesize);
        error_file("Not a valid ELF file");
    }

    if(bytes[EI_CLASS] == ELFCLASS64) {
        elf.ehdr64 = (Elf64_Ehdr *)elf.map;
        elf.phdr64 = (Elf64_Phdr *)(elf.map + elf.ehdr64->e_phoff);
        elf.ehdr32 = NULL;
        elf.phdr32 = NULL;
    } else if (bytes[EI_CLASS] == ELFCLASS32) {
        elf.ehdr32 = (Elf32_Ehdr *)elf.map;
        elf.phdr32 = (Elf32_Phdr *)(elf.map + elf.ehdr32->e_phoff);
        elf.ehdr64 = NULL;
        elf.phdr64 = NULL;
    } else {
        munmap(elf.map, elf.filesize);
        error_file("Unsupported ELF class");
    }
    
    munmap(elf.map, elf.filesize);
}