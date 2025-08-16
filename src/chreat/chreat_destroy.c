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
    // ft_memcpy(new_elf->new_text, DESTROY, new_elf->new_text_size);
    ft_memcpy(new_elf->new_text, DESTROY, sizeof(DESTROY));

    hdr = elf->map;
    last_entry = hdr->e_entry;
    rel_entry = last_entry - ((Elf64_Addr)(new_elf->last_sect->p_vaddr + new_elf->last_sect->p_memsz) + (uint16_t)0x71 + sizeof(int32_t));
    text = ((Elf64_Phdr *)elf->text)->p_vaddr - 
        (new_elf->last_sect->p_vaddr + new_elf->last_sect->p_memsz + (uint16_t)0x16 + sizeof(int32_t));
    file_sz = ((Elf64_Phdr *)elf->text)->p_filesz;
    // text_size = 17 - 11 ; text = 24 - 18; key = 174 - ae; end = 114 - 72;
    // text_size = 15 - 0f ; text = 22 - 16; key = 173 - ad; end = 113 - 71;

    
	// printf("filesz: %lu\n", new_elf->new_text_size);
    // for (size_t i = 0; i < new_elf->new_text_size; ++i) {
    //     printf("\\x%02x", ((uint8_t *)new_elf->new_text)[i]);
    //     // if ((i + 1) % 16 == 0)   
    //     //     printf("\n");
    // }
    // printf("\n");
    // printf("rel_entry:  0x%x\n", rel_entry);
    // printf("text:       0x%x\n", text);

    memcpy(new_elf->new_text + (uint16_t)0x0f, &file_sz, sizeof(uint32_t));
    memcpy(new_elf->new_text + (uint16_t)0xad, &elf->key, KEY_SIZE);
    memcpy(new_elf->new_text + (uint16_t)0x16, &text, sizeof(int32_t));
    memcpy(new_elf->new_text + ((uint16_t)0x71), &rel_entry, sizeof(int32_t));

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

    write(1, "text01\n", 8);
    hdr = (Elf32_Ehdr *)elf->map;
    last_entry = hdr->e_entry;
    
    // rel_entry = last_entry - 
    //     ((Elf32_Addr)(new_elf->last_sect_32->p_vaddr + new_elf->last_sect_32->p_memsz) + (uint16_t)0x60 + sizeof(int32_t));
    // write(1, "text02\n", 8);;
    // text = (Elf32_Addr)(new_elf->last_sect_32->p_vaddr + new_elf->last_sect_32->p_memsz) + (uint16_t)0x1c - 2 -
    //             ((Elf32_Phdr *)elf->text)->p_vaddr;
    // file_sz = ((Elf32_Phdr *)elf->text)->p_filesz;

    rel_entry = last_entry - 
        ((Elf32_Addr)(new_elf->last_sect_32->p_vaddr + new_elf->last_sect_32->p_memsz) + (uint16_t)0x6b + sizeof(int32_t));
    write(1, "text02\n", 8);;
    text = (Elf32_Addr)(new_elf->last_sect_32->p_vaddr + new_elf->last_sect_32->p_memsz) + (uint16_t)0x21 - 2 -
                ((Elf32_Phdr *)elf->text)->p_vaddr;
    file_sz = ((Elf32_Phdr *)elf->text)->p_filesz;

    // text_size = 25)--- 0x19; text = 35)--- 0x23; key = 467)--- 0x1d3 ; end = 283)--- 0x11b;
    // text_size = 22)--- 0x16; text = 33)--- 0x21; key = 139)--- 0x8b ; end = 107)--- 0x6b;

    memcpy(new_elf->new_text + (uint16_t)0x16, &file_sz, sizeof(uint32_t));
    memcpy(new_elf->new_text + (uint16_t)0x8b, &elf->key, KEY_SIZE);
    memcpy(new_elf->new_text + (uint16_t)0x21, &text, sizeof(int32_t));
    memcpy(new_elf->new_text + (uint16_t)0x6b, &rel_entry, sizeof(int32_t));
}