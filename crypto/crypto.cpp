#include "crypto.h"
#include <random>

Crypto::Crypto() {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<unsigned int> dist(2, P - 2);

    privateKey = dist(gen);
    publicKey = modexp(G, privateKey, P);
}

unsigned int Crypto::getPublicKey() {
    return publicKey;
}

void Crypto::computeSharedSecret(int userId, unsigned int peerPublicKey) {
    unsigned int secret = modexp(peerPublicKey, privateKey, P);
    sharedSecrets[userId] = secret;
}

unsigned int Crypto::modexp(unsigned int base, unsigned int exp, unsigned int mod) {
    unsigned int result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = (result * base) % mod;
        exp >>= 1;
        base = (base * base) % mod;
    }
    return result;
}

std::string Crypto::encrypt(std::string& message, int userId) {
    auto it = sharedSecrets.find(userId);
    if (it == sharedSecrets.end()) return {};

    unsigned int secret = it->second;
    std::string result = message;

    for (size_t i = 0; i < result.size(); ++i)
        result[i] ^= (secret >> (8 * (i % 8))) & 0xFF;

    return result;
}

std::string Crypto::decrypt(std::string& message, int userId) {
    return encrypt(message, userId); 
}
