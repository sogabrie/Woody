#include "woody.h"

void chreat_elf_64(Elf_t *elf, New_Elf_t *new_elf) {
    if(!(new_elf->last_sect = get_last_seqcion(elf)))
        error_file("Failed to get last section for 64-bit ELF");
    if(!(new_elf->data = get_section(elf, is_data_section)))
        new_elf->data = new_elf->last_sect;
    
    chreat_destroy(elf);

}

void chreat_elf_32(Elf_t *elf, New_Elf_t *new_elf) {
    if(!(new_elf->last_sect_32 = get_last_seqcion_32(elf)))
        error_file("Failed to get last section for 32-bit ELF");
    if(!(new_elf->data_32 = get_section(elf, is_data_section)))
        new_elf->data_32 = new_elf->last_sect_32;
    
    chreat_destroy_32(elf);
}

void chreat_elf(Elf_t *elf) {
    New_Elf_t new_elf;
    if (elf->arch == ELFCLASS64) {
        chreat_elf_64(elf, &new_elf);
    } else if (elf->arch == ELFCLASS32) {
        chreat_elf_32(elf, &new_elf);
    } else {
        error_file("Unsupported ELF architecture");
    }
}