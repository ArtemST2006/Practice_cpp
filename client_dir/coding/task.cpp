#include <string>
#include <cstring>


void encoding(char *lis, const std::string& code) {
    long long sdvig_of_encode = std::stoll(code) % 256;
    for(int i = 0; lis[i] != '\0'; i++) {
        unsigned char now_char = static_cast<unsigned char>(lis[i]);
        lis[i] = static_cast<char>((now_char + sdvig_of_encode) % 256);
    }
    return;
}

void decoding(char *lis, const std::string& code) {
    if (strcmp(lis, "Connected") == 0 || strcmp(lis, "You are alone") == 0) return;
    long long sdvig_of_decode = std::stoll(code) % 256;
    for(int i = 0; lis[i] != '\0'; i++) {
        unsigned char now_char = static_cast<unsigned char>(lis[i]);
        lis[i] = static_cast<char>((now_char - sdvig_of_decode + 256) % 256);
    }
    return;
}
