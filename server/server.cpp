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

#ifdef _WIN32
std::map<std::thread::id, socket_t> all_clients;
#else
std::set<socket_t> all_clients;
#endif

int main() {
    socket_t serverSocket = setup_server(8080);
    if (serverSocket == -1)
        return 1;
    #ifdef _WIN32
    std::vector<std::thread> threads;
    #else
    std::vector<socket_t> Clients;
    #endif
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
        #ifdef _WIN32
        std::thread new_thread(communication, clientSocket);
        all_clients[new_thread.get_id()] = clientSocket;
        threads.push_back(std::move(new_thread));
        #else
        pthread_t thread_communication;
        pthread_create(&thread_communication, nullptr, communication, &clientSocket);
        Clients.push_back(clientSocket);
        all_clients.insert(clientSocket);
        #endif
        std::cout << "Client connected" << std::endl;
    }
    #ifdef _WIN32
    for (auto& client : all_clients) {
        close_connection(client.second);
    }
    #else

    for (auto client : all_clients) {
        close_connection(client);
    }
    #endif


    close_connection(serverSocket);
    #ifdef _WIN32
    WSACleanup();
    #endif

    return 0;
}
