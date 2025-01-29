#ifdef _WIN32
#include "server.h"

// void waiting(SOCKET& clientSocket){
//     if(all_clients.size() == 1){
//         send(all_clients.begin()->second, "Waiting", 0, 0);
//         return;
//     }
// }  


void communication(SOCKET clientSocket){
    for (auto& client: all_clients){
        if(client.first != std::this_thread::get_id()){
            send(client.second, "Connected", strlen("Connected"), 0);
        }
    }  

    char buffer[1024];
    while (true){
        memset(buffer, 0, sizeof(buffer));
        int btrec = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (btrec < 0){
            std::cerr << "Something went wrong" << std::endl;
            break;
        }
        else if (btrec == 0){
            std::cerr << "Client disconnected" << std::endl;
            break;
        }

        for (auto& client: all_clients){
            if (client.first != std::this_thread::get_id()){
                send(client.second, buffer, strlen(buffer), 0);
            }
        }
        
    }
    all_clients.erase(std::this_thread::get_id());
    if (all_clients.size() == 1){
        send(all_clients.begin()->second, "You are alone", strlen("You are alone"), 0);
    }
    closesocket(clientSocket);
}  
    


SOCKET settings(int port = 8080){
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
    serverAddr.sin_port = htons(port); 
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
    std::cout << "Listening on port 8080..." << std::endl;

    return serverSocket;
}
#endif