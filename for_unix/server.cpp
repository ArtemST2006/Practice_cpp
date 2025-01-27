#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netdb.h>
#include <stdlib.h>
#include <thread>
#include <strings.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <vector>
#include <netinet/in.h>

#define MAXHOSTNAME 256

using namespace std;
int coutOfConnections = 0;
vector<int> clients;
void handleClient(int socketConnection) {
	coutOfConnections++;
	int clientId = coutOfConnections;
	clients.push_back(socketConnection);
	string s = "Client" + to_string(clientId)+": ";

	while (1) {
    char buf[2048];
    int rc = recv(socketConnection, buf, sizeof(buf), 0);
    if (rc <= 0) {
        break;
    }
	memmove(buf + s.length(), buf, rc);
	memcpy(buf, s.c_str(), s.length());
    for (auto i : clients) {
        send(i, buf, rc+s.length(), 0);
    }
}
}


int main() {
    struct sockaddr_in socketInfo;
    char sysHost[MAXHOSTNAME + 1];
    struct hostent* hPtr;
    int socketHandle;
    int portNumber = 8080;


    bzero(&socketInfo, sizeof(sockaddr_in));


    gethostname(sysHost, MAXHOSTNAME);
    if ((hPtr = gethostbyname(sysHost)) == NULL) {
        cerr << "System hostname misconfigured." << endl;
        exit(EXIT_FAILURE);
    }


    if ((socketHandle = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        close(socketHandle);
        exit(EXIT_FAILURE);
    }

    
    socketInfo.sin_family = AF_INET;
    socketInfo.sin_addr.s_addr = htonl(INADDR_ANY); 
    socketInfo.sin_port = htons(portNumber);  


    if (::bind(socketHandle, (struct sockaddr*)&socketInfo, sizeof(struct sockaddr_in)) < 0) {
        close(socketHandle);
        perror("bind");
        exit(EXIT_FAILURE);
    }

    listen(socketHandle, 5);

    
    for (;;) { 
		sockaddr_in clientAddress;
        socklen_t clientAddressSize = sizeof(clientAddress);
        int socketConnection;
        if ((socketConnection = accept(socketHandle, (sockaddr*)&clientAddress, &clientAddressSize)) < 0) {
            close(socketHandle);
            if (errno == EINTR) continue;
            perror("accept");
            exit(EXIT_FAILURE);

        }
		
		std::thread clientThread;
		clientThread = std::thread(handleClient, socketConnection);
		clientThread.detach();

    
	}
    return 0;
}
