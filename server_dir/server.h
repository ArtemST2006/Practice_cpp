#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <fcntl.h> 
#include <sys/epoll.h>
#include <map>
#include <string>
#include <nlohmann/json.hpp> 
#include "../database_dir/database.h"

using std::cout;  
using std::endl;
using json = nlohmann::json;

class Server {
    private:
        struct message {
            int type; // 0-rename; 1-full; 2-protected; 3-update
            char buffer[1024];
            int occused; // откого
            int adress; // optional куда
        };
        Storage storage;
        int socket_fd;
        char buffer[1024];
        std::map<const int, std::string> lis; // fd(id), name
        json data;
    
        void set_socket_nonblocked(const int);
        void settings(int );
        void handle_events(const int, epoll_event*, int);
        void send_json();
        void json_erase(int );
        void append_json(int , char* );
    
    public:
        Server(const Server&) = delete;
        Server& operator=(const Server&) = delete;
        
        Server(int port, const string database_path) : storage(database_path){ data = json::array(); settings(port);} 
        
        ~Server() {
            if (socket_fd != -1) {
                close(socket_fd);
            }
        }
    
        void communication();
    };