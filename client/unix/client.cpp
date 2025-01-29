#ifdef __unix__

#include "client.h"


mutex consoleMutex;

void clearCurrentLine() {
    lock_guard<mutex> lock(consoleMutex);
    std::cout << "\033[2K\r" << std::flush;
}

void receiveMessages(int socketHandle) {
    char buf[2048];
    while (true) {
        int rc = recv(socketHandle, buf, sizeof(buf), 0);
        if (rc <= 0) {
            if (rc == 0) {
                lock_guard<mutex> lock(consoleMutex);
                cout << "Server disconnected." << endl;
            } else {
                lock_guard<mutex> lock(consoleMutex);
                cerr << "Failed to receive message from server." << endl;
            }
            exit(EXIT_FAILURE);
        }

        buf[rc] = '\0';
        lock_guard<mutex> lock(consoleMutex);
        cout << buf << endl;
    }
}

int unix_client() {
    struct sockaddr_in remoteSocketInfo;
    struct hostent* hPtr;
    int socketHandle;
    const char* remoteHost = "localhost";
    int portNumber = 8080;

    bzero(&remoteSocketInfo, sizeof(sockaddr_in));

    if ((hPtr = gethostbyname(remoteHost)) == NULL) {
        cerr << "System DNS name resolution not configured properly." << endl;
        cerr << "Error number: " << ECONNREFUSED << endl;
        exit(EXIT_FAILURE);
    }

    if ((socketHandle = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        close(socketHandle);
        exit(EXIT_FAILURE);
    }

    memcpy((char*)&remoteSocketInfo.sin_addr, hPtr->h_addr, hPtr->h_length);
    remoteSocketInfo.sin_family = AF_INET;
    remoteSocketInfo.sin_port = htons((u_short)portNumber);

    if (connect(socketHandle, (struct sockaddr*)&remoteSocketInfo, sizeof(sockaddr_in)) < 0) {
        close(socketHandle);
        exit(EXIT_FAILURE);
    }





    thread receiverThread(receiveMessages, socketHandle);
    receiverThread.detach();

    while (true) {
        string messagesToSend;
        getline(cin, messagesToSend);
        clearCurrentLine();
        if (send(socketHandle, messagesToSend.c_str(), messagesToSend.length(), 0) < 0) {
            cerr << "Failed to send message to server." << endl;
            break;
        }

        
    }

    close(socketHandle);
    return 0;
}

#endif