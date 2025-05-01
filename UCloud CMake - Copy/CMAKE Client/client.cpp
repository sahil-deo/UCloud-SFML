#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "tinyfiledialogs.h"
#include <imgui.h>
#include <imgui-SFML.h>

#include <iostream>
#include <thread>
#include <vector>
#include <filesystem>
#include <fstream>

/// Networking Components
#pragma region

std::shared_ptr<sf::TcpSocket> clientSocket;

/// Components
sf::Socket::Status status;
std::string _status;
bool isWindowClosed;

/// Threads
std::thread connectThread;
std::thread sendThread;

/// Functions
void connectSocket(std::string ip);

void sendData(std::shared_ptr<sf::TcpSocket> socket);

void sendFile(std::shared_ptr<sf::TcpSocket> socket, std::string filePath);

void sendFolder(std::shared_ptr<sf::TcpSocket> socket, std::string folderPath);

std::string getExtention(std::string filePath);

std::string removeQuotes(std::string filePath);

#pragma endregion

/// Buffer for IMGUI TextBox
#pragma region

static char ipBuffer[512] = "";
static char passBuffer[512] = "";
static char pathBuffer[512] = "";
static std::string pathBufferString;

#pragma endregion

/// Bool for IMGUI
#pragma region

// Before Connection
bool isConnected = false;
bool isConnectButton = false;

// After Connection
bool isSendButton = false;
bool isSend = false;

bool isDisconnectButton = false;
bool isDisconnected = false;

bool isRecursive;

#pragma endregion

/// IMGUI Components
std::string imStatus;

