#include "client.h"
#include <chrono>

void Client::settings(int port, std::string host){
    struct sockaddr_in serv_addr;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    inet_pton(AF_INET, host.c_str(), &serv_addr.sin_addr);
        
    ::connect(socket_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

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
        cout << msg.type << endl;
        if (msg.type == 1){
            std::string str(msg.buffer);
            check_chat(1, str);
        }
        else if (msg.type == 2){
            std::string str(msg.buffer);
            check_chat(msg.occused, str);
        }
        else if (msg.type == 3){ //update json state
            data = json::parse(std::string(msg.buffer, sizeof(msg.buffer)));
        }
        else if (msg.type == 4){
            current_chat = json::parse(std::string(msg.buffer, sizeof(msg.buffer)));
        }
        else if(msg.type == 5){
            close_window();
        }
    }
}

void Client::send_message(std::string str, int id_add, int type){
    message msg {};
    msg.type = type; 
    strcpy(msg.buffer, str.c_str());
    msg.occused = id; // можно понять имя отправителя(для общего чата)
    msg.adress = id_add;
    send(socket_fd, &msg, sizeof(msg), 0);
}

json Client::give_data(int type, int sender = -1, int address = -1){
    cout << "order" << type << endl;
    message msg {};
    msg.type = type;
    msg.adress = address;
    msg.occused = sender;
    if (type == 3){ // обновление списка клиентов
        send(socket_fd, &msg, sizeof(msg), 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(200)); 
        return data;
    } 
    else if (type == 4){ // запрос 
        send(socket_fd, &msg, sizeof(msg), 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(200)); 
        return current_chat;
    }
}

void ClientAdmin::delete_user(int id){
    message msg{};
    msg.type = 5;
    msg.adress = id;
    send(socket_fd, &msg, sizeof(msg), 0);
}
