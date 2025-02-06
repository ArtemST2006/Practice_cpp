#include <string>

const int SIZE_OF_ALPH = 26;
const char FIRST_EL = 'a';

void encoding(char *lis, const std::string& code){
    int sdvig_of_encode = std::stoi(code);
    for(int i = 0; lis[i] != '\0'; i++) {
        char now_char = lis[i];
        if (now_char >= FIRST_EL && now_char <= 'z'){
            lis[i] = FIRST_EL + (now_char - FIRST_EL + sdvig_of_encode) % SIZE_OF_ALPH;    
        }
    }
    return;
}

void decoding(char *lis, const std::string& code){
    int sdvig_of_decode = std::stoi(code);
    for(int i = 0; lis[i] != '\0'; i++) {
        char now_char = lis[i];
        if (now_char >= FIRST_EL && now_char <= 'z') {
            lis[i] = FIRST_EL + (now_char - FIRST_EL - sdvig_of_decode + SIZE_OF_ALPH) % SIZE_OF_ALPH;        
        }
    }
    return;
}
