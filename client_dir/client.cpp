
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <fcntl.h> 
#include <sys/epoll.h>
#include <map>
#include <string>
#include <arpa/inet.h>
#include <thread>
#include <nlohmann/json.hpp> 
#include <fstream>

using std::cout;  
using std::endl;
using std::cin;
using json = nlohmann::json;


class Window{

};

class Client {
    private:
        int id;
        std::string name;
        int socket_fd;
        json data;

        struct message {
            int type; // 0-rename; 1-full; 2-protected
            char buffer[1024];
            int occused; // откого
            int adress; // optional
        };

        void settings(int port, std::string host){
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
    }
    
    public:
        Client(std::string name, int port, std::string host): name(name) {settings(port, host);}
        ~Client(){
            if (socket_fd != -1) {
                close(socket_fd);
            }
        }

        void listen(){
            while (true){
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

        void writing(){
            cin.ignore();
            while (true){
                message msg{};
                cin.getline(msg.buffer, sizeof(msg.buffer));
                msg.type = 1;
                if (send(socket_fd, &msg, sizeof(msg), 0) == 0) break;
            }
        }
};


void listen_th(Client& client){
    client.listen();
}

void writing_th(Client& client){
    client.writing();
}

int main (){
    std::string name;
    cin >> name;

    Client client(name, 8080, "127.0.0.1");
    std::thread l_thread(listen_th, std::ref(client));
    std::thread w_thread(writing_th, std::ref(client));

    l_thread.join();
    w_thread.join();

    {
        std::ofstream("output.json", std::ios::trunc).close();
    }

    return 0;
}