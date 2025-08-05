#include "woody.h"

void chreat_destroy(Elf_t *elf, New_Elf_t *new_elf) {
    if (!elf || !new_elf) {
        error_file("Invalid ELF or new ELF structure");
    }

    Elf64_Ehdr	*hdr;
    uint32_t	last_entry;
	int32_t		rel_entry;
	int32_t		text;
	uint64_t	file_sz;
    new_elf->new_text_size = sizeof(DESTROY) - 1 + KEY_SIZE;
    new_elf->new_text = malloc(new_elf->new_text_size);
    ft_memset(new_elf->new_text, 0, new_elf->new_text_size);
    ft_memcpy(new_elf->new_text, DESTROY, new_elf->new_text_size - 31);

    hdr = elf->map;
    last_entry = hdr->e_entry;
    rel_entry = last_entry - (new_elf->last_sect->p_vaddr + new_elf->last_sect->p_memsz + (uint16_t)0x72 + sizeof(int32_t));
    text = ((Elf64_Phdr *)elf->text)->p_vaddr - 
        (new_elf->last_sect->p_vaddr + new_elf->last_sect->p_memsz + (uint16_t)0x18 + sizeof(int32_t));
    file_sz = ((Elf64_Phdr *)elf->text)->p_filesz;
    // text_size = 17 - 11 ; text = 24 - 18; key = 174 - ae; end = 114 - 72;

    
	// printf("filesz: %lu\n", new_elf->new_text_size);
    // for (size_t i = 0; i < new_elf->new_text_size; ++i) {
    //     printf("\\x%02x", ((uint8_t *)new_elf->new_text)[i]);
    //     // if ((i + 1) % 16 == 0)   
    //     //     printf("\n");
    // }
    // printf("\n");
    // printf("rel_entry:  0x%x\n", rel_entry);
    // printf("text:       0x%x\n", text);
    memcpy(new_elf->new_text + (uint16_t)0x11, &file_sz, sizeof(uint32_t));
    memcpy(new_elf->new_text + (uint16_t)0xae, &elf->key, KEY_SIZE);
    memcpy(new_elf->new_text + (uint16_t)0x18, &text, sizeof(int32_t));
    memcpy(new_elf->new_text + ((uint16_t)0x72), &rel_entry, sizeof(int32_t));

	// printf("filesz: %lu\n", new_elf->new_text_size);
    // for (size_t i = 0; i < new_elf->new_text_size; ++i) {
    //     printf("\\x%02x", ((uint8_t *)new_elf->new_text)[i]);
    //     // if ((i + 1) % 16 == 0)   
    //     //     printf("\n");
    // }
    // printf("\n");
}

void chreat_destroy_32(Elf_t *elf, New_Elf_t *new_elf) {
    if (!elf || !new_elf) {
        error_file("Invalid ELF or new ELF structure");
    }

    Elf32_Ehdr	*hdr;
    uint32_t	last_entry;
	int32_t		rel_entry;
	int32_t		text;
	uint32_t	file_sz;
    new_elf->new_text_size = sizeof(DESTROY_32) - 1 + KEY_SIZE;
    new_elf->new_text = malloc(new_elf->new_text_size);
    ft_memset(new_elf->new_text, 0, new_elf->new_text_size);
    ft_memcpy(new_elf->new_text, DESTROY_32, new_elf->new_text_size - 31);

    hdr = (Elf32_Ehdr *)elf->map;
    last_entry = hdr->e_entry;
    rel_entry = last_entry - 
        (new_elf->last_sect->p_vaddr + new_elf->last_sect->p_memsz + (uint16_t)0x60 + sizeof(int32_t));
    text = ((Elf32_Phdr *)elf->text)->p_vaddr - 
        (new_elf->last_sect->p_vaddr + new_elf->last_sect->p_memsz + (uint16_t)0x1c + sizeof(int32_t));
    file_sz = ((Elf32_Phdr *)elf->text)->p_filesz;
    // text_size = 9 - 9; text = 28 - 1c; key = 152 - 98 ; end = 96  - 60;
    memcpy(new_elf->new_text + (uint16_t)0x9, &file_sz, sizeof(uint32_t));
    memcpy(new_elf->new_text + (uint16_t)0x98, &elf->key, KEY_SIZE);
    memcpy(new_elf->new_text + (uint16_t)0x1c, &text, sizeof(int32_t));
    memcpy(new_elf->new_text + (uint16_t)0x60, &rel_entry, sizeof(int32_t));
}