cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED True)

project(UCLOUD_SERVER)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)

add_executable(server server.cpp)


target_link_libraries(server PRIVATE sfml-system sfml-network sfml-graphics sfml-window ImGui-SFML::ImGui-SFML)


