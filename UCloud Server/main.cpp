#include <iostream>

#include <SFML/Network.hpp>
#include <SFML/System.hpp>

#include <thread>
#include <fstream>
#include <ctime>
#include <mutex>
#include <filesystem>

#define _CRT_SECURE_NO_WARNINGS 1

void startServer();
void receiveData(std::shared_ptr<sf::TcpSocket> socket);

// Helper Functions
std::string getTime();

std::mutex clientMutex;
std::vector<std::thread> clientThreads;

int main()
{
    std::cout << "Welcome to UCloud Server!\n";
    std::cout << "--------------------------\n";
    std::cout << "Server is starting\n";

    startServer();
}

void startServer()
{

    while (true)
    {
        auto listener = std::make_shared<sf::TcpListener>();

        if (listener->listen(55555) != sf::Socket::Done)
        {
            std::cout << "Listen Error\n";
            return;
        }
        auto client = std::make_shared<sf::TcpSocket>();

        if (listener->accept(*client) != sf::Socket::Done)
        {
            std::cout << "Accept Error\n";
            return;
        }

        std::cout << "Connected to Client\n";
        std::lock_guard<std::mutex> lock(clientMutex);
        clientThreads.emplace_back(std::thread(receiveData, client));
    }

    for (std::thread& Thread : clientThreads)
    {
        if (Thread.joinable())
            Thread.join();
    }
}

void receiveData(std::shared_ptr<sf::TcpSocket> socket)
{
    try
    {

        while (true)
        {
            char* data = new char[1024];
            std::vector<char> totalData = {};
            std::string extention = "";
            std::string dataLen = "";
            std::size_t received = 0;
            std::size_t dataRecevied = 0;
            std::size_t dataLenInt;

            if (socket->receive(data, 10, received) != sf::Socket::Done)
            {
                delete[] data;
                throw("Unable To Receive Meta Data");
            }

            dataLen = data;
            std::cout << dataLen << ":" << data << "\n";
            dataLenInt = stoi(dataLen.substr(1, dataLen.length() - 1));

            std::cout << "Receiving Data...\n";
            std::cout << "Data Size: " << dataLenInt << "\n";
            std::cout << "Do Not Close This Window...\n";

            if (socket->send("ACK", 3) != sf::Socket::Done)
            {
                delete[] data;

                throw("Unable To Acknowledge\n");
            }

            if (socket->receive(data, stoi(dataLen.substr(0, 1)), received) != sf::Socket::Done)
            {
                delete[] data;
                throw("Unable To Receive Meta Data");
            }

            extention = data;
            extention = extention.substr(0, received);

            char* fileData = new char[dataLenInt];

            if (socket->send("ACK", 3) != sf::Socket::Done)
            {
                delete[] data;
                delete[] fileData;

                throw("Unable To Acknowledge\n");
            }

            if (socket->receive(fileData, dataLenInt, received) != sf::Socket::Done)
            {
                delete[] data;
                delete[] fileData;

                throw("Unable To Receive Data\n");
            }

            { // while (dataRecevied < dataLenInt)
              // {
              //     received = 0;
              //     socket.receive(data, 1024, received);
              //     dataRecevied += received;

                //     for (int i = 0; i < received; i++)
                //     {
                //         totalData.push_back(data[i]);
                //     }
                // }
            }

            for (int i = 0; i < received; i++)
            {
                
                totalData.push_back(fileData[i]);

            }

            std::string fileName = "CloudImage ";
            fileName.append(getTime());
            fileName.append("." + extention);

            std::ofstream file(fileName, std::ios_base::binary);

            // file << fileData;

            for (char c : totalData)
            {
                file << c;
            }

            std::cout << "Data Stored!\n";
            file.close();


            if (socket->send("ACK", 3) != sf::Socket::Done)
            {
                delete[] data;
                delete[] fileData;

                throw("Unable To Acknowledge\n");
            }

            delete[] data;
            delete[] fileData;
        }
    }
    catch (std::exception e)
    {
        std::cout << "Exception";
        std::cout << e.what() << "\n";
        std::cout << "Client " << socket->getRemoteAddress() << " closed!\n";
        socket->disconnect();
        return;
    }
    catch (...)
    {
        std::cout << "Client " << socket->getRemoteAddress() << " closed!\n";
        socket->disconnect();
        return;
    }
}

std::string prevTime;
int times = 0;

std::string getTime()
{
    // time format:
    //  year-month-date at hour-min-sec
    std::string Time;

    time_t now = time(0);

    std::tm* time = std::localtime(&now);

    Time.append(std::to_string(time->tm_year));
    Time.append("-" + std::to_string(time->tm_mon));
    Time.append("-" + std::to_string(time->tm_mday));
    Time.append(" at ");
    Time.append(std::to_string(time->tm_hour));
    Time.append("." + std::to_string(time->tm_min));
    Time.append("." + std::to_string(time->tm_sec));

    if (Time == prevTime)
    {
        times++;
        Time.append(" " + std::to_string(times));
        // prevTime = Time;
    }
    else
    {
        prevTime = Time;
        times = 0;
    }
    return Time;
}
