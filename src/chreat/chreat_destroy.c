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
        if(!new_elf->new_text)
            error_file_map("Memory allocation failed for new ELF text", elf->map, elf->size);
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

    memcpy(new_elf->new_text + (uint16_t)0x0f, &file_sz, sizeof(uint32_t));
    memcpy(new_elf->new_text + (uint16_t)0xad, &elf->key, KEY_SIZE);
    memcpy(new_elf->new_text + (uint16_t)0x16, &text, sizeof(int32_t));
    memcpy(new_elf->new_text + ((uint16_t)0x71), &rel_entry, sizeof(int32_t));
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
        if(!new_elf->new_text)
            error_file_map("Memory allocation failed for new ELF text", elf->map, elf->size);
    ft_memset(new_elf->new_text, 0, new_elf->new_text_size);
    ft_memcpy(new_elf->new_text, DESTROY_32, new_elf->new_text_size - 31);

    hdr = (Elf32_Ehdr *)elf->map;
    last_entry = hdr->e_entry;
    
    // rel_entry = last_entry - 
    //     ((Elf32_Addr)(new_elf->last_sect_32->p_vaddr + new_elf->last_sect_32->p_memsz) + (uint16_t)0x00 + sizeof(int32_t));
    // text = (Elf32_Addr)(new_elf->last_sect_32->p_vaddr + new_elf->last_sect_32->p_memsz) + (uint16_t)0x00 - 2 -
    //             ((Elf32_Phdr *)elf->text)->p_vaddr;
    // file_sz = ((Elf32_Phdr *)elf->text)->p_filesz;

    rel_entry = last_entry - 
        ((Elf32_Addr)(new_elf->last_sect_32->p_vaddr + new_elf->last_sect_32->p_memsz) + (uint16_t)0x5a + sizeof(int32_t));
    text = (Elf32_Addr)(new_elf->last_sect_32->p_vaddr + new_elf->last_sect_32->p_memsz) + (uint16_t)0x1c - 2 -
                ((Elf32_Phdr *)elf->text)->p_vaddr;
    file_sz = ((Elf32_Phdr *)elf->text)->p_filesz;


    // text_size = 22)--- 0x16; text = 38)--- 0x26; key = 131)--- 0x83 ; end = 99)--- 0x63;
    // text_size = 37)--- 0x25; text = 28)--- 0x1c; key = 122)--- 0x7a ; end = 90)--- 0x5a;

    memcpy(new_elf->new_text + (uint16_t)0x25, &file_sz, sizeof(uint32_t));
    memcpy(new_elf->new_text + (uint16_t)0x7a, &elf->key, KEY_SIZE);
    memcpy(new_elf->new_text + (uint16_t)0x1c, &text, sizeof(int32_t));
    memcpy(new_elf->new_text + (uint16_t)0x5a, &rel_entry, sizeof(int32_t));
}