int main()
{

    sf::RenderWindow window(sf::VideoMode(800, 700), "UCloud", sf::Style::Close | sf::Style::Titlebar);
    sf::Event event;

    isWindowClosed = false;

    ImGui::SFML::Init(window);
    sf::Clock deltaClock;

    imStatus = "Not Connected";

    ImVec2 oldWindowSize = {0, 0};

    // Main Loop
    while (window.isOpen())
    {

        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
            {

                window.close();
            }

            // Call GUI Event Handler to Handle Events
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("UCloud", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::GetIO().FontGlobalScale = 2.0f;

        float guiHeight = ImGui::GetWindowHeight();
        float guiWidth = ImGui::GetWindowWidth();

        if (guiWidth != (oldWindowSize.x) || guiHeight != (oldWindowSize.y))
        {
            oldWindowSize.x = guiWidth;
            oldWindowSize.y = guiHeight;

            sf::Vector2u temp;
            temp.x = guiWidth + 20;
            temp.y = guiHeight + 20;

            window.setSize({temp.x, temp.y});
        }

        if (!isConnected)
        {
            ImGui::Text("Connect to Server");
            ImGui::InputText("Server Address", ipBuffer, 512);
            ImGui::InputText("Server Password", passBuffer, 512, ImGuiInputTextFlags_Password); //
            if (ImGui::Button("Connect"))
            {
                isConnectButton = true;

                std::cout << ipBuffer << "\n";
                connectSocket(ipBuffer);

                imStatus = _status;
            }
            std::string tempstring = "Status: ";
            tempstring.append(imStatus);
            ImGui::Text(tempstring.c_str());
        }
        else if (isConnected)
        { /// if connected

            ImGui::Text("Send Files");
            ImGui::InputText("Path", pathBuffer, 512);
            ImGui::Checkbox("Recursive", &isRecursive);
            if (ImGui::IsItemHovered())
            {
                ImGui::SetTooltip("Check to send the folder recursively");
            }
            if (ImGui::Button("Choose File"))
            {
                const char *filePath = tinyfd_openFileDialog(
                    "Select a File", // Dialog title
                    "",              // Default path
                    0,               // No filter count
                    nullptr,         // No filter patterns
                    nullptr,         // No filter description
                    0                // Don't allow multi-select
                );
                if (filePath)
                {
                    std::strncpy(pathBuffer, filePath, sizeof(pathBuffer) - 1); // copy safely
                    pathBuffer[sizeof(pathBuffer) - 1] = '\0';                  // ensure null termination
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Choose Folder"))
            {
                const char *folderPath = tinyfd_selectFolderDialog(
                    "Select a Folder", // Dialog title
                    ""                 // Default path (empty = current directory)
                );
                if (folderPath)
                {
                    std::strncpy(pathBuffer, folderPath, sizeof(pathBuffer) - 1); // copy safely
                    pathBuffer[sizeof(pathBuffer) - 1] = '\0';                    // ensure null termination
                }
            }

            if (ImGui::Button("Send"))
            {
                isSendButton = true;
            }
            if (ImGui::Button("Disconnect"))
            {
                isDisconnectButton = true;
            }

            std::string tempStatus = "Status: ";
            tempStatus.append(imStatus);

            ImGui::Text(tempStatus.c_str());
        }

        ImGui::End();

        window.clear(sf::Color::Black);

        ImGui::SFML::Render(window);

        window.display();
    }

    isWindowClosed = true;

    if (sendThread.joinable())
    {
        sendThread.join();
    }

    ImGui::SFML::Shutdown();
}

void connectSocket(std::string ip)
{

    if (clientSocket)
    {
        clientSocket->disconnect();
        clientSocket.reset(); // Free the old socket
    }

    if (sendThread.joinable())
    {
        sendThread.join(); // Ensure the previous thread is finished
    }
    clientSocket = std::make_shared<sf::TcpSocket>();
    status = clientSocket->connect(ip, 55555);

    if (status != sf::Socket::Done)
    {
        // Implement Error
        _status = "Connection Error, Try again";
        isConnectButton = false;
        clientSocket->disconnect();
        clientSocket.reset();
    }
    else
    {
        _status = "Connecting.. Authenticating Password";

        // Send Password

        std::string _password = passBuffer;
        if (_password == "")
        {
            _password = "PASSWORD";
        }

        clientSocket->send(_password.c_str(), _password.length());

        // Get Password Ack
        char data[1];
        size_t i;
        clientSocket->receive(data, 1, i);

        if (data[0] == '0')
        {
            clientSocket->disconnect();
            _status = "Invalid Password";
            isConnectButton = false;
        }
        else if (data[0] == '1')
        {

            _status = "Connected";
            isConnected = true;
            sendThread = std::thread(sendData, clientSocket);
        }
    }
}

void sendData(std::shared_ptr<sf::TcpSocket> socket)
{
    try
    {

        while (true)
        {
            // if window is closed then break the loop
            if (isWindowClosed)
            {
                break;
            }

            std::string path = "";

            if (isDisconnectButton)
            {
                isDisconnectButton = false;
                isConnected = false;
                isConnectButton = false;
                isSend = false;
                isSendButton = false;
                throw("Disconnecting");
            }

            if (isSendButton)
            {

                isSendButton = false;
                path = pathBuffer;

                path = removeQuotes(path);

                std::filesystem::path checkPath(path);

                if (std::filesystem::is_directory(checkPath))
                {

                    sendFolder(socket, path);
                }
                else
                {
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

    std::ifstream file(filePath, std::ios_base::binary | std::ios_base::ate);

    if (!file)
    {
        std::cout << "Invalid File Path\n";
        _status = "Invalid Path";
        imStatus = _status;
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
    imStatus = _status;
    isSend = true;
    std::cout << "Data Sent!\n\n";
}

void sendFolder(std::shared_ptr<sf::TcpSocket> socket, std::string folderPath)
{

    std::vector<std::filesystem::path> filePaths;
    for (auto file : std::filesystem::directory_iterator(folderPath))
    {
        if (!file.is_directory())
            filePaths.emplace_back(file);
        else if (isRecursive && file.is_directory())
        {
            sendFolder(socket, std::filesystem::path(file).string());
        }
    }

    for (auto file : filePaths)
    {

        // sendFile(socket, file.string());

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
            _status = "Invalid Path";

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

std::string removeQuotes(std::string filePath)
{

    if (filePath[0] == '\"' && filePath[filePath.length() - 1] == '\"')
    {
        filePath.erase(filePath.begin());
        filePath.erase(filePath.end() - 1);
        return filePath;
    }
    return filePath;
}