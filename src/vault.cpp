#include <iostream>
#include <fstream>
#include <cstdio>
#include "../include/key_derivation.hpp"
#include "../include/vault.hpp"

namespace Vault{
    void listAccounts(KeyDerivation& kdf){
        std::cout <<"edit";
    }
    void saveAccountToFile(Account& account) {
        using json = nlohmann::json;

        // Read existing data or create new
        json vault_json;
        std::ifstream inFile(Vault::ACCOUNTS_FILE);
        if (inFile.is_open()) {
            inFile >> vault_json;
            inFile.close();
        }
        // Ensure "Accounts" exists and is an array
        if (!vault_json.contains("Accounts") || !vault_json["Accounts"].is_array()) {
            vault_json["Accounts"] = json::array();
        }

        // Create JSON object from Account
        json newAccount = {
            {"id", account.id},
            {"username", account.username},
            {"encrypted_password", account.encrypted_password},
            {"notes", account.notes}
        };

        // Append to "Accounts" array
        vault_json["Accounts"].push_back(newAccount);

        // Save back to file
        std::ofstream outFile(Vault::ACCOUNTS_FILE);
        outFile << vault_json.dump(4);  // Pretty-print with 4-space indent
        outFile.close();
    }

}
