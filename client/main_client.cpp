

#ifdef _WIN32
#include "windows/client.h"
#elif __unix__
#include "unix/client.h"
#endif

int main (){
    int res = 0;
    #ifdef _WIN32
    res = windows_client();
    #elif __unix__
    res = unix_client();
    #endif

    return res;
}