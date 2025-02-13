#include <string>
#include <cstring>

void encoding(char *lis, const std::string& code){
    int sdvig_of_encode = std::stoi(code);
    for(int i = 0; lis[i] != '\0'; i++) {
        char now_char = lis[i];
        lis[i] = now_char + sdvig_of_encode;    
    }
    return;
}

void decoding(char *lis, const std::string& code){
    if (strcmp(lis, "Connected") == 0 || strcmp(lis, "You are alone") == 0) return;
    int sdvig_of_decode = std::stoi(code);
    for(int i = 0; lis[i] != '\0'; i++) {
        char now_char = lis[i];
        lis[i] = now_char - sdvig_of_decode;
    }
    return;
}
