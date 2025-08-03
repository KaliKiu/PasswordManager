#ifndef KEY_DERIVATION
#define KEY_DERIVATION

#include <iostream>
#include <fstream>
#include <cstdio>
#include <argon2.h>
#include "../include/json.hpp"
namespace Login{
    struct Argon2Config;
}
class KeyDerivation{
public:
    void kdfGen(std::string& password, Login::Argon2Config& config_arg2,nlohmann::json json);
    void kdfGenerateSalt(std::size_t length,const std::string index,const std::string path);
    std::vector<uint8_t> getDerivedKey();
    void resetDerivedKey();
    bool isderivedKeyempty();
private:
    std::vector<uint8_t> derivedKey;
};
#endif