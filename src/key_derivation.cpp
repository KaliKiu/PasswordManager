#include <iostream>
#include <fstream>
#include <cstdio>
#include <argon2.h>
#include "../include/login.hpp"
#include "../include/json.hpp"
#include "../include/utils.hpp"
#include "../include/key_derivation.hpp"

    std::vector<uint8_t> derivedKey;

    void KeyDerivation::kdfGen(std::string& password, Login::Argon2Config& config_arg2,nlohmann::json json){
        std::string salt = json.at("salt_KDF");
        this->derivedKey= Login::hash_password(password, utils::hex_to_bytes(salt),config_arg2);
    }
    void KeyDerivation::kdfGenerateSalt(std::size_t length,const std::string index,const std::string path){
        utils::generate_salt(length);
        utils::storeJsonValueInFile(utils::bytes_to_hex(utils::generate_salt(length)),index,path);
    }
    std::vector<uint8_t> KeyDerivation::getDerivedKey(){
        return this->derivedKey;
    }
    void KeyDerivation::resetDerivedKey(){
        this->derivedKey.clear();
    }
    bool KeyDerivation::isderivedKeyempty(){
        if(this->derivedKey.empty()){
            return true;
        }else{
            return false;
        }
    }
