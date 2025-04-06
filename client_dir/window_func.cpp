#include "client.h"

void VKStyleWindow::setupUI(){
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
    setWindowTitle("VK Style Messenger");
}

void VKStyleWindow::setupConnections(){
    updateTimer = new QTimer(this);
        connect(updateTimer, &QTimer::timeout, this, &VKStyleWindow::updateChats);
        updateTimer->start(2000); // Обновление каждые 2 секунды
        
        connect(sendButton, &QPushButton::clicked, this, &VKStyleWindow::sendMessage);
        // connect(messageInput, &QLineEdit::returnPressed, this, &VKStyleWindow::sendMessage);
        connect(contactsList, &QListWidget::itemClicked, this, &VKStyleWindow::selectChat);
}

void VKStyleWindow::loadContacts(){
    try {
        std::ifstream file("output.json");
        if (file) {
            json data = json::parse(file);
            updateContactsList(data);
        }
    } catch (...) {
        QMessageBox::critical(this, "Ошибка", "Не удалось загрузить контакты");
    }
}

void VKStyleWindow::updateContactsList(const json& data){
    contactsList->clear();
        
        if (data.is_array()) {
            for (const auto& contact : data) {
                int id = contact["id"];
                std::string name = contact["name"];
                
                if (id != client->get_id()){
                    QListWidgetItem* item = new QListWidgetItem(
                        QString::fromStdString(name), 
                        contactsList
                    );
                    item->setData(Qt::UserRole, id);
                }
            }
        }
}

void VKStyleWindow::selectChat(QListWidgetItem* item){
    int chatId = item->data(Qt::UserRole).toInt();
    current_id = chatId;
    // Здесь можно загрузить историю чата по chatId
    chatHistory->clear();
    chatHistory->addItem("Выбран чат: " + item->text());
}

void VKStyleWindow::sendMessage() {
    std::string str = messageInput->text().toStdString();
    if (!str.empty() && current_id != -1){
        client->send_message(str, current_id);
        messageInput->clear();
    }
}