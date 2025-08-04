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
    constexpr const char* ACCOUNTS_FILE = "../data/accounts.json";
    /*void addAccount();
    void deleteAccount();
    void editAccout();*/
    void listAccounts(KeyDerivation& kdf);
    void saveAccountToFile(Account& account);
    void deleteAccountInFile(Account& account);
    void getAccount(KeyDerivation& kdf,Account& account);
    bool verifyMasterPassword();
};
/*can you code a function that will getAccount(keyderivation& kdf), will get the full account with password with a specific ID. Dont forget to use the doesIDexist function from before. decrypt password after requesting master password*/
#endif