#ifndef UTILS
#define UTILS

#include <string>
#include <cstdio>
#include <fstream>

namespace utils{
    inline const std::string CLEAR="\033[2J\033[1;1H";
    bool file_exists(const std::string& nums);
    std::string input_word();
    std::vector<uint8_t> generate_salt(std::size_t length);
    std::string bytes_to_hex(const std::vector<uint8_t>& data);
    std::vector<uint8_t> hex_to_bytes(std::string& hex);
    void storeJsonValueInFile(std::string value, const std::string index, const std::string path);
    
}
#endif