#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "GUI/TextBox.h"
#include "GUI/Button.h"
#include "GUI/GuiHandler.h"

#include <iostream>
#include <thread>
#include <vector>
#include <fstream>
#include <filesystem>


///Networking Components
#pragma region


///Components
std::shared_ptr<sf::TcpSocket> clientSocket;
sf::Socket::Status status;
std::string _status;

///Threads
std::thread connectThread;
std::thread sendThread;

///Functions
void connectSocket(std::string ip);

void sendData(std::shared_ptr<sf::TcpSocket> socket);

void sendFile(std::shared_ptr<sf::TcpSocket> socket, std::string filePath);
void sendFolder(std::shared_ptr<sf::TcpSocket> socket, std::string folderPath);

std::string getExtention(std::string filePath);

#pragma endregion


///GUI Components
#pragma region 

Button _connectButton("Connect");
Button _sendButton("Send");
TextBox _ipBox;
TextBox _filePath;
TextBox _statusText;
GuiHandler _handler;

#pragma endregion


int main() {
	
	sf::RenderWindow window(sf::VideoMode(800, 800), "NewWindow", sf::Style::Close | sf::Style::Titlebar);    
    sf::Event event;


    ///GUI Components
    #pragma region 
        _handler.addToHandler(&_connectButton);
        _handler.addToHandler(&_sendButton);
        _handler.addToHandler(&_ipBox);
        _handler.addToHandler(&_filePath);
        _handler.addToHandler(&_statusText);
    #pragma endregion
    
    ///Styling
    {
        _ipBox.setPosition(10, 10);
        _ipBox.setBoxSize(780, 24);
        _ipBox.setPadding(4, 0);

        _connectButton.setPosition(10, 100);
        _connectButton.setBoxSize(780, 24);
        _connectButton.setPadding(4, 0);

        _filePath.setPosition(10, 200);
        _filePath.setBoxSize(780, 24);
        _filePath.setPadding(4, 0);

        _sendButton.setPosition(10, 300);
        _sendButton.setBoxSize(780, 24);
        _sendButton.setPadding(4, 0);

        _statusText.setString("Status: ");
        _statusText.setPosition(10, 400);
        _statusText.setBoxSize(780, 24);
        _statusText.setPadding(4, 0);
    }

    


    while (window.isOpen())
    {

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {

                window.close();
            }

            _handler.HandleGuiEvent(&window, &event);
        }

        if (_connectButton.isReleased()) {
            connectSocket(_ipBox.getString());
            _connectButton.setEnabled(false);
        }

        if (_status == "Disconnected") {
            _connectButton.setEnabled(true);
        }

        _statusText.setString("Status: " + _status);

        window.clear(sf::Color::White);

        _handler.HandleGuiRender(&window);

		window.display();
	}


    if (sendThread.joinable()) {
        sendThread.join();
    }

}

void connectSocket(std::string ip) {
    clientSocket = std::make_shared<sf::TcpSocket>();
    status = clientSocket->connect(ip, 55555);
    
    if (status != sf::Socket::Done) {
        //Implement Error
        _connectButton.setEnabled(true);
        _status = "Connection Error, Try again";
        clientSocket->disconnect();

    }
    else {
        _status = "Connected";
        sendThread = std::thread(sendData, clientSocket);
    }
}


