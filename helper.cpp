#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <stdexcept>

int main(int argc, char* argv[]) {
    char my_bytecode[] = 
        "\x60\xeb\x4c\x59\xb8\x04\x00\x00\x00\xbb\x01\x00\x00\x00\xba\x0e\x00\x00\x00\xcd\x80\xb9\x2b\x00\x00\x00\xbe\x04\x00\x00\x00\xe8\x27\x00\x00\x00\x81\xea\x00\x00\x01\x00\x31\xdb\x31\xc0\xeb\x32\x5f\x8a\x04\x1f\x00\xe0\x30\x02\x42\x43\x39\xf3\x75\x05\x80\xc4\x2a\x31\xdb\xe2\xec\x61\xe9\x26\x00\x00\x00\x8b\x14\x24\xc3\xe8\xaf\xff\xff\xff\x2e\x2e\x2e\x2e\x57\x4f\x4f\x44\x59\x2e\x2e\x2e\x2e\x0a\xe8\xc9\xff\xff\xff";
    size_t bytecode_size = sizeof(my_bytecode) / sizeof(my_bytecode[0]);
    
    if (argc != 2) {
        std::cerr << "err arg" << std::endl;
        return 1;
    }

    std::string input_arg = argv[1];
    unsigned int search_byte;

    try {
        search_byte = std::stoul(input_arg, nullptr, 16);
    } catch (const std::exception& e) {
        std::cerr << "err" << std::endl;
        return 1;
    }

    bool found = false;

    for (size_t i = 0; i < bytecode_size; ++i) {
        if (static_cast<unsigned char>(my_bytecode[i]) == search_byte) {
            std::cout << "  " << std::dec << i << ")" << "--- 0x" << std::hex << i<< std::endl;
            found = true;
        }
    }

    if (!found) {
        std::cout << "Байт 0x" << std::uppercase << std::hex << search_byte << " не найден." << std::endl;
    }

    return 0;
}