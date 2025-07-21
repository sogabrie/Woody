#include "woody.h"

uint64_t get_phdr_offset(Elf_t *elf) {
	if (elf->arch == ELFCLASS32)
		return ((Elf32_Phdr *)elf->text)->p_offset;
	else
		return ((Elf64_Phdr *)elf->text)->p_offset;
}

uint32_t get_phdr_filesz(Elf_t *elf) {
	if (elf->arch == ELFCLASS32)
		return ((Elf32_Phdr *)elf->text)->p_filesz;
	else
		return ((Elf64_Phdr *)elf->text)->p_filesz;
}

void set_phdr_flags(Elf_t *elf, uint32_t flags) {
	if (elf->arch == ELFCLASS32)
		((Elf32_Phdr *)elf->text)->p_flags |= flags;
	else
		((Elf64_Phdr *)elf->text)->p_flags |= flags;
}

static inline uint8_t rol8(uint8_t value, uint8_t shift) {
    shift %= 8;
    return (value << shift) | (value >> (8 - shift));
}

static inline uint8_t ror8(uint8_t value, uint8_t shift) {
    shift %= 8;
    return (value >> shift) | (value << (8 - shift));
}

void encrypt_xor_rol(uint8_t *data, size_t len, const uint8_t *key, size_t keylen) {
    for (size_t i = 0; i < len; i++) {
        uint8_t k = key[i % keylen];
        uint8_t x = data[i] ^ k;
        uint8_t r = (i + keylen) % 31;
        data[i] = rol8(x, r);
    }
}

void decrypt_xor_rol(uint8_t *data, size_t len, const uint8_t *key, size_t keylen) {
    for (size_t i = 0; i < len; i++) {
        uint8_t r = (i + keylen) % 31;
        r %= 8;
        uint8_t x = ror8(data[i], r);
        uint8_t k = key[i % keylen];
        data[i] = x ^ k;
    }
}

// cipher[i] = ROL(plain[i] ^ key[i % keylen], (i + keylen) % 31);

void encryption_text(Elf_t *elf)
{
	void		*text;
	void		*key;
	uint32_t	key_size;

    // write(1, "text01\n", 8);
	elf->text = get_section(elf, is_text_section);
    // write(1, "text02\n", 8);
	set_phdr_flags(elf, PF_W);
	text = elf->map + get_phdr_offset(elf);
    // write(1, "text03\n", 8);
	// offset = get_phdr_offset(phdr, elf->arch);
    // write(1, "text04\n", 8);
	// elf->text_size = get_phdr_filesz(phdr, elf->arch);
    // write(1, "text05\n", 8);
	elf->text_size = get_phdr_filesz(elf);
	key = &(elf->key);
	key_size = KEY_SIZE;
    // write(1, "text06\n", 8);
	if (text < elf->map || text > elf->map_end || text + elf->text_size > elf->map_end)
        error_file_map("Text section out of bounds", elf->map, elf->size);
    // write(1, "text08\n", 8);
    for (size_t i = 0; i < elf->text_size; i++)
	{
		printf("%02x ", ((uint8_t *)text)[i]);
		if ((i + 1) % 16 == 0)
			printf("\n");
	}
	printf("\n");
    
	// encrypt_xor_rol_64(text, elf->text_size, key, key_size);
	encrypt_xor_rol(text, elf->text_size, key, key_size);
    for (size_t i = 0; i < elf->text_size; i++)
	{
		printf("%02x ", ((uint8_t *)text)[i]);
		if ((i + 1) % 16 == 0)
			printf("\n");
	}
	printf("\n");
	// decrypt_xor_rol(text, elf->text_size, key, key_size);
	_decrypt_xor_rol_32(text, elf->text_size, key, key_size);
    for (size_t i = 0; i < elf->text_size; i++)
	{
		printf("%02x ", ((uint8_t *)text)[i]);
		if ((i + 1) % 16 == 0)
			printf("\n");
	}
	printf("\n");

}