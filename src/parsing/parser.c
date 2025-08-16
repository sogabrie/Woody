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

int hex_char_to_int(char c) {
    if ('0' <= c && c <= '9') return c - '0';
    else if ('a' <= c && c <= 'f') return 10 + (c - 'a');
    else if ('A' <= c && c <= 'F') return 10 + (c - 'A');
    else return -1;
}

void check_key(Elf_t *elf, char *key) {
    if (strlen(key) != KEY_SIZE * 2)
        error_file_map("Input string must be exactly 64 hex characters.", elf->map, elf->size);
    for (int i = 0; i < KEY_SIZE; i++) {
        int high = hex_char_to_int(key[2 * i]);
        int low  = hex_char_to_int(key[2 * i + 1]);

        if (high == -1 || low == -1) {
            error_file_map("Invalid hex character", elf->map, elf->size);
        }
        elf->key[i] = (char)((high << 4) | low);
    }
}

void	get_key(Elf_t *elf)
{
	int		fd;

	if ((fd = open("/dev/urandom", O_RDONLY)) == -1)
        error_file_map("Error urandom", elf->map, elf->size);
	read(fd, elf->key, KEY_SIZE);
	close(fd);
}

void print_key(Elf_t *elf, char *key)
{
    if(key)
        check_key(elf, key);
    else
        get_key(elf);
    
    int i = -1;
	printf("encryption key : 0x");
	while (++i < KEY_SIZE)
		printf("%02hhx", elf->key[i]);
	printf("\n");
    fflush(NULL);
}

void main_pars(Elf_t *elf, char *filename, char *key) {
    int fd;

    if (elf == NULL || filename == NULL)
        error_file("Invalid parameters passed to main_pars.\n");
    // fd = open(filename, O_RDWR);
    fd = open(filename, O_RDONLY);
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
    close(fd);
    // if (elf->size < sizeof(Elf64_Ehdr))
    //     error_file_map("File too small to be a valid ELF file", elf->map, elf->size);
    elf->arch = ((Elf64_Ehdr *)elf->map)->e_ident[EI_CLASS];
    elf->map_end = elf->map + elf->size;
    check_ELF(elf);
    print_key(elf, key);
}

// nasm -f elf64 woody_stub.asm -o woody_stub.o
// objcopy -O binary woody_stub.o woody_stub.bin
// hexdump -v -e '1/1 "%02X "' decrypy.bin > binfile


// nasm -f elf64 woody_stub.asm -o woody_stub.o
// ld -o woody_stub.elf -nostdlib -static woody_stub.o
// objcopy -O binary woody_stub.elf woody_stub.bin