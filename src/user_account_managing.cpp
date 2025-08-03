#include <iostream>
#include "../include/key_derivation.hpp"
#include "../include/utils.hpp"
#include "../include/user_account_managing.hpp"
#include "../include/vault.hpp"
#include "../include/encryption.hpp"
namespace User{

    void parseUserInput(KeyDerivation& kdf,std::string input,User::LoggedInStatus& session){
        //use key-value pair for mapping input to function
        std::unordered_map<std::string,std::function<void(KeyDerivation& kdf)>> commandMap = {
            {"edit",User::userInput_editAccount},
            {"add",User::userInput_addAccount},
            {"delete",User::userInput_deleteAccount},
            {"list",Vault::listAccounts},
            {"logout",User::userInput_logout}
        };
        if(commandMap.find(input) !=commandMap.end()){
            commandMap[input](kdf);
        }else{
            std::cout <<utils::CLEAR;
            std::cout <<"Unknown command\n";
        }
    }
    void userInput_editAccount(KeyDerivation& kdf){

    }
    void userInput_addAccount(KeyDerivation& kdf){
        Vault::Account account;
        std::cout <<"Website name: ";
        account.id = utils::input_word();
        std::cout <<"\nUsername: ";
        account.username = utils::input_word();
        std::cout <<"\nPassword: ";
        std::string password = utils::input_word();
        account.encrypted_password = Encryption::encryptPassword(kdf,password);
        std::cout <<"\nEncrypted: "<<account.encrypted_password <<"\nDecrypted: "<<Encryption::decryptPassword(kdf,Encryption::encryptPassword(kdf,password));
        std::cout <<"\nNotes: ";
        account.notes = utils::input_word();
        Vault::saveAccountToFile(account);
    }
    void userInput_deleteAccount(KeyDerivation& kdf){
        std::cout <<"delete\n";
    }
    void userInput_logout(KeyDerivation& kdf){
        kdf.resetDerivedKey();
    }

}