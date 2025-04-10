#include "client.h"

void listen_th(Client& client){
    client.listen();
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    qRegisterMetaType<std::string>("std::string");
    
    {
        std::ofstream file("output.json");
        file << "[]";
    }

    bool ok;
    QString name = QInputDialog::getText(nullptr, "Вход", "Введите ваше имя:",
                                        QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty()) {
        return 0;
    }
    
    Client client(name.toStdString(), 8080, "127.0.0.1");

    std::thread listen_thread([&client]() { client.listen(); });
    
    VKStyleWindow window;

    QObject::connect(&client, &Client::check_chat, 
        &window, &VKStyleWindow::handle_check_chat);

    window.setClient(&client);
    window.show();
    
    int result = app.exec();
    
    std::ofstream("output.json", std::ios::trunc).close();

    listen_thread.detach();

    return result;
}

#include "moc_client.cpp"