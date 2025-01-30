#include <iostream>
#include <cstring>
#include <map>
#include <thread>
#include <vector>

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
typedef SOCKET socket_t;  // Для Windows используем SOCKET
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
typedef int socket_t;  // Для Unix-подобных систем используем int
#endif

extern std::map<std::thread::id, socket_t> all_clients;

int setup_server(int);
void communication(socket_t clientSocket);
void close_connection(socket_t& clientSocket);