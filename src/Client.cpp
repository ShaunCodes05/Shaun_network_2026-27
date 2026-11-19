
#include "Client.h"

Client::Client()
{
    socket = std::make_unique<sf::TcpSocket>();
    connected = false;
}

void Client::connect()
{
    if (socket == nullptr)
    {
        socket = std::make_unique<sf::TcpSocket>();
    }

    socket->disconnect();

    std::cout << "Connecting to 127.0.0.1:53000..."
        << std::endl;

    if (connect("127.0.0.1", 53000))
    {
        std::cout << "Connection Complete!"
            << std::endl;

        connected = true;
    }
    else
    {
        std::cout << "Connection Failed to 127.0.0.1:53000"
            << std::endl;

        connected = false;
    }
}

bool Client::connect(const std::string& ip, unsigned short port)
{
    if (socket == nullptr)
    {
        socket = std::make_unique<sf::TcpSocket>();
    }

    auto address = sf::IpAddress::resolve(ip);

    if (!address)
    {
        std::cout << "Could not resolve IP address!"
            << std::endl;
        return false;
    }

    sf::Socket::Status status = socket->connect(*address, port);

    if (status != sf::Socket::Status::Done)
    {
        std::cout << "Socket connection failed! Status: "
            << static_cast<int>(status)
            << std::endl;

        return false;
    }

    return true;
}
