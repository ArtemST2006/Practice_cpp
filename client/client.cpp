#include "client.h"

std::string code;

int main() {
    socket_t clientSocket = settings();
    if (clientSocket == INVALID_SOCKET) return 1;

    char name[30];
    cout << "Enter a name: ";
    cin.getline(name, sizeof(name));
    name[strlen(name)] = ':';
    name[strlen(name) + 1] = '\0';

    cout << "Enter code: ";
    cin >> code;
    cin.ignore();

    char message[1024];

    #ifdef _WIN32
        std::thread new_thread(receiv, clientSocket);
    #else
        pthread_t thread_recive;
        pthread_create(&thread_recive, nullptr, receiv, &clientSocket);
    #endif

    while (true) {
        memset(message, 0, sizeof(message));
        cin.getline(message, sizeof(message));

        if (strcmp(message, "stop") == 0) break;
        memmove(message + strlen(name), message, strlen(message) + 1);
        memcpy(message, name, strlen(name)); 
        encoding(message, code);

        if (send(clientSocket, message, strlen(message), 0) == SOCKET_ERROR) break;
    }

    CLOSESOCKET(clientSocket);

    #ifdef _WIN32
        WSACleanup();
    #endif

    #ifdef _WIN32
        new_thread.join();
    #else
        pthread_join(thread_recive, nullptr);
    #endif

    return 0;
}
