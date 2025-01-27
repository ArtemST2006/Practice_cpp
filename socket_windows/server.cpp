#include "server.h"

std::map<std::thread::id, SOCKET> all_clients;

int main() {
    SOCKET serverSocket = settings();
    if (serverSocket == INVALID_SOCKET)
        return 1;

    std::vector<std::thread> threads;
    int timeout = 0;
    while (true){  // waiting 2 cliens !!сделать ограничения

        if (all_clients.size() == 2) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }
        else 
            SOCKET client_socket = accept(serverSocket, nullptr, nullptr);

        if (client_socket == INVALID_SOCKET) {
            std::cerr << "Ошибка подключения клиента.\n";
            continue;
        }
        
        std::thread new_thread(communication, client_socket);
        all_clients[new_thread.get_id()] = client_socket; 
        threads.push_back(std::move(new_thread));

        std::cout << "Client connected!" << std::endl;
    }
    
    for (auto& client: all_clients){
        closesocket(client.second);
    }

    closesocket(serverSocket);
    for (auto& t : threads) {
        if (t.joinable()) t.join();
    }
    WSACleanup();

    return 0;
}
