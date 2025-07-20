#define _GNU_SOURCE
#include <elf.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <errno.h>

#define STUB_PATH "woody_stub.bin"
#define STUB_ENTRY_OFFSET 32
#define PAGE_ALIGN 0x1000

void fatal(const char *msg) {
    perror(msg);
    exit(1);
}

off_t get_file_size(int fd) {
    off_t size = lseek(fd, 0, SEEK_END);
    if (size == (off_t)-1) fatal("lseek");
    if (lseek(fd, 0, SEEK_SET) == (off_t)-1) fatal("lseek reset");
    return size;
}

void *read_file(const char *path, size_t *size_out) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) fatal("open");

    off_t size = get_file_size(fd) + 8;
    void *buf = malloc(size);
    if (!buf) fatal("malloc");

    if (read(fd, buf, size - 8) != size - 8) fatal("read");
    close(fd);
    *size_out = size;
    return buf;
}

void write_file(const char *path, void *data, size_t size) {
    int fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0755);
    if (fd < 0) fatal("open out");

    if (write(fd, data, size) != (ssize_t)size) fatal("write");
    close(fd);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        dprintf(2, "Usage: %s <input_elf>\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) fatal("open");

    off_t file_size = get_file_size(fd);
    void *map = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (map == MAP_FAILED) fatal("mmap");
    close(fd);

    Elf64_Ehdr *ehdr = (Elf64_Ehdr *)map;
    if (memcmp(ehdr->e_ident, ELFMAG, SELFMAG) != 0 || ehdr->e_ident[EI_CLASS] != ELFCLASS64) {
        dprintf(2, "Invalid ELF64 file\n");
        return 1;
    }

    Elf64_Phdr *phdr = (Elf64_Phdr *)((char *)map + ehdr->e_phoff);
    Elf64_Addr orig_entry = ehdr->e_entry;

    size_t stub_size;
    uint8_t *stub = read_file(STUB_PATH, &stub_size);

    if (stub_size < STUB_ENTRY_OFFSET + sizeof(orig_entry)) {
        dprintf(2, "Stub too small\n");
        free(stub);
        return 1;
    }

    printf("Original e_entry: 0x%lx\n", orig_entry);

    memcpy(stub + STUB_ENTRY_OFFSET, &orig_entry, sizeof(orig_entry));

    for (int i = 0; i < 8; i++)
        printf("%02x ", stub[STUB_ENTRY_OFFSET + i]);
    printf(" <-- patched orig_entry\n");

    Elf64_Off max_offset = 0;
    Elf64_Addr max_vaddr = 0;
    int free_phdr_index = -1;

    for (int i = 0; i < ehdr->e_phnum; i++) {
        if (phdr[i].p_type == PT_LOAD) {
            Elf64_Off end = phdr[i].p_offset + phdr[i].p_filesz;
            if (end > max_offset) {
                max_offset = end;
                max_vaddr = phdr[i].p_vaddr + phdr[i].p_memsz;
            }
        }
        if (phdr[i].p_type == PT_NULL && free_phdr_index == -1)
            free_phdr_index = i;
    }

    Elf64_Off stub_offset = (max_offset + PAGE_ALIGN - 1) & ~(PAGE_ALIGN - 1);
    Elf64_Addr stub_vaddr = (max_vaddr + PAGE_ALIGN - 1) & ~(PAGE_ALIGN - 1);
    size_t new_file_size = stub_offset + stub_size;

    void *new_map = malloc(new_file_size);
    if (!new_map) fatal("malloc new_map");
    memcpy(new_map, map, file_size);
    memcpy((char *)new_map + stub_offset, stub, stub_size);

    Elf64_Ehdr *new_ehdr = (Elf64_Ehdr *)new_map;
    Elf64_Phdr *new_phdr = (Elf64_Phdr *)((char *)new_map + new_ehdr->e_phoff);

    Elf64_Phdr new_seg = {
        .p_type = PT_LOAD,
        .p_offset = stub_offset,
        .p_vaddr = stub_vaddr,
        .p_paddr = stub_vaddr,
        .p_filesz = stub_size,
        .p_memsz = stub_size,
        .p_flags = PF_R | PF_X,
        .p_align = PAGE_ALIGN
    };

    if (free_phdr_index != -1) {
        new_phdr[free_phdr_index] = new_seg;
        new_ehdr->e_entry = stub_vaddr;
        write_file("woody", new_map, new_file_size);
        munmap(map, file_size);
        free(new_map);
        free(stub);
        return 0;
    }

    size_t old_phnum = new_ehdr->e_phnum;
    size_t new_phnum = old_phnum + 1;
    size_t phdr_size = new_phnum * sizeof(Elf64_Phdr);
    size_t new_phoff = (new_file_size + PAGE_ALIGN - 1) & ~(PAGE_ALIGN - 1);
    size_t final_size = new_phoff + phdr_size;

    void *final_map = malloc(final_size);
    if (!final_map) fatal("malloc final_map");
    memset(final_map, 0, final_size);
    memcpy(final_map, new_map, new_file_size);
    memcpy((char *)final_map + new_phoff, new_phdr, old_phnum * sizeof(Elf64_Phdr));
    memcpy((char *)final_map + new_phoff + old_phnum * sizeof(Elf64_Phdr), &new_seg, sizeof(Elf64_Phdr));

    Elf64_Ehdr *f_ehdr = (Elf64_Ehdr *)final_map;
    f_ehdr->e_phoff = new_phoff;
    f_ehdr->e_phnum = new_phnum;
    f_ehdr->e_entry = stub_vaddr;

    write_file("woody", final_map, final_size);
    munmap(map, file_size);
    free(new_map);
    free(final_map);
    free(stub);

    return 0;
}
