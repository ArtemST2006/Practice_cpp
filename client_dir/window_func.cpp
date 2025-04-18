#include "client.h"

void VKStyleWindow::setupUI(){
    cout << "setupUI" << endl;
    // Главный виджет и layout
    QWidget* centralWidget = new QWidget(this);
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);

    // Панель контактов (левая часть)
    QWidget* contactsPanel = new QWidget;
    QVBoxLayout* contactsLayout = new QVBoxLayout(contactsPanel);
    contactsList = new QListWidget;
    contactsList->setStyleSheet(
        "QListWidget {"
        "   font-size: 14px;"
        "   border: none;"
        "}"
        "QListWidget::item {"
        "   padding: 10px;"
        "   border-bottom: 1px solid #e7e8ec;"
        "}"
    );
    contactsLayout->addWidget(contactsList);

    // Панель чата (правая часть)
    QWidget* chatPanel = new QWidget;
    QVBoxLayout* chatLayout = new QVBoxLayout(chatPanel);
    
    chatHistory = new QListWidget;
    chatHistory->setStyleSheet(
        "QListWidget {"
        "   font-size: 14px;"
        "   background-color: #f5f6f8;"
        "   border: none;"
        "}"
        "QListWidget::item {"
        "   padding: 10px;"
        "   border-bottom: 1px solid #e7e8ec;"
        "}"
    );
    
    messageInput = new QLineEdit;
    messageInput->setPlaceholderText("Введите сообщение...");
    
    sendButton = new QPushButton("Отправить");
    sendButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #5181b8;"
        "   color: white;"
        "   border: none;"
        "   padding: 8px 16px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #5a8ac7;"
        "}"
    );
    
    QHBoxLayout* inputLayout = new QHBoxLayout;
    inputLayout->addWidget(messageInput);
    inputLayout->addWidget(sendButton);
    
    chatLayout->addWidget(chatHistory);
    chatLayout->addLayout(inputLayout);

    // Добавляем панели в главный layout
    mainLayout->addWidget(contactsPanel, 1);
    mainLayout->addWidget(chatPanel, 3);
    
    setCentralWidget(centralWidget);
    resize(800, 600);
    setWindowTitle(QString::fromStdString(client->get_name()));

    contactsList->setStyleSheet("QListView::item { border: none; }");

    cout << "setupUI end" << endl;
}

void VKStyleWindow::setupConnections(){
    cout << "setupConnections" << endl;
    updateTimer = new QTimer(this);
        connect(updateTimer, &QTimer::timeout, this, &VKStyleWindow::updateChats);
        updateTimer->start(2000); // Обновление каждые 2 секунды
        
        connect(sendButton, &QPushButton::clicked, this, &VKStyleWindow::sendMessage);
        connect(messageInput, &QLineEdit::returnPressed, this, &VKStyleWindow::sendMessage);
        connect(contactsList, &QListWidget::itemClicked, this, &VKStyleWindow::selectChat);

        cout << "setupConnections end" << endl;
}

void VKStyleWindow::loadContacts(){
    cout << "loadContacts" << endl; 
    if (!client) {
        cout << "Ошибка Клиент не инициализирован" << endl;
    }
    try {
        json data_list = client->give_data(3, client->get_id(), -1); //запрос json клиентов
        updateContactsList(data_list);
        
    } catch (...) {
        QMessageBox::critical(this, "Ошибка", "Не удалось загрузить контакты");
    }
    cout << "loadContacts end " << endl;
}

void VKStyleWindow::updateContactsList(const json& data){ 
    cout << "updateContactList" << endl;
    contactsList->clear();
    QListWidgetItem* general = new QListWidgetItem(QString::fromStdString("general"), contactsList);
    general->setData(Qt::UserRole, 1);
        cout << "111" << endl;//mist
        if (data.is_array()) {
            for (const auto& contact : data) {
                int id = contact["id"];
                std::string name = contact["name"];
                
                if (id != client->get_id()){
                    QListWidgetItem* item = new QListWidgetItem(
                        QString::fromStdString(name), 
                        contactsList
                    );
                    
                    if (id == current_id){
                        item->setBackground(QColor(10, 220, 255));
                    }
                    item->setData(Qt::UserRole, id);
                }
            }
        }
        cout << "updateContactList end" << endl;
}

void VKStyleWindow::selectChat(QListWidgetItem* item){
    int chatId = item->data(Qt::UserRole).toInt();
    item->setBackground(QColor(200, 220, 255));
    current_id = chatId;

    if (client->get_admin_flag() && current_id != 1){
        QMessageBox adminDialog;
        adminDialog.setWindowTitle("действия");
    
        QPushButton *openChatBtn = adminDialog.addButton("Открыть чат", QMessageBox::ActionRole);
        QPushButton *deleteUserBtn = adminDialog.addButton("Удалить пользователя", QMessageBox::ActionRole);

        adminDialog.exec();
        
        if (adminDialog.clickedButton() == deleteUserBtn) {
            client->delete_user(current_id);
            return;
        }
    }

    json current_chat = client->give_data(4, client->get_id(), current_id);
    
    chatHistory->clear();
    chatHistory->addItem("Выбран чат: " + item->text());
    displayMessages(current_chat);
}

void VKStyleWindow::sendMessage() {
    std::string str = messageInput->text().toStdString();
    if (!str.empty() && current_id != -1){
        if (current_id == 1){ // общий чат
            client->send_message(str, current_id, 1);
            add_one_message(str);
        }
        else{
            client->send_message(str, current_id, 2);
            add_one_message(str);
        }
        messageInput->clear();
    }
}

void VKStyleWindow::displayMessages(const json& messages) {
    chatHistory->clear();
    
    if (messages.is_array()) {
        for (auto it = messages.rbegin(); it != messages.rend(); ++it) {
            const auto& msg = *it;
            int senderId = msg["sender_id"];
            std::string text = msg["text"];
            //std::string time = msg["time"];
            
            QString messageText = QString::fromStdString(text);
            QListWidgetItem* item = new QListWidgetItem(messageText, chatHistory);
            
            if (senderId == client->get_id()) {
                item->setBackground(QColor(240, 240, 240));
                item->setTextAlignment(Qt::AlignRight);
            } else {
                item->setBackground(QColor(200, 220, 255));
                item->setTextAlignment(Qt::AlignLeft);
            }
            item->setSizeHint(QSize(chatHistory->width(), 50));
            item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
        }
    }
    
    chatHistory->setUpdatesEnabled(true);
    chatHistory->update();
    chatHistory->repaint();
    chatHistory->scrollToBottom();
}

void VKStyleWindow::handle_check_chat(int add, std::string buffer){
    cout << buffer;
    if (add == current_id){
        cout << buffer;
        add_one_message(buffer, 1);
    }
    // добавление значка непрочитанных соо
}

void VKStyleWindow::add_one_message(std::string text, int mode){
    QString messageText = QString::fromStdString(text);
    QListWidgetItem* item = new QListWidgetItem(messageText, chatHistory);
    if (mode == 0)
        item->setTextAlignment(Qt::AlignRight);
    else if (mode == 1)
        item->setTextAlignment(Qt::AlignLeft);
    item->setSizeHint(QSize(chatHistory->width(), 50));
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);

    chatHistory->setUpdatesEnabled(true);
    chatHistory->update();
    chatHistory->repaint();
    chatHistory->scrollToBottom();
}