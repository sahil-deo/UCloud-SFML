#include <iostream>

#include <SFML/Network.hpp>
#include <SFML/System.hpp>

#include <thread>
#include <vector>
#include <fstream>
#include <filesystem>

void startClient();

void sendData(std::shared_ptr<sf::TcpSocket> socket);
void sendFile(std::shared_ptr<sf::TcpSocket> socket);
void sendFolder(std::shared_ptr<sf::TcpSocket> socket);

// Helper Function
std::string getExtention(std::string filePath);

int main()
{
    std::cout << "Welcome to UCloud Client!\n";
    std::cout << "--------------------------\n";
    std::cout << "Client is starting\n";

    startClient();
    
}

void startClient()
{
    sf::IpAddress ip;
    std::cout << "Enter Server IP Address: ";
    std::cin >> ip;

    auto socket = std::make_shared<sf::TcpSocket>();
    sf::Socket::Status status = socket->connect(ip, 55555);
    if (status != sf::Socket::Done)
    {
        std::cout << "Cannot Connect to " << ip << "\n";
        return;
    }

    std::thread sendThread(sendData, socket);

    sendThread.join();
}

void sendData(std::shared_ptr<sf::TcpSocket> socket)
{
    try
    {

        while (true)
        {
            
            std::string dataType;
            std::cout << "Send Data: \n";
            std::cout << "1. File\n";
            std::cout << "2. Folder\n";
            std::getline(std::cin, dataType);
            std::cin.clear();
            std::cin.sync();
            if (dataType == "1")
            {
                sendFile(socket);
            }
            else if (dataType == "2")
            {
                sendFolder(socket);
            }
            else {
                system("CLS");
            }
        }
    }
    catch (std::exception e)
    {
        std::cout << e.what() << "\n";
        std::cout << "Disconnected from the server\n";
        socket->disconnect();
    }
    catch (...)
    {
        std::cout << "Disconnected from the server\n";
        socket->disconnect();
    }
}

void sendFile(std::shared_ptr<sf::TcpSocket> socket)
{
    std::string data = "";
    std::string dataLen = "";
    std::string filePath = "";
    std::string extention = "";

    std::cin.clear();
    std::cin.sync();

    std::cout << "Enter File Path: ";
    std::getline(std::cin, filePath);

    std::ifstream file(filePath, std::ios_base::binary | std::ios_base::ate);

    if (!file)
    {
        std::cout << "Invalid File Path\n";
        return;
    }

    std::streamsize streamSize = file.tellg();
    std::vector<char> buffer(streamSize);

    file.close();

    file.open(filePath, std::ios_base::binary);

    file.read(buffer.data(), streamSize);

    extention = getExtention(filePath);
    dataLen = std::to_string(extention.length());
    dataLen.append(std::to_string(buffer.size()));

    for (char c : buffer)
    {
        data.push_back(c);
    }

    if (socket->send(dataLen.c_str(), dataLen.length()) != sf::Socket::Done)
    {
        throw("Unable To Send Meta Data");
    }

    if (socket->send(extention.c_str(), extention.length()) != sf::Socket::Done)
    {
        throw("Unable To Send Meta Data");
    }

    if (socket->send(data.c_str(), data.length()) != sf::Socket::Done)
    {
        throw("Unable To Send Data");
    }

    std::cout << "\nSending Data...\nDo Not Close This Window...\n";

    char ack[3];
    std::size_t recv = 0;

    if (socket->receive(ack, 3, recv) != sf::Socket::Done)
    {
        throw("Unable To Acknowdge");
    }
    std::cout << "Data Sent!\n\n";
}

void sendFolder(std::shared_ptr<sf::TcpSocket> socket)
{
    std::cin.clear();
    std::cin.sync();
    std::string folderPath;
    std::cout << "Enter Folder Path: ";
    std::getline(std::cin, folderPath);
    std::filesystem::path FolderPath(folderPath);

    std::vector<std::filesystem::path> filePaths;
    for (auto file : std::filesystem::directory_iterator(FolderPath))
    {
        if (!file.is_directory())
            filePaths.emplace_back(file);
    }

    for (auto file : filePaths)
    {
        std::string filePath = file.string();
        std::string data = "";
        std::string dataLen = "";
        std::string extention = "";

        std::cin.clear();
        std::cin.sync();

        std::ifstream file(filePath, std::ios_base::binary | std::ios_base::ate);

        if (!file)
        {
            std::cout << "Invalid File Path\n";
            return;
        }

        std::streamsize streamSize = file.tellg();
        std::vector<char> buffer(streamSize);

        file.close();

        file.open(filePath, std::ios_base::binary);

        file.read(buffer.data(), streamSize);

        extention = getExtention(filePath);
        dataLen = std::to_string(extention.length());
        dataLen.append(std::to_string(buffer.size()));

        for (char c : buffer)
        {
            data.push_back(c);
        }

        if (socket->send(dataLen.c_str(), dataLen.length()) != sf::Socket::Done)
        {
            throw("Unable To Send Meta Data");
        }

        if (socket->send(extention.c_str(), extention.length()) != sf::Socket::Done)
        {
            throw("Unable To Send Meta Data");
        }

        if (socket->send(data.c_str(), data.length()) != sf::Socket::Done)
        {
            throw("Unable To Send Data");
        }

        std::cout << "\nSending Data...\nDo Not Close This Window...\n";

        char ack[3];
        std::size_t recv = 0;

        if (socket->receive(ack, 3, recv) != sf::Socket::Done)
        {
            throw("Unable To Acknowdge");
        }
        std::cout << "Data Send!\n";
    }
}

std::string getExtention(std::string filePath)
{
    int len = filePath.length();
    int index = -1;
    std::string extention = "";

    for (int i = len - 1; i >= 0; i--)
    {
        if (filePath[i] == '.')
        {
            index = i;
        }
    }
    if (index == -1)
    {
        return " ";
    }

    extention;
    extention = filePath.substr(index + 1, filePath.length() - index);

    return extention;
}
