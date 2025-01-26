#include "server.h"

SOCKET settings(){
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
        return INVALID_SOCKET;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return INVALID_SOCKET;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080); 
    serverAddr.sin_addr.s_addr = INADDR_ANY; 

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return INVALID_SOCKET;
    }

    if (listen(serverSocket, 2) == SOCKET_ERROR) {
        std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return INVALID_SOCKET;
    }
    std::cout << "Server is listening on port 8080..." << std::endl;

    return serverSocket;
}


void communication(SOCKET clientSocket){
    char buffer[1024];
    bool flag = false;
    while (true) {
        memset(buffer, 0, sizeof(buffer));

        int con_or_br = waiting(clientSocket, flag);
        if (con_or_br == 0)
            break;
        else if (con_or_br == 1 && flag){
            flag = false;
            send(clientSocket, "Connected", strlen("Connected"), 0);
        }

        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesReceived <= 0) {
            std::cout << "Client disconnected." << std::endl;
            break;
        }
        
        std::cout << "Received: " << buffer << std::endl;
        
        if (strcmp(buffer, "stop") == 0){
            std::cout << "Client disconnected." << std::endl;
            all_clients.erase(std::this_thread::get_id());
            send(all_clients.begin()->second, "You are alone", strlen("You are alone"), 0); 
            
            break;
        }

        SOCKET right_client;
        for (auto x: all_clients){
            if (x.first != std::this_thread::get_id())
                right_client = x.second;
        }
        send(right_client, buffer, strlen(buffer), 0);

    }

    closesocket(clientSocket);
}


int waiting(SOCKET& clientSocket, bool& flag){
    int timeout = 0;
    while (timeout++ < 10){
        if (all_clients.size() == 2)
            return 1;
        flag = true;
        char response[] = "Waiting";
        send(clientSocket, response, strlen(response), 0);
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    return 0;
}