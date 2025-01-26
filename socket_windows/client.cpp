#include "client.h"


int main (){
    SOCKET clientSocket = settings();
    if (clientSocket == INVALID_SOCKET)
        return 1;

    char message[1024];
    char buffer[1024];
    
    while (true) {
        int received = recv(clientSocket, buffer, sizeof(buffer) - 1, 0); 
        if (received < 0){
            cout << "Something went wrong" << endl;
            break;
        }
        else if (received == 0){
            cout << "Server disconnected" << endl;
            break;
        }
        else{
            buffer[received] = '\0';
            cout << buffer << endl;
            if (strcmp(buffer, "Waiting") == 0)
                continue;
        }

        cout << "Enter: ";
        while(cin.getline(message, sizeof(message))){
            if (strlen(message) != 0)
                break;
            cout << "Enter again: ";
        }

        if (send(clientSocket, message, strlen(message), 0) == SOCKET_ERROR) {
            std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
            break;
        }
    }

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
