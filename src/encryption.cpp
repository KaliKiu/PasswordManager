#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include "../include/key_derivation.hpp"
#include <string>
#include <vector>
#include <stdexcept>

namespace Encryption{

    std::string encryptPassword(KeyDerivation& kdf,std::string password) {
        using namespace CryptoPP;

        std::string ciphertext;
        ECB_Mode<AES>::Encryption encryptor;
        encryptor.SetKey(kdf.getDerivedKey().data(), kdf.getDerivedKey().size());

        StringSource ss(password, true,
            new StreamTransformationFilter(encryptor,
                new HexEncoder(
                    new StringSink(ciphertext)
                )
            )
        );
        return ciphertext;
    }
    std::string decryptPassword(KeyDerivation& kdf,std::string ciphertext) {
    using namespace CryptoPP;

    std::string recovered;
    ECB_Mode<AES>::Decryption decryptor;
    decryptor.SetKey(kdf.getDerivedKey().data(), kdf.getDerivedKey().size());

    StringSource ss(ciphertext, true,
        new HexDecoder(
            new StreamTransformationFilter(decryptor,
                new StringSink(recovered)
            )
        )
    );
    return recovered;
}

}