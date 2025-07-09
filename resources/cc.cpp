// #include <unistd.h>
// #include <fcntl.h>
// #include <stdio.h>


// int main()
// {
//     int fd = open("porc", O_RDONLY);

//     char buf[100000];
//     ssize_t bytes_read = read(fd, buf, sizeof(buf) - 1);
//     if (bytes_read == -1) {
//         perror("read");
//         close(fd);
//         return 1;
//     }
//     buf[bytes_read] = '\0';

//     puts(buf);

//     close(fd);
//     return 0;
// }

// #include <stdio.h>
// #include <fcntl.h>
// #include <unistd.h>
// #include <string.h>
// #include <elf.h>

// int main(int argc, char **argv) {
//     if (argc != 2) {
//         fprintf(stderr, "Usage: %s <binary_file>\n", argv[0]);
//         return 1;
//     }

//     const char *filename = argv[1];

//     int fd = open(filename, O_RDONLY);
//     if (fd < 0) {
//         perror("open");
//         return 1;
//     }

//     Elf64_Ehdr header;
//     ssize_t bytes = read(fd, &header, sizeof(header));
//     if (bytes != sizeof(header)) {
//         perror("read");
//         close(fd);
//         return 1;
//     }

//     if (memcmp(header.e_ident, ELFMAG, SELFMAG) != 0) {
//         fprintf(stderr, "Not an ELF file.\n");
//         close(fd);
//         return 1;
//     }

//     if (header.e_ident[EI_CLASS] == ELFCLASS32) {
//         printf("This is a 32-bit ELF file.\n");
//     } else if (header.e_ident[EI_CLASS] == ELFCLASS64) {
//         printf("This is a 64-bit ELF file.\n");
//     } else {
//         printf("Unknown ELF class.\n");
//     }

//     close(fd);
//     return 0;
// }


#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <elf.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdint.h>

void insert_loader(void *map, size_t filesize, Elf64_Ehdr *ehdr) {
    Elf64_Phdr *phdr = (Elf64_Phdr *)((char*)map + ehdr->e_phoff);
    Elf64_Phdr *rx_load = NULL;
    for (int i = 0; i < ehdr->e_phnum; i++)
        if (phdr[i].p_type == PT_LOAD && (phdr[i].p_flags & PF_X))
            rx_load = &phdr[i];
    if (!rx_load) { fprintf(stderr, "No RX PT_LOAD\n"); return; }

    size_t page_size = 0x1000;
    FILE *f = fopen("resources/loader.bin", "rb");
    if (!f) { perror("fopen loader.bin"); return; }
    fseek(f, 0, SEEK_END);
    size_t loader_size = ftell(f);
    fseek(f, 0, SEEK_SET);
    unsigned char *loader = (unsigned char *)malloc(loader_size);
    fread(loader, 1, loader_size, f);
    fclose(f);

    size_t old_end_offset = rx_load->p_offset + rx_load->p_filesz;
    Elf64_Addr old_end_vaddr = rx_load->p_vaddr + rx_load->p_filesz;
    size_t loader_offset = (old_end_offset + page_size - 1) & ~(page_size - 1);
    Elf64_Addr loader_vaddr = (old_end_vaddr + page_size - 1) & ~(page_size - 1);
    size_t padding = loader_offset - old_end_offset;
    size_t need_filesz = (loader_offset + loader_size) - rx_load->p_offset;
    if (need_filesz > rx_load->p_filesz) rx_load->p_filesz = need_filesz;
    if (need_filesz > rx_load->p_memsz) rx_load->p_memsz = need_filesz;

    int fd = open("woody", O_RDWR | O_CREAT | O_TRUNC, 0755);
    if (fd < 0) { perror("open woody"); free(loader); return; }
    if (ftruncate(fd, filesize + padding + loader_size) != 0) {
        perror("ftruncate"); close(fd); free(loader); return;
    }
    void *new_map = mmap(NULL, filesize + padding + loader_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (new_map == MAP_FAILED) {
        perror("mmap (resize)"); close(fd); free(loader); return;
    }
    memcpy(new_map, map, filesize);
    if (padding > 0)
        memset((char*)new_map + old_end_offset, 0, padding);
    memcpy((char*)new_map + loader_offset, loader, loader_size);

    ((Elf64_Ehdr *)new_map)->e_entry = loader_vaddr;
    msync(new_map, filesize + padding + loader_size, MS_SYNC);
    munmap(new_map, filesize + padding + loader_size);
    close(fd);
    free(loader);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <binary_file>\n", argv[0]);
        return 1;
    }
    const char *filename = argv[1];
    int fd = open(filename, O_RDWR);
    if (fd < 0) { perror("open"); return 1; }
    struct stat st;
    if (fstat(fd, &st) != 0) { perror("fstat"); close(fd); return 1; }
    void *map = mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) { perror("mmap"); close(fd); return 1; }
    Elf64_Ehdr *ehdr = (Elf64_Ehdr *)map;

    insert_loader(map, st.st_size, ehdr);

    munmap(map, st.st_size);
    close(fd);
    return 0;
}