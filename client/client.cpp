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


#include "client.h"

std::string code;

int main() {
    socket_t clientSocket = settings();
    if (clientSocket == INVALID_SOCKET) return 1;

    char name[30];
    cout << "Enter a name: ";
    cin.getline(name, sizeof(name));
    if (strcmp(name, "secret") == 0) {
        cout << "01101101 01111001 01111001 01110101 01111000 00111010 00101111 00101111 01100010 01100010 01100010 00101110 01100100 01110100 01111010 01111001 01111010 01100111 01101010 00101110 01101000 01110100 01110010 00101111 01100010 01100110 01111001 01101000 01101101 00111111 01100001 00111101 01101001 01010110 01100010 00110100 01100010 00111001 01000010 01101100 01000011 01101000 01010110" << endl;
    }
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
