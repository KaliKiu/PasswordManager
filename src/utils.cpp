#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <vector>
#include <sstream>     // for std::ostringstream
#include <iomanip>     // for std::hex, std::setw, std::setfill
#include "../include/json.hpp"

namespace utils{
    //clear terminal string

        //check if specific file exists
    bool file_exists(const std::string& path){
        std::fstream file(path);
        return file.good();
    }

        //one word input
    std::string input_word(){
        std::string input;
        while(input.empty()){
            std::getline(std::cin,input);
        }
        return input;
    }

        //random generated in length n
    std::vector<uint8_t> generate_salt(std::size_t length) {
        std::vector<uint8_t> salt(length);
        std::random_device rd;
        std::generate(salt.begin(), salt.end(), std::ref(rd));

        return salt;
    }

        //bytes to hex function
    std::string bytes_to_hex(const std::vector<uint8_t>& data) {
        std::ostringstream oss;
        for (auto b : data) {
            oss << std::hex << std::setw(2) << std::setfill('0') << (int)b;
            }
        return oss.str();
    }

        //hex to bytes function
    std::vector<uint8_t> hex_to_bytes(std::string hex) {
        if (hex.length() % 2 != 0) {
            throw std::invalid_argument("Hex string length must be even.");
        }
        std::vector<uint8_t> bytes;
        for (size_t i = 0; i < hex.length(); i += 2) {
            std::string byteStr = hex.substr(i, 2);

            uint8_t byte = static_cast<uint8_t>(std::stoi(byteStr, nullptr, 16));

            bytes.push_back(byte);
        }
        return bytes;
    }
    //store a value into JsonFile
    void storeJsonValueInFile(std::string value, const std::string index, const std::string path){
        std::ifstream file_in(path);
        nlohmann::json json;
        file_in >> json;
        json[index]=value;
        file_in.close();
        std::ofstream file_out(path);
        file_out <<json.dump(4);
        file_out.close();
    }
    std::string getJsonValueFromFile(std::string index,const std::string path){
        std::ifstream file(path);
        nlohmann::json json;
        file >> json;
        return json[index];
    }
    bool isIdUnique(nlohmann::json& data, std::string idToCheck) {
    if (!data.contains("Accounts") || !data["Accounts"].is_array()) {
        return true; // No accounts means the ID is unique
    }

    for (const auto& account : data["Accounts"]) {
        if (account.contains("id") && account["id"] == idToCheck) {
            return false; // ID already exists
        }
    }
    return true; // ID is unique
}
}