#include "woody.h"

int save_file(char *woody, size_t size)
{
	int		fd;

    fd = open("woody", O_WRONLY | O_CREAT | O_TRUNC, 0775);
    if (fd < 0)
    {
        return 0;
    }
	write(fd, woody, size);
	close(fd);
    return 1;
}

size_t chreat_new_elf(Elf_t *elf, New_Elf_t *new_elf) {
    size_t		index;
    size_t		file_index;

    index = new_elf->data->p_offset + new_elf->data->p_filesz;
    file_index = index;
    ft_memcpy(new_elf->str, elf->map, index);
    if(new_elf->data && new_elf->data->p_filesz != new_elf->data->p_memsz) {
        ft_bzero(new_elf->str + index, new_elf->data->p_memsz - new_elf->data->p_filesz);
        index += new_elf->data->p_memsz - new_elf->data->p_filesz;
        new_elf->data = new_elf->str + ((void *)new_elf->data - elf->map);
        new_elf->data->p_filesz = new_elf->data->p_memsz;
    }
    if(new_elf->last_sect != new_elf->data) {
        index += new_elf->last_sect->p_offset + new_elf->last_sect->p_filesz - file_index;
        ft_memcpy(new_elf->str + index, elf->map + file_index, new_elf->last_sect->p_offset +
            new_elf->last_sect->p_filesz - file_index);
    }
    new_elf->last_sect = new_elf->str + ((void *)new_elf->last_sect - elf->map);
    return index;
}

size_t chreat_new_elf_32(Elf_t *elf, New_Elf_t *new_elf) {
    size_t		index;
    size_t		file_index;

    index = new_elf->data_32->p_offset + new_elf->data_32->p_filesz;
    file_index = index;
    ft_memcpy(new_elf->str, elf->map, index);
    if(new_elf->data_32 && new_elf->data_32->p_filesz != new_elf->data_32->p_memsz) {
        ft_bzero(new_elf->str + index, new_elf->data_32->p_memsz - new_elf->data_32->p_filesz);
        index += new_elf->data_32->p_memsz - new_elf->data_32->p_filesz;
        new_elf->data_32 = new_elf->str + ((void *)new_elf->data_32 - elf->map);
        new_elf->data_32->p_filesz = new_elf->data_32->p_memsz;
    }
    if(new_elf->last_sect_32 != new_elf->data_32) {
        index += new_elf->last_sect_32->p_offset + new_elf->last_sect_32->p_filesz - file_index;
        ft_memcpy(new_elf->str + index, elf->map + file_index, new_elf->last_sect_32->p_offset +
            new_elf->last_sect_32->p_filesz - file_index);
    }
    new_elf->last_sect_32 = new_elf->str + ((void *)new_elf->last_sect_32 - elf->map);
    return index;
}

void chreat_elf_64(Elf_t *elf, New_Elf_t *new_elf) {
    size_t		index;

    if(!(new_elf->last_sect = get_last_seqcion(elf)))
        error_file_map("Failed to get last section for 64-bit ELF", elf->map, elf->size);
    new_elf->data = get_section(elf, is_data_section);
    if(!new_elf->data)
        new_elf->data = new_elf->last_sect;
    
    chreat_destroy(elf, new_elf);

    new_elf->str_size = new_elf->last_sect->p_offset + new_elf->last_sect->p_filesz +
        new_elf->new_text_size + (new_elf->data->p_memsz - new_elf->data->p_filesz);
    new_elf->str = malloc(new_elf->str_size);
    if(!new_elf->str)
        error_file_map("Memory allocation failed for new ELF string", elf->map, elf->size);
    index = chreat_new_elf(elf, new_elf);

    ft_memcpy(new_elf->str + index, new_elf->new_text, new_elf->new_text_size);
    ((Elf64_Ehdr *)new_elf->str)->e_entry = new_elf->last_sect->p_vaddr + new_elf->last_sect->p_memsz;
    new_elf->last_sect->p_filesz += new_elf->new_text_size;
	new_elf->last_sect->p_memsz += new_elf->new_text_size;
	new_elf->last_sect->p_flags |= PF_X | PF_R;

	((Elf64_Ehdr *)new_elf->str)->e_shoff = 0;
	((Elf64_Ehdr *)new_elf->str)->e_shnum = 0;
	((Elf64_Ehdr *)new_elf->str)->e_shstrndx = SHN_UNDEF;
	((Elf64_Ehdr *)new_elf->str)->e_shentsize = 0;
    if(!save_file(new_elf->str, new_elf->str_size))
        error_file_map("Failed to save new ELF file", new_elf->str, new_elf->str_size);
    munmap(new_elf->str, new_elf->str_size);
}

void chreat_elf_32(Elf_t *elf, New_Elf_t *new_elf) {
    size_t		index;

    if(!(new_elf->last_sect_32 = get_last_seqcion_32(elf)))
        error_file_map("Failed to get last section for 32-bit ELF", elf->map, elf->size);
    if(!(new_elf->data_32 = get_section(elf, is_data_section)))
        new_elf->data_32 = new_elf->last_sect_32;
    chreat_destroy_32(elf, new_elf);

    new_elf->str_size = new_elf->last_sect_32->p_offset + new_elf->last_sect_32->p_filesz +
        new_elf->new_text_size + (new_elf->data_32->p_memsz - new_elf->data_32->p_filesz);
    new_elf->str = malloc(new_elf->str_size);
    if(!new_elf->str)
        error_file_map("Memory allocation failed for new ELF string", elf->map, elf->size);
    
    index = chreat_new_elf_32(elf, new_elf);

    ft_memcpy(new_elf->str + index, new_elf->new_text, new_elf->new_text_size);
    ((Elf32_Ehdr *)new_elf->str)->e_entry = new_elf->last_sect_32->p_vaddr + new_elf->last_sect_32->p_memsz;
    new_elf->last_sect_32->p_filesz += new_elf->new_text_size;
	new_elf->last_sect_32->p_memsz += new_elf->new_text_size;
	new_elf->last_sect_32->p_flags |= PF_X | PF_R;

	((Elf32_Ehdr *)new_elf->str)->e_shoff = 0;
	((Elf32_Ehdr *)new_elf->str)->e_shnum = 0;
	((Elf32_Ehdr *)new_elf->str)->e_shstrndx = SHN_UNDEF;
	((Elf32_Ehdr *)new_elf->str)->e_shentsize = 0;
    if(!save_file(new_elf->str, new_elf->str_size))
        error_file_map("Failed to save new ELF file", new_elf->str, new_elf->str_size);
    munmap(new_elf->str, new_elf->str_size);
}

void chreat_elf(Elf_t *elf) {
    New_Elf_t new_elf;
    if (elf->arch == ELFCLASS64) {
        chreat_elf_64(elf, &new_elf);
    } else if (elf->arch == ELFCLASS32) {
        // error_file_map("32-bit ELF not supported yet", elf->map, elf->size);
        chreat_elf_32(elf, &new_elf);
    } else {
        error_file("Unsupported ELF architecture");
    }
}