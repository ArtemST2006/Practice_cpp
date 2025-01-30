#include "server.h"

std::map<std::thread::id, socket_t> all_clients;


int main() {
    socket_t serverSocket = setup_server(8080);
    if (serverSocket == -1)
        return 1;

    std::vector<std::thread> threads;
    while (true) {
        socket_t clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket < 0) {
            std::cerr << "Accept failed" << std::endl;
            continue;
        }

        if (all_clients.size() >= 2) {
            const char* resp = "Go away";
            send(clientSocket, resp, strlen(resp), 0);
            close_connection(clientSocket);
            continue;
        }

        std::thread new_thread(communication, clientSocket);
        all_clients[new_thread.get_id()] = clientSocket;
        threads.push_back(std::move(new_thread));
        std::cout << "Client connected" << std::endl;
    }

    // Закрытие всех соединений и завершение работы сервера
    for (auto& client : all_clients) {
        close_connection(client.second);
    }


    close_connection(serverSocket);
    #ifdef _WIN32
    WSACleanup();
    #endif

    return 0;
}
