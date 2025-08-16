#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <stdexcept>

int main(int argc, char* argv[]) {
    char my_bytecode[] = 
        "\x60\xeb\x5f\x59\xb8\x04\x00\x00\x00\xbb\x01\x00\x00\x00\xba\x0e\x00\x00\x00\xcd\x80\xe8\x44\x00\x00\x00\x81\xea\x00\x00\x01\x00\x89\xd5\x31\xdb\xb9\x2b\x00\x00\x00\xeb\x4a\x5f\x39\xcb\x7d\x28\x89\xd8\x83\xc0\x20\x31\xd2\xb9\x1f\x00\x00\x00\xf7\xf1\x88\xd1\x80\xe1\x07\x8a\x44\x1d\x00\xd2\xc8\x88\xd9\x80\xe1\x1f\x32\x04\x0f\x88\x44\x1d\x00\x43\xeb\xd4\x61\xe9\x26\x00\x00\x00\x8b\x14\x24\xc3\xe8\x9c\xff\xff\xff\x2e\x2e\x2e\x2e\x57\x4f\x4f\x44\x59\x2e\x2e\x2e\x2e\x0a\xe8\xb1\xff\xff\xff";
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
