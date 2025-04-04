#include "client.h"

void Client::settings(int port, std::string host){
    struct sockaddr_in serv_addr;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    inet_pton(AF_INET, host.c_str(), &serv_addr.sin_addr);
        
    connect(socket_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    //обмен id и name 
    {
        recv(socket_fd, &id, sizeof(id), 0);
        id = ntohl(id);
        message msg{};
        msg.type = 0; strcpy(msg.buffer, name.c_str()); msg.adress = -1;
        send(socket_fd, &msg, sizeof (msg), 0);
    }

    cout << "Connect with id: " <<  id << endl;
    connected = true;
}


void Client::listen(){
    while (connected){
        message msg{};
        memset(msg.buffer, 0, sizeof(msg.buffer));
        if (recv(socket_fd, &msg, sizeof(msg), 0) == 0) break;
        if (msg.type == 1){
            //rendering general chat
        }
        else if (msg.type == 2){
            //remember in buffer and rendering own chat
        }
        else if (msg.type == 3){ //update json state
            char buffer[4096];
            int bytes_received = recv(socket_fd, buffer, sizeof(buffer), 0);

            data = json::parse(std::string(buffer, bytes_received));

            std::ofstream file("output.json", std::ios::binary);
            file << data.dump(4);
            file.close();
        }
        cout << "---" << msg.buffer << endl;
    }
}
