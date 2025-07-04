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

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <binary_file>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];

    // Открываем файл
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    // Получаем размер файла через fstat
    struct stat st;
    if (fstat(fd, &st) < 0) {
        perror("fstat");
        close(fd);
        return 1;
    }

    // Мапим файл в память для удобства
    void *map = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (map == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }

    // ELF header
    Elf64_Ehdr *ehdr = (Elf64_Ehdr *) map;

    // Проверяем ELF magic
    if (memcmp(ehdr->e_ident, ELFMAG, SELFMAG) != 0) {
        fprintf(stderr, "Not an ELF file.\n");
        munmap(map, st.st_size);
        close(fd);
        return 1;
    }

    // Проверяем класс ELF
    if (ehdr->e_ident[EI_CLASS] != ELFCLASS64) {
        fprintf(stderr, "Not a 64-bit ELF file.\n");
        munmap(map, st.st_size);
        close(fd);
        return 1;
    }

    // Переходим к таблице заголовков секций
    Elf64_Shdr *shdr_table = (Elf64_Shdr *) ((char *)map + ehdr->e_shoff);
    int shnum = ehdr->e_shnum;

    // Индекс секции с именами секций
    Elf64_Shdr *sh_strtab = &shdr_table[ehdr->e_shstrndx];
    const char *const sh_strtab_p = (char *)map + sh_strtab->sh_offset;

    // Ищем секцию с именем ".text"
    for (int i = 0; i < shnum; i++) {
        const char *name = sh_strtab_p + shdr_table[i].sh_name;

        if (strcmp(name, ".text") == 0) {
            printf("Found .text section!\n");

            // Получаем offset и размер
            Elf64_Off offset = shdr_table[i].sh_offset;
            Elf64_Xword size = shdr_table[i].sh_size;

            printf(".text offset: 0x%lx, size: %lu bytes\n", offset, size);

            // Читаем содержимое .text
            const unsigned char *text_data = (unsigned char *)map + offset;

            printf("First 16 bytes of .text:\n");
            for (size_t j = 0; j < size; j++) {
                printf("%02x ", text_data[j]);
            }
            printf("\n");

            break;
        }
    }

    munmap(map, st.st_size);
    close(fd);
    return 0;
}