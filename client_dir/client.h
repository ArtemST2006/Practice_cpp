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
#include <arpa/inet.h>
#include <thread>
#include <nlohmann/json.hpp> 
#include <fstream>
#include "../crypto/crypto.h"

#include <QApplication>
#include <QMainWindow>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTimer>
#include <QMessageBox>
#include <QInputDialog>
#include <QString>
#include <QMetaType>
#include <QObject>

using std::cout;  
using std::endl;
using std::cin;
using json = nlohmann::json;


class Client : public QObject{
    Q_OBJECT

    private:
        int id;
        std::string name;
        int socket_fd;
        json data;
        json current_chat;
        bool connected = false;

        struct message {
            int type; // 0-rename; 1-full; 2-protected, 3 - add/del /, 4 - chat
            char buffer[1024];
            int occused; // откого
            int adress; // optional
        };

        void settings(int port, std::string host);
    
    public:
        Client(std::string name, int port, std::string host): name(name) {settings(port, host);}
        ~Client(){
            if (socket_fd != -1) {
                close(socket_fd);
            }
        }

        void listen();
        void send_message(std::string, int, int);
        inline int get_id() {return id;}
        json& give_data(int, int, int);
        inline void close_connetion() { close(socket_fd); connected = false; }

    signals:
        void check_chat(int, std::string);
};



class VKStyleWindow : public QMainWindow {
    Q_OBJECT
public:
    VKStyleWindow(QWidget* parent = nullptr) : QMainWindow(parent) {
        setupUI();
        setupConnections();
        loadContacts();
    }
    void setClient(Client* cl){
        client = cl;
    }

protected:
    void closeEvent(QCloseEvent *event) override {
        if (client) {
            client->close_connetion();
        }
        if (updateTimer) {
            updateTimer->stop();
        }
        QApplication::quit();
    }

private:
    QListWidget* contactsList;
    QListWidget* chatHistory;
    QLineEdit* messageInput;
    QPushButton* sendButton;
    QTimer* updateTimer;
    Client* client;
    int current_id = -1;

    void setupUI();

    void setupConnections();

    void loadContacts();

    void updateContactsList(const json& data);

    void displayMessages(const json&);
    void add_one_message(std::string str, int mode=0);
    
public slots:
    void handle_check_chat(int, std::string);

private slots:
    void updateChats() {
        loadContacts();
    }

    void selectChat(QListWidgetItem* item);
    void sendMessage();

    // void sendMessage();
};
