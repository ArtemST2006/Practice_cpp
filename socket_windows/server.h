#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <iostream>
#include <cstring> 
#include <vector>
#include <thread>
#include <map>

#pragma comment(lib, "ws2_32.lib")

extern std::map<std::thread::id, SOCKET> all_clients;

void communication(SOCKET clientSocket);
SOCKET settings();
int waiting(SOCKET& clientSocket, bool&);

