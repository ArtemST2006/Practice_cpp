

#ifdef _WIN32 
#include "windows/server.h"
#elif __unix__
#include "unix/server.h"
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