#include "woody.h"

int main(int argc, char **argv) {
    Elf_t elf;
    char *key = NULL;
    if (argc == 2) {
        fprintf(stdout, "Usage: %s <binary_file>\n", argv[1]);
    }else if (argc == 3){
        fprintf(stdout, "Processing file: %s  %s\n", argv[1], argv[2]);
        key = argv[2];
    }else {
        fprintf(stderr, "Invalid number of arguments.\n");
    }
    main_pars(elf, argv[1], key);
    return 0;
}