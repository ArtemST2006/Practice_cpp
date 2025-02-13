#include <iostream>
#include <cstring>
#include <map>
#include <vector>
#include <set>

#ifdef _WIN32
#include <winsock2.h>
#include <thread>
#include <windows.h>
typedef SOCKET socket_t;  

extern std::map<std::thread::id, socket_t> all_clients;

void communication(socket_t );
#else
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
typedef int socket_t; 

extern std::set<socket_t> all_clients; 

void* communication( void*);
#endif



int setup_server(int);
void close_connection(socket_t& clientSocket);