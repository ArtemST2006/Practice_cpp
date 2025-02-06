#include "server.h"


std::set<socket_t> all_clients;


int main() {
    socket_t serverSocket = setup_server(8080);
    if (serverSocket == -1)
        return 1;

    std::vector<socket_t> Clients;
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
        pthread_t thread_communication;
        pthread_create(&thread_communication, nullptr, communication, &clientSocket);
        Clients.push_back(clientSocket);
        all_clients.insert(clientSocket);
        std::cout << "Client connected" << std::endl;
    }

    // Закрытие всех соединений и завершение работы сервера
    for (auto client : all_clients) {
        close_connection(client);
    }


    close_connection(serverSocket);
    #ifdef _WIN32
    WSACleanup();
    #endif

    return 0;
}
