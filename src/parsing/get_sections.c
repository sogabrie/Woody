#include "woody.h"

int	is_text_section(void *phdr, uint8_t arch) {
	if (arch == ELFCLASS64) {
		Elf64_Phdr *p = phdr;
		return (p->p_type == PT_LOAD && (p->p_flags & PF_X) && (p->p_flags & PF_R));
	} else {
		Elf32_Phdr *p = phdr;
		return (p->p_type == PT_LOAD && (p->p_flags & PF_X) && (p->p_flags & PF_R));
	}
}

int	is_data_section(void *phdr, uint8_t arch) {
	if (arch == ELFCLASS64) {
		Elf64_Phdr *p = phdr;
		return (p->p_type == PT_LOAD && p->p_filesz != p->p_memsz);
	} else {
		Elf32_Phdr *p = phdr;
		return (p->p_type == PT_LOAD && p->p_filesz != p->p_memsz);
	}
}

void	*get_section(Elf_t *elf, int (*f)(void *, uint8_t))
{
	uint16_t	i = 0;
	uint16_t	size;
	uint64_t	phoff;

	if (elf->arch == ELFCLASS64) {
		Elf64_Phdr *ehdr = (Elf64_Phdr *)elf->map;
		size = ((Elf64_Ehdr *)elf->map)->e_phnum;
		// printf("size: %d\n", size);
		phoff = ((Elf64_Ehdr *)elf->map)->e_phoff;
		ehdr = elf->map + phoff;
		while (i < size && (void *)(ehdr + i + 1) < elf->map_end) {
			if (f((void *)(ehdr + i), ELFCLASS64))
			{
				// elf->text = ehdr + i;
				// uint32_t text_size = ((Elf64_Phdr *)elf->text)->p_filesz;
				// printf("text_size: %d\n", text_size);
				// for (size_t j = 0; j < text_size; j++)
				// {
					
				// 	printf("%02x ", ((uint8_t *)ehdr + i)[j]);
				// 	if ((j + 1) % 16 == 0)
				// 		printf("\n");
				// }
				// printf("\n");
				return ((void *)(ehdr + i));
			}
			i++;
            // write(1, "aaaa15\n", 8);
		}
	} else if (elf->arch == ELFCLASS32) {
		Elf32_Phdr *ehdr = (Elf32_Phdr *)elf->map;
		size = ((Elf32_Ehdr *)elf->map)->e_phnum;
		phoff = ((Elf32_Ehdr *)elf->map)->e_phoff;
		ehdr = elf->map + phoff;
		while (i < size && (void *)(ehdr + i + 1) < elf->map_end) {
			if (f((void *)(ehdr + i), ELFCLASS32))
				return ((void *)(ehdr + i));
			i++;
		}
	}
	return (NULL);
}

Elf64_Phdr	*get_last_seqcion(Elf_t *elf)
{
	Elf64_Phdr	*ph;
	uint16_t	size;

	size = ((Elf64_Ehdr *)elf->map)->e_phnum;
	ph = elf->map + ((Elf64_Ehdr *)elf->map)->e_phoff;
	while (--size && (void *)(ph + size) < elf->map_end)
	{
		if (ph[size].p_type == PT_LOAD)
			return (ph + size);
	}
	return (NULL);
}

Elf32_Phdr	*get_last_seqcion_32(Elf_t *elf)
{
	Elf32_Phdr	*ph;
	uint16_t	size;

	size = ((Elf32_Ehdr *)elf->map)->e_phnum;
	ph = elf->map + ((Elf32_Ehdr *)elf->map)->e_phoff;
	while (--size && (void *)(ph + size) < elf->map_end)
	{
		if (ph[size].p_type == PT_LOAD)
			return (ph + size);
	}
	return (NULL);
}