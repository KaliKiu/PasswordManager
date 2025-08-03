#ifndef LOGIN
#define LOGIN

#include <iostream>
#include <fstream>
#include <cstdio>
#include <argon2.h>
#include "../include/json.hpp"
class KeyDerivation;
namespace Login{
    
    struct Argon2Config{
        uint32_t t_cost;
        uint32_t m_cost;
        uint32_t parallelism;
        size_t hash_len;
        size_t salt_len;
    };
    KeyDerivation log();
    std::vector<uint8_t> hash_password(std::string& password,std::vector<uint8_t> salt, Argon2Config config_arg2);
    void storeJsonValueInFile(std::string value, const std::string index, const std::string path);
    Argon2Config load_config(const std::string& path);
    void safe_hash_to_file(std::string hash,std::string salt,const std::string path);
    bool hash_exists();
};


#endif