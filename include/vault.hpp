#ifndef VAULTS
#define VAULTS
#include <iostream>
namespace Vault{
    struct Account{
        std::string id;
        std::string username;
        std::string encrypted_password;
        std::string notes;
    };
    /*void addAccount();
    void deleteAccount();
    void editAccout();*/
    void listAccounts(KeyDerivation& kdf);
}

#endif