cmake_minimum_required(VERSION 3.10)
project(MyProject)

# Настройка автоматической системы Qt (MOC, UIC, RCC)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Установка стандарта C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Поиск необходимых компонентов
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Network)
find_package(SQLite3 REQUIRED)  # Добавлен поиск SQLite

# Установка выходной директории
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# Клиентское приложение (с GUI)
add_executable(client 
    client_dir/client.cpp 
    client_dir/client_func.cpp
    client_dir/window_func.cpp
)

# Серверное приложение (без GUI)
add_executable(server 
    server_dir/server.cpp 
    server_dir/server_func.cpp
    database_dir/database.cpp  # Добавлен файл базы данных
)

# Подключение библиотек для клиента
target_link_libraries(client PRIVATE
    Qt5::Core
    Qt5::Widgets
    Qt5::Network
)

# Подключение библиотек для сервера
target_link_libraries(server PRIVATE
    Qt5::Core
    Qt5::Network
    SQLite::SQLite3  # Добавлена SQLite библиотека
)

# Для Windows дополнительно подключаем Winsock
if (WIN32)
    target_link_libraries(client PRIVATE ws2_32)
    target_link_libraries(server PRIVATE ws2_32)
endif()

# Дополнительные настройки для Qt
target_compile_definitions(client PRIVATE
    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)

# Добавляем пути к заголовочным файлам
include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Network_INCLUDE_DIRS}
    ${SQLite3_INCLUDE_DIRS}  # Добавлены пути SQLite
)