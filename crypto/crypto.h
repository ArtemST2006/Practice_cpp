// crypto.h
#ifndef CRYPTO_H
#define CRYPTO_H

#include <string>
#include <unordered_map>
#include <cstdint>

class Crypto {
private:
    unsigned int privateKey;
    unsigned int publicKey;
    static const unsigned int P = 2147483647;
    static const unsigned int G = 5;
    std::unordered_map<int, unsigned int> sharedSecrets;

    static unsigned int modexp(unsigned int base, unsigned int exp, unsigned int mod);

public:
    Crypto();
    unsigned int getPublicKey();
    void computeSharedSecret(int userId, unsigned int peerPublicKey);
    std::string encrypt(std::string& message, int userId);
    std::string decrypt(std::string& message, int userId);

};

#endif