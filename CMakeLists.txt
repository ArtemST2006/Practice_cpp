cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(client client_dir/client.cpp client_dir/client_func.cpp client_dir/coding/task.cpp)

add_executable(server server_dir/server.cpp server_dir/server_func.cpp)


if (WIN32)
    target_link_libraries(client ws2_32)
    target_link_libraries(server ws2_32)
endif()
