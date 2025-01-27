#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <iostream>
#include <cstring> 
#include <chrono>
#include <thread> 
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")


using std::cin;
using std::cout;
using std::endl;
using std::cerr;


bool connect_server(SOCKET& , sockaddr_in& );
SOCKET settings();
void receiv(SOCKET clientSocket);