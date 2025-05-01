cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED True)

project(UCLOUD_CLIENT)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/inc)


add_executable(client 
    client.cpp
    tinyfiledialogs.c
)
if(WIN32)
    target_link_libraries(client PRIVATE comdlg32 ole32)
endif()


target_link_libraries(client PRIVATE sfml-system sfml-network sfml-graphics sfml-window ImGui-SFML::ImGui-SFML)


