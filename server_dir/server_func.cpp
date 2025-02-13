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

#include "server.h"


void close_connection(socket_t& client){
    #ifdef _WIN32
    closesocket(client);
    #else
    close(client);
    #endif
}
#ifdef _WIN32
void communication(socket_t clientSocket) {
    for (auto& client : all_clients) {
        if (client.first != std::this_thread::get_id()) {
            send(client.second, "Connected", strlen("Connected"), 0);
        }
    }

    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            std::cerr << "Client disconnected" << std::endl;
            break;
        }
        std::cout << buffer << std::endl;
        for (auto& client : all_clients) {
            if (client.first != std::this_thread::get_id()) {
                send(client.second, buffer, bytes_received, 0);
            }
        }
    }
    all_clients.erase(std::this_thread::get_id());
    if (all_clients.size() == 1) {
        send(all_clients.begin()->second, "You are alone", strlen("You are alone"), 0);
    }

    #ifdef _WIN32
    closesocket(clientSocket);
    #else
    close(clientSocket);
    #endif
}
#else
void* communication(void* clientSocket2) {
    socket_t clientSocket = *((socket_t*)clientSocket2);
    for (auto client : all_clients) {
        if (client != clientSocket) {
            send(client, "Connected", strlen("Connected"), 0);
        }
    }

    char buffer[1024];

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            std::cerr << "Client disconnected" << std::endl;
            break;
        }
        std::cout << buffer << std::endl;
        for (auto client : all_clients) {
            if (client != clientSocket) {
                send(client, buffer, bytes_received, 0);
            }
        }
    }
    
    if (all_clients.size() == 1) {
        send(clientSocket, "You are alone", strlen("You are alone"), 0);
    }

    #ifdef _WIN32
    closesocket(clientSocket);
    #else
    close(clientSocket);
    #endif
    return nullptr;
}
#endif

int setup_server(int port = 8080) {
    #ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
        return -1;
    }
    socket_t serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    #else
    socket_t serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    #endif

    if (serverSocket == -1) {
        std::cerr << "Socket creation failed" << std::endl;
        return -1;
    }

    sockaddr_in serverAddr {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        #ifdef _WIN32
        closesocket(serverSocket);
        WSACleanup();
        #else
        close(serverSocket);
        #endif
        return -1;
    }

    if (listen(serverSocket, 2) < 0) {
        std::cerr << "Listen failed" << std::endl;
        #ifdef _WIN32
        closesocket(serverSocket);
        WSACleanup();
        #else
        close(serverSocket);
        #endif
        return -1;
    }

    #ifdef _WIN32
    std::cout << "Win serevr..." << port << "..." << std::endl;
    #else
    std::cout << "Unix server..." << port << "..." << std::endl;
    #endif
    return serverSocket;
}