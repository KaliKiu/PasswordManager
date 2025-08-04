#include <iostream>
#include <fstream>
#include <cstdio>
#include "../include/key_derivation.hpp"
#include "../include/vault.hpp"
#include "../include/login.hpp"
#include "../include/utils.hpp"
#include "../include/encryption.hpp"
namespace Vault{

    bool verifyMasterPassword() {
        std::cout << "Enter masterpassword: \n";
        std::string passwordin = utils::input_word();

        std::string stored_hash = utils::getJsonValueFromFile("hash", Login::MASTER_HASH_FILE);
        std::string stored_salt = utils::getJsonValueFromFile("salt", Login::MASTER_HASH_FILE);

        std::string input_hash = utils::bytes_to_hex(
            Login::hash_password(passwordin, utils::hex_to_bytes(stored_salt))
        );

        if (input_hash != stored_hash) {
            std::cout << "Wrong password, returning...\n";
            return false;
        }

        std::cout << "Correct\n";
        return true;
    }

    void listAccounts(KeyDerivation& kdf) {
        using json = nlohmann::json;
        json vault_json;

        std::ifstream inFile(Vault::ACCOUNTS_FILE);
        if (inFile.is_open()) {
            if (inFile.peek() == std::ifstream::traits_type::eof()) {
                std::cout << "No accounts stored.\n";
                return;
            } else {
                try {
                    inFile >> vault_json;
                } catch (const std::exception& e) {
                    std::cerr << "Error parsing accounts file: " << e.what() << "\n";
                    return;
                }
            }
            inFile.close();
        } else {
            std::cout << "Failed to open accounts file.\n";
            return;
        }

        if (!vault_json.contains("Accounts") || !vault_json["Accounts"].is_array()) {
            std::cout << "Accounts list is missing or invalid.\n";
            return;
        }

        const auto& accounts = vault_json["Accounts"];
        if (accounts.empty()) {
            std::cout << "No accounts stored.\n";
            return;
        }

        std::cout << "\nStored Accounts:\n";
        std::cout << "-------------------------\n";

        for (const auto& acc : accounts) {
            std::string id = acc.value("id", "[No ID]");
            std::string username = acc.value("username", "[No Username]");
            std::string notes = acc.value("notes", "");

            std::cout << "ID:       " << id << "\n";
            std::cout << "Username: " << username << "\n";
            std::cout << "Notes:    " << notes << "\n";
            std::cout << "-------------------------\n";
        }
    }

    void saveAccountToFile(Account& account) {
        using json = nlohmann::json;
        json vault_json;

        std::ifstream inFile(Vault::ACCOUNTS_FILE);
        if (inFile.is_open()) {
            if (inFile.peek() == std::ifstream::traits_type::eof()) {
                // File is empty → initialize new JSON
                vault_json = json::object();
            } else {
                try {
                    inFile >> vault_json;
                } catch (const std::exception& e) {
                    std::cerr << "Error parsing JSON: " << e.what() << "\n";
                    vault_json = json::object(); // reset if parsing fails
                }
            }
            inFile.close();
        } else {
            // File doesn't exist → start fresh
            vault_json = json::object();
        }

        // Ensure "Accounts" array exists
        if (!vault_json.contains("Accounts") || !vault_json["Accounts"].is_array()) {
            vault_json["Accounts"] = json::array();
        }

        // Check if ID is unique
        if (utils::isIdUnique(vault_json, account.id)) {
            // Create JSON object from Account
            json newAccount = {
                {"id", account.id},
                {"username", account.username},
                {"encrypted_password", account.encrypted_password},
                {"notes", account.notes}
            };

            // Append to "Accounts" array
            vault_json["Accounts"].push_back(newAccount);

            // Save updated JSON back to file
            std::ofstream outFile(Vault::ACCOUNTS_FILE);
            if (outFile.is_open()) {
                outFile << vault_json.dump(4);
                outFile.close();
            } else {
                std::cerr << "Error opening output file\n";
            }
        } else {
            std::cout << "ID already exists. Account not added.\n";
        }
    }

    void deleteAccountInFile(Account& account) {
        if(!verifyMasterPassword()){
            return;
        }
        using json = nlohmann::json;
        json vault_json;

        // Load JSON safely
        std::ifstream inFile(Vault::ACCOUNTS_FILE);
        if (inFile.is_open()) {
            if (inFile.peek() == std::ifstream::traits_type::eof()) {
                std::cout << "No accounts found.\n";
                return;
            } else {
                try {
                    inFile >> vault_json;
                } catch (const std::exception& e) {
                    std::cerr << "Error parsing accounts file: " << e.what() << "\n";
                    return;
                }
            }
            inFile.close();
        } else {
            std::cout << "Failed to open accounts file.\n";
            return;
        }

        // Check if "Accounts" exists
        if (!vault_json.contains("Accounts") || !vault_json["Accounts"].is_array()) {
            std::cout << "Accounts list is missing or invalid.\n";
            return;
        }

        // Search for account with matching id and erase it
        auto& accounts = vault_json["Accounts"];
        bool found = false;

        for (auto it = accounts.begin(); it != accounts.end(); ++it) {
            if (it->contains("id") && (*it)["id"] == account.id) {
                accounts.erase(it);
                found = true;
                break;
            }
        }

        if (!found) {
            std::cout << "No account found with id: " << account.id << "\n";
            return;
        }

        // Save back to file
        std::ofstream outFile(Vault::ACCOUNTS_FILE);
        if (outFile.is_open()) {
            outFile << vault_json.dump(4);
            outFile.close();
            std::cout << "Account with id '" << account.id << "' has been deleted.\n";
        } else {
            std::cerr << "Failed to write to accounts file.\n";
        }
    }
    void getAccount(KeyDerivation& kdf, Account& account) {
        using json = nlohmann::json;

        std::string targetId = account.id;

        // Load vault JSON
        json vault_json;
        std::ifstream inFile(Vault::ACCOUNTS_FILE);
        if (inFile.is_open()) {
            if (inFile.peek() == std::ifstream::traits_type::eof()) {
                std::cout << "No accounts stored.\n";
                return;
            } else {
                try {
                    inFile >> vault_json;
                } catch (const std::exception& e) {
                    std::cerr << "Error parsing accounts file: " << e.what() << "\n";
                    return;
                }
            }
            inFile.close();
        } else {
            std::cout << "Failed to open accounts file.\n";
            return;
        }

        // If ID is unique, it does not exist
        if (utils::isIdUnique(vault_json, targetId)) {
            std::cout << "No account found with ID: " << targetId << "\n";
            return;
        }

        // Verify master password
        if (!Vault::verifyMasterPassword()) return;

        // Search for account
        const auto& accounts = vault_json["Accounts"];
        for (const auto& acc : accounts) {
            if (acc.contains("id") && acc["id"] == targetId) {
                std::string username = acc.value("username", "[No Username]");
                std::string notes = acc.value("notes", "");
                std::string encrypted_password = acc.value("encrypted_password", "");

                std::string decrypted_password = Encryption::decryptPassword(kdf, encrypted_password);

                std::cout << "\n--- Account Details ---\n";
                std::cout << "ID:        " << targetId << "\n";
                std::cout << "Username:  " << username << "\n";
                std::cout << "Password:  " << decrypted_password << "\n";
                std::cout << "Notes:     " << notes << "\n";
                std::cout << "------------------------\n";
                return;
            }
        }

        // Should not reach here if ID check worked
        std::cout << "Unexpected error: ID not found during final search.\n";
    }
}
