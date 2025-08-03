
#include <iostream>
#include <fstream>
#include <cstdio>
#include <argon2.h>
#include "../include/json.hpp"
#include "../include/utils.hpp"
#include "../include/login.hpp"
#include "../include/key_derivation.hpp"

const std::string ARGON2_CONFIG_FILE = "../config/argon2_config.json";
const std::string MASTER_HASH_FILE = "../data/master_hash.json";

//master login, for accessing his passwords
//run : g++ your_code.cpp -I/usr/local/include -L/usr/local/lib -largon2 -o your_program


using namespace std;
namespace Login{

KeyDerivation log(){
        cout <<utils::CLEAR;
        KeyDerivation kdf;
        while(true){
        if(hash_exists()){
            //login
            cout <<"Login: \n\n";

            //load stored hash
            std::ifstream file(MASTER_HASH_FILE);
            nlohmann::json json;
            file >> json;
            file.close();
            string saved_salt = json.at("salt");
            string saved_hash = json.at("hash");
            
            //hash input password
            Argon2Config config_arg2 = load_config(ARGON2_CONFIG_FILE);
            std::string password = utils::input_word();
            string login_hash = utils::bytes_to_hex(hash_password(password,utils::hex_to_bytes(saved_salt),config_arg2));
            //cout <<"Login_hash: "<<login_hash <<"\n" <<"Saved_hash: " <<saved_hash <<"\n";
            
            //compare
            if(saved_hash==login_hash){
                cout <<utils::CLEAR;
                //generate KD with kdf
                kdf.kdfGen(password,config_arg2,json);
                return kdf;
            }else{
                cout <<utils::CLEAR <<"Password wrong! Try again\n";
            }
            
        }else{
            //register
            cout <<"Register: ";

            Argon2Config config_arg2 = load_config(ARGON2_CONFIG_FILE);
            //generate salt for password
            vector<uint8_t> salt = utils::generate_salt(config_arg2.salt_len);
            //generate unique salt for KD
            kdf.kdfGenerateSalt(128,"salt_KDF",MASTER_HASH_FILE);
            //hash password and safe hash+salt
            std::string password= utils::input_word();
            vector<uint8_t> hash = hash_password(password,salt,config_arg2);
            utils::storeJsonValueInFile(utils::bytes_to_hex(hash),"hash",MASTER_HASH_FILE);
            utils::storeJsonValueInFile(utils::bytes_to_hex(salt),"salt",MASTER_HASH_FILE);
            cout <<utils::CLEAR<<"\nRegistered!\n";
        }
    }
}

    //load config from json file:
Login::Argon2Config load_config(const std::string& path){
    std::ifstream file(path);
    if(!file.is_open()){
        std::cerr <<"Failed to open";
    }
    //error
    nlohmann::json config_json;
    file >> config_json;
    file.close();
    return Argon2Config{
        config_json.at("t_cost").get<uint32_t>(),
        config_json.at("m_cost").get<uint32_t>(),
        config_json.at("parallelism").get<uint32_t>(),
        config_json.at("hash_length").get<size_t>(),
        config_json.at("salt_length").get<size_t>()
    };
}
    //password hash function
vector<uint8_t> hash_password(std::string& password,vector<uint8_t> salt, Argon2Config config_arg2){
    
    vector<uint8_t> hash(config_arg2.hash_len);

    int result = argon2id_hash_raw(
        config_arg2.t_cost,  //time cost
        config_arg2.m_cost,  //memory cost
        config_arg2.parallelism, //parallelism
        password.data(),
        password.size(),
        salt.data(),
        salt.size(),
        hash.data(),
        hash.size()
    );
    if(result!= ARGON2_OK){
        cerr <<"Argon2 error: "<<argon2_error_message(result) <<"\n";
    }
    
    return hash;
}
    //check if user is registered
bool hash_exists(){
    std::ifstream file(MASTER_HASH_FILE);
    nlohmann::json json_hash_file;
    file >> json_hash_file;
    if(json_hash_file.at("hash")==""){
        return false;
    }else{
        return true;
    }
}
};
