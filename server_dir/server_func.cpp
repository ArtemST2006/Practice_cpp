#include "server.h"

void Server::set_socket_nonblocked(const int fd){
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void Server::settings(int port){
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
            
    int opt = 1;
    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
                
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port); 
    
    bind(socket_fd, (sockaddr*)&address, sizeof(address));
                
    listen(socket_fd, 1000);
    set_socket_nonblocked(socket_fd);
              
    cout << "Server listening on port " << port << endl; 
}

void Server::handle_events(int epoll_fd, epoll_event* events, int ndfs){
    for (int i = 0; i < ndfs; ++i) {
        int fd = events[i].data.fd;
        if (events[i].events & EPOLLHUP) {
            cout << "Disconnected " << fd << endl;
            lis.erase(fd);
            json_erase(fd); send_json();
            close(fd);
        } else if (events[i].events & EPOLLIN) {
            message msg{};
            ssize_t count;
            while ((count = read(fd, &msg, sizeof(msg))) > 0){
                if (msg.type == 0){ //прислали имя
                    append_json(fd, msg.buffer); send_json();
                    lis[fd] = msg.buffer;
                    storage.add_user(lis[fd]); //add to database
                }
                else if (msg.type == 1){
                    message msg_new;
                    msg_new.type = msg.type;  
                    strcpy(msg_new.buffer, msg.buffer);  
                    msg_new.occused = fd;         
                    msg_new.adress = msg.adress; 
                    for (auto [fdms, name] : lis){
                        if (fdms != fd)
                            send(fdms, &msg_new, sizeof(msg_new), 0);
                    }
                }
                else if (msg.type == 2){
                    //send on msg.address
                }
                cout << msg.buffer << endl;
                
            }
            if (count == -1 && errno != EAGAIN){
                break;
            }
            else if(count == 0){
                cout << "Disconnected " << fd << endl;
                lis.erase(fd);
                json_erase(fd); send_json();
                close(fd);
            }
        }
    }
}


void Server::communication(){
    int epoll_fd = epoll_create1(0);
    
    epoll_event event;
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = socket_fd;
    
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &event);
    
    epoll_event events[100];
    while (1) {
        int ndfs = epoll_wait(epoll_fd, events, 100, -1);
    
        for (int i = 0; i < ndfs; ++i) {
            if (events[i].data.fd == socket_fd) {
                while(1) {
                    sockaddr_in client_addr;
                    socklen_t client_len = sizeof(client_addr);
                    const int client_fd = accept(socket_fd, (sockaddr*)&client_addr, &client_len);
                    if (client_fd == -1) {
                        if (errno == EAGAIN) {
                            break;
                        }
                    }
    
                    set_socket_nonblocked(client_fd);
                    event.events = EPOLLIN | EPOLLHUP | EPOLLET;
                    event.data.fd = client_fd;
    
                    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event);
                    cout << "Connected fd: " << client_fd << endl;
                    
                    { // обмен предварительными данными
                        int netid = htonl(client_fd);
                        send(client_fd, (char*)&netid, sizeof(netid), 0);
                        lis[client_fd] = "";
                    }
                }
            } else {
                handle_events(epoll_fd, events, ndfs);
            }
        }
    }
    close(epoll_fd);
}


void Server::send_json(){
    message msg{};
    msg.type = 3;

    std::string json_str = data.dump();

    for (auto [fd, name] : lis){
        send(fd, &msg, sizeof(msg), 0);
        send(fd, json_str.c_str(), json_str.size(), 0);
    }
}

void Server::append_json(int id, char* name){
    json obj;
    obj["id"] = id;
    obj["name"] = name;

    data.push_back(obj);
}


void Server::json_erase(int id){
    auto it = std::remove_if(data.begin(), data.end(), [id](const json& item) {
        return item["id"] == id;
    });
    data.erase(it, data.end());
}
