#ifndef ENCRYPTION
#define ENCRYPTION
#include <iostream>

class KeyDerivation;

namespace Encryption{
    std::string encryptPassword(KeyDerivation& kdf,std::string password);
    std::string decryptPassword(KeyDerivation& kdf,std::string ciphertext);
}
#endif