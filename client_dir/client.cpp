#include "client.h"

std::mutex Client::file_mutex;

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

    bool isAdmin = false;
    QString name;
    {
        QDialog dialog;
        dialog.setWindowTitle("Вход");

        QVBoxLayout layout(&dialog);
        QLabel nameLabel("Введите ваше имя:");
        QLineEdit nameEdit;
        layout.addWidget(&nameLabel);
        layout.addWidget(&nameEdit);
        QCheckBox adminCheckBox("Я администратор");
        layout.addWidget(&adminCheckBox);
        QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        layout.addWidget(&buttons);

        QObject::connect(&buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
        QObject::connect(&buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

        if (dialog.exec() == QDialog::Accepted) {
            name = nameEdit.text();
            if (!name.isEmpty()) {
                isAdmin = adminCheckBox.isChecked(); 
            }
        }
    }
    
    Client* client = nullptr;
    if (isAdmin)
        client = new ClientAdmin(name.toStdString(), 8080, "127.0.0.1");
    else 
        client = new Client(name.toStdString(), 8080, "127.0.0.1");

    std::thread listen_thread([client]() { client->listen(); });

    //-------------------------------//
    
    VKStyleWindow window(client);

    cout << 666;

    QObject::connect(client, &Client::check_chat, 
        &window, &VKStyleWindow::handle_check_chat);
    QObject::connect(client, &Client::close_window, 
        &window, &VKStyleWindow::handle_close_window);

    window.show();
    
    int result = app.exec();

    
    std::ofstream("output.json", std::ios::trunc).close();

    listen_thread.detach();

    delete client;
    return result;
}

#include "moc_client.cpp"