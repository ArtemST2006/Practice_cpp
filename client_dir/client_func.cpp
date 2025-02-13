/*  
===========================================================
|   CLIENT-SERVER APPLICATION                             |
|   Simple multi-client server implementation             |
|   Created by:                                           |
|   - Arstep (https://github.com/ArtemST2006)             |
|   - Senpa1ka (https://github.com/Senpa1k)               |
|   - Glafira (https://github.com/Glafira-Kharitonova)    |
|                                                         |
===========================================================
*/

#include "client.h"


#ifdef _WIN32
void receiv(socket_t clientSocket) 
#else
void* receiv(void* clientSocket2) 
#endif
{
    #ifndef _WIN32
    socket_t clientSocket = *((socket_t*)clientSocket2);
    #endif
    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        if (recv(clientSocket, buffer, 1024, 0) == SOCKET_ERROR) break;
        decoding(buffer, code);
        cout << buffer << endl;
    }
    
    #if defined(__unix) || defined(__APPLE__)
    return nullptr;
    #endif
}

bool connect_server(socket_t& clientSocket, sockaddr_in& serverAddr) {
    int timeout = 0;
    while (timeout < 10) {
        if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) != SOCKET_ERROR) {
            return true;
        } else {
            cout << "Please, wait.....";
            #ifdef _WIN32
                Sleep(500);
            #else
                usleep(500000);
            #endif
            timeout += 2;
        }
    }
    return false;
}

socket_t settings() {
    #ifdef _WIN32
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            cerr << "WSAStartup failed: " << WSAGetLastError() << endl;
            return INVALID_SOCKET;
        }
    #endif

    socket_t clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "Socket creation failed" << endl;
        return INVALID_SOCKET;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (!connect_server(clientSocket, serverAddr)) {
        cerr << "Connection failed" << endl;
        CLOSESOCKET(clientSocket);
        #ifdef _WIN32
            WSACleanup();
        #endif
        return INVALID_SOCKET;
    }

    cout << "Connected to server!" << endl;
    return clientSocket;
}
