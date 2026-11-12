#include "Client.h"

Client::Client()
{
    socket = std::make_unique<sf::TcpSocket>();
}

void Client::connect()
{
    if (socket == nullptr)
    {
        socket = std::make_unique<sf::TcpSocket>();
    }

    if (connect("127.0.0.1", 5300))
    {
        std::cout << "Connection Complete" << std::endl;
        connected = true;
    }
    else
    {
        std::cout << "Connection Failed to 127.0.0.1:5300"
            << std::endl;

        connected = false;
    }
}

bool Client::connect(const std::string& ip, unsigned short port)
{
    auto address = sf::IpAddress::resolve(ip);

    if (!address)
    {
        std::cout << "Could not resolve IP address!" << std::endl;
        return false;
    }

    return socket->connect(*address, port) == sf::Socket::Status::Done;
}