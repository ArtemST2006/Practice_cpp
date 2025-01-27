

#ifdef _WIN32 
#include "for_windows/server.h"
#elif __unix__
#include "for_unix/server.cpp"
#endif

int main (){
    int res = 0;
    #ifdef _WIN32 
    res = windows_server();
    #elif __unix__
    res = unix_server();
    #endif

    return res;
}