#include "woody.h"

void check_ELF(Elf_t *elf)
{
    unsigned char *bytes = (unsigned char *)elf->map;
    Elf64_Ehdr *ehdr64 = (Elf64_Ehdr *)bytes;

    if (ft_memcmp(bytes, ELFMAG, SELFMAG) != 0)
        error_file_map("Not a valid ELF file", elf->map, elf->size);
    if (elf->arch != ELFCLASS64 && elf->arch != ELFCLASS32)
        error_file_map("Unsupported ELF class", elf->map, elf->size);
    // if (bytes[EI_DATA] != ELFDATA2LSB && bytes[EI_DATA] != ELFDATA2MSB)
    //     error_file_map("Unsupported ELF data encoding", elf->map, elf->size);
    if(bytes[EI_DATA] != ELFDATA2LSB)
        error_file_map("Unsupported ELF data encoding (only little-endian supported)", elf->map, elf->size);
    if(bytes[EI_DATA] == ELFDATANONE)
        error_file_map("Unsupported endianness", elf->map, elf->size);
    if (bytes[EI_VERSION] == EV_NONE)
        error_file_map("Unsupported ELF version", elf->map, elf->size);
    if(ehdr64->e_type != ET_EXEC && ehdr64->e_type != ET_DYN)
        error_file_map("Unsupported ELF type", elf->map, elf->size);
}

void check_key(char *key) {
    if (strlen(key) != KEY_SIZE)
        error_file("Key must be 32 characters long.\n");
}

void main_pars(Elf_t *elf, char *filename, char *key) {
    int fd;

    if (elf == NULL || filename == NULL)
        error_file("Invalid parameters passed to main_pars.\n");
    if(key)
        check_key(key);
    fd = open(filename, O_RDWR);
    if (fd < 0)
        error_file(filename);
    elf->size = lseek(fd, 0, SEEK_END);
    if (elf->size < 0)
        error_file_fd("lseek", fd);
    if (lseek(fd, 0, SEEK_SET) < 0)
        error_file_fd("lseek", fd);
    elf->map = mmap(NULL, elf->size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (elf->map == MAP_FAILED)
        error_file_fd("Error mapping file", fd);
    // close(fd);
    if (elf->size < sizeof(Elf64_Ehdr))
        error_file_map("File too small to be a valid ELF file", elf->map, elf->size);
    elf->arch = ((Elf64_Ehdr *)elf->map)->e_ident[EI_CLASS];
    elf->map_end = elf->map + elf->size;
    check_ELF(elf);
}

    // if(bytes[EI_CLASS] == ELFCLASS64) {
    //     elf.ehdr64 = (Elf64_Ehdr *)elf.map;
    //     elf.phdr64 = (Elf64_Phdr *)(elf.map + elf.ehdr64->e_phoff);
    //     elf.ehdr32 = NULL;
    //     elf.phdr32 = NULL;
    // } else if (bytes[EI_CLASS] == ELFCLASS32) {
    //     elf.ehdr32 = (Elf32_Ehdr *)elf.map;
    //     elf.phdr32 = (Elf32_Phdr *)(elf.map + elf.ehdr32->e_phoff);
    //     elf.ehdr64 = NULL;
    //     elf.phdr64 = NULL;
    // } else {
    //     munmap(elf.map, elf.size);
    //     error_file("Unsupported ELF class");
    // }
    // munmap(elf.map, elf.filesize);

// nasm -f elf64 woody_stub.asm -o woody_stub.o
// objcopy -O binary woody_stub.o woody_stub.bin
// hexdump -v -e '1/1 "%02X "' decrypy.bin > binfile


// nasm -f elf64 woody_stub.asm -o woody_stub.o
// ld -o woody_stub.elf -nostdlib -static woody_stub.o
// objcopy -O binary woody_stub.elf woody_stub.bin