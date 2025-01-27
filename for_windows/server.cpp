#include "server.h"


std::map<std::thread::id, SOCKET> all_clients;

int main(){
    SOCKET serverSocket = settings(8080);
    if (serverSocket == INVALID_SOCKET)
        return 1;
    int timeout = 0;

    std::vector<std::thread> threads;
    while (true){  // waiting 2 cliens !!сделать ограничения
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Error" << std::endl;
            continue;
        }

        if (all_clients.size() >= 2) {
            char resp[] = "Go away";
            send(clientSocket, resp, strlen(resp), 0);
            closesocket(clientSocket);
            continue;
        }

        std::thread new_thread(communication, clientSocket);
        all_clients[new_thread.get_id()] = clientSocket;
        threads.push_back(std::move(new_thread));

        std::cout << "Client connected" << std::endl;
        
    }

    for (auto& client: all_clients)
        closesocket(client.second);
    
    for (auto& client: all_clients)
        closesocket(client.second);

    closesocket(serverSocket);
    WSACleanup();

    return 0;
}




