#include "client.h"


void receiv(SOCKET clientSocket){
    char buffer[1024];
    while (true){
        memset(buffer, 0, 1024);
        if (recv(clientSocket, buffer, 1024, 0) == -1)
            break;
        cout << buffer << endl;
    }
}


bool connect_server(SOCKET& clientSocket, sockaddr_in& serverAddr){
    int timeout = 0;
    while (timeout < 10){
        if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) != SOCKET_ERROR) {
            return true;
        }
        else{
            cout << "Please, wait" << ".....";
#if __GNUC__ < 11
            Sleep(500);         
#else
            std::this_thread::sleep_for(std::chrono::seconds(2));

#endif
            timeout += 2;   
        }
    }
    return false;
}


SOCKET settings(){
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
        return INVALID_SOCKET;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation : " << WSAGetLastError() << std::endl;
        WSACleanup();
        return INVALID_SOCKET;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080); 
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Локальный сервер

    bool is_connected = connect_server(clientSocket, serverAddr);
    if (!is_connected){
        cerr << "Connection failed" << endl;
        closesocket(clientSocket);
        WSACleanup();
        return INVALID_SOCKET;
    }
    cout << "Connected to server!" << std::endl;

    return clientSocket;
}