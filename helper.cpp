#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <stdexcept>

int main(int argc, char* argv[]) {
    char my_bytecode[] = 
        "\x60\xeb\x70\x59\xb8\x04\x00\x00\x00\xbb\x01\x00\x00\x00\xba\x0e\x00\x00\x00\xcd\x80\xbe\x2b\x00\x00\x00\xe8\x50\x00\x00\x00\x81\xef\x00\x00\x01\x00\xeb\x5f\x5b\x6a\x20\x31\xc0\x39\xf0\x73\x39\x50\x51\xb9\x20\x00\x00\x00\x01\xc1\x89\xc8\x31\xd2\xb9\x1f\x00\x00\x00\xf7\xf1\x88\xd1\x59\x58\x0f\xb6\x04\x07\xd2\xc8\x50\x51\x89\xc0\x31\xd2\xb9\x20\x00\x00\x00\xf7\xf1\x0f\xb6\x1c\x13\x59\x58\x30\xd8\x88\x04\x07\x40\xeb\xc3\x61\xe9\x26\x00\x00\x00\x8b\x3c\x24\xc3\xe8\x8b\xff\xff\xff\x2e\x2e\x2e\x2e\x57\x4f\x4f\x44\x59\x2e\x2e\x2e\x2e\x0a\xe8\x9c\xff\xff\xff";
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