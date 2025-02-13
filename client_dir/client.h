#include <iostream>
#include <cstring>
#include <string>
#include <chrono>

#ifdef _WIN32
    #include <winsock2.h>
    #include <windows.h>
    #include <thread>
    using socket_t = SOCKET; 
    #define CLOSESOCKET closesocket
    #pragma comment(lib, "ws2_32.lib")

    void receiv(socket_t);
#else
    #include <sys/types.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #include <netdb.h>
    #include <unistd.h>
    #include <stdlib.h>
    #include <errno.h>

    #include <pthread.h>
    using socket_t = int; 
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define CLOSESOCKET close

    void* receiv(void* );
#endif

using std::cin;
using std::cout;
using std::endl;
using std::cerr;

extern std::string code;

bool connect_server(socket_t& , sockaddr_in& );
socket_t settings();
void encoding(char *, const std::string&);
void decoding(char *, const std::string&);

