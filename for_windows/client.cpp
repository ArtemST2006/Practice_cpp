#include "client.h"


int windows_client (){
    //создание и привязка сокета
    SOCKET clientSocket = settings();
    if (clientSocket == INVALID_SOCKET)
        return 1;

    char name[30];
    std::cout << "Enter a name: ";
    cin.getline(name, sizeof(name));
    name[strlen(name)] = ':';
    name[strlen(name) + 2] = '\0';

    char message[1024];

    //новый поток, связанный с функцией receiv, для приема сообщений
    std::thread new_thread(receiv, clientSocket);

    //основной поток для отправки
    while (true) {
        memset(message, 0, 1024);
        cin.getline(message, sizeof(message));
        if (strcmp(message, "stop") == 0)
            break;

        memmove(message + strlen(name), message, strlen(name) + 1);
        memcpy(message, name, strlen(name));

        if (send(clientSocket, message, strlen(message), 0) == -1)
            break;
    }
    closesocket(clientSocket);
    new_thread.join();
    
    WSACleanup();

    return 0;
}