void sendData(std::shared_ptr<sf::TcpSocket> socket)
{
    try
    {

        while (true)
        {
            std::string path = "";

            /*
            std::string dataType = "1";
            //std::cout << "Send Data: \n";
            //std::cout << "1. File\n";
            //std::cout << "2. Folder\n";
            //std::getline(std::cin, dataType);           

            std::cin.clear();
            std::cin.sync();
            if (dataType == "1")
            {
                sendFile(socket);
            }
            else if (dataType == "2")
            {
                //sendFolder(socket);
            }
            else {
                system("CLS");
            }
            */

            if (_sendButton.isReleased() == true) {
                path = _filePath.getString();

                std::filesystem::path checkPath(path);

                if (std::filesystem::is_directory(checkPath)) {

                    sendFolder(socket, path);
                }
                else {
                    sendFile(socket, path);
                }
            }

                
        }
    }
    catch (std::exception e)
    {
        std::cout << e.what() << "\n";
        std::cout << "Disconnected from the server\n";
        _status = "Disconnected";

        socket->disconnect();
    }
    catch (...)
    {
        _status = "Disconnected";
        std::cout << "Disconnected from the server\n";
        socket->disconnect();
    }
}



void sendFile(std::shared_ptr<sf::TcpSocket> socket, std::string filePath)
{
    std::string data = "";
    std::string dataLen = "";
    std::string extention = "";

    std::cin.clear();
    std::cin.sync();

//    std::cout << "Enter File Path: ";
//    std::getline(std::cin, filePath);
    /*
    while (true) {

        if (_sendButton.isReleased() == true) {
            filePath = _filePath.getString();

            std::filesystem::path checkPath(filePath);
           
            break;
        }
        else {
            continue;
        }
    }
    */

    std::ifstream file(filePath, std::ios_base::binary | std::ios_base::ate);

    if (!file)
    {
        std::cout << "Invalid File Path\n";
        return;
    }
    _status = "Reading Data";

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

    _status = "Sending Data";

    char ack[3];
    std::size_t recv = 0;

    if (socket->send(dataLen.c_str(), dataLen.length()) != sf::Socket::Done)
    {
        throw("Unable To Send Meta Data");
    }
    if (socket->receive(ack, 3, recv) != sf::Socket::Done)
    {
        _status = "Unable To Acknowdge";
        throw("Unable To Acknowdge");
    }
    if (socket->send(extention.c_str(), extention.length()) != sf::Socket::Done)
    {
        throw("Unable To Send Meta Data");
    }
    if (socket->receive(ack, 3, recv) != sf::Socket::Done)
    {
        _status = "Unable To Acknowdge";
        throw("Unable To Acknowdge");
    }
    if (socket->send(data.c_str(), data.length()) != sf::Socket::Done)
    {
        throw("Unable To Send Data");
    }

   std::cout << "\nSending Data...\nDo Not Close This Window...\n";
    _status = "Sending Data. Do not close this window";


    if (socket->receive(ack, 3, recv) != sf::Socket::Done)
    {
        _status = "Unable To Acknowdge";
        throw("Unable To Acknowdge");
    }
    _status = "Data Sent";
    std::cout << "Data Sent!\n\n";
}



void sendFolder(std::shared_ptr<sf::TcpSocket> socket, std::string folderPath)
{

    std::vector<std::filesystem::path> filePaths;
    for (auto file : std::filesystem::directory_iterator(folderPath))
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

        char ack[3];
        std::size_t recv = 0;

        if (socket->send(dataLen.c_str(), dataLen.length()) != sf::Socket::Done)
        {
            throw("Unable To Send Meta Data");
        }

        if (socket->receive(ack, 3, recv) != sf::Socket::Done)
        {
            _status = "Unable To Acknowdge";
            throw("Unable To Acknowdge");
        }

        if (socket->send(extention.c_str(), extention.length()) != sf::Socket::Done)
        {
            throw("Unable To Send Meta Data");
        }

        if (socket->receive(ack, 3, recv) != sf::Socket::Done)
        {
            _status = "Unable To Acknowdge";
            throw("Unable To Acknowdge");
        }

        if (socket->send(data.c_str(), data.length()) != sf::Socket::Done)
        {
            throw("Unable To Send Data");
        }

        std::cout << "\nSending Data...\nDo Not Close This Window...\n";


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


//C:\Users\shree\Videos\Bandi Opts\bandicam 2024-07-06 15-53-31-470.mp4