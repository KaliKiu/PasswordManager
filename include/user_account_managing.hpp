#ifndef USER_ACCOUNT_MANAGING
#define USER_ACCOUNT_MANAGING

#include <iostream>
class KeyDerivation;

namespace User{
    struct LoggedInStatus{
    bool isLoggedin = false;
    std::string username;
    std::string input;
};
    void parseUserInput(KeyDerivation& kdf, std::string input,LoggedInStatus& session);
    void userInput_editAccount(KeyDerivation& kdf);
    void userInput_addAccount(KeyDerivation& kdf);
    void userInput_deleteAccount(KeyDerivation& kdf);
    void userInput_logout(KeyDerivation& kdf);
}
#endif