
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

void Client::input()
{
    while (running)
    {
        std::string str;
		std::getline(std::cin, str);
		std::cin.clear();

        if (connected)
        {
			socket->send(reinterpret_cast<char*>(str.data()),str.length());
        }
    }
}

void Client::run()
{
	char static_buffer[1028];
    while (connected)
    {
		std::size_t received;
		sf::Socket::Status status = socket->receive(static_buffer, sizeof(static_buffer), received);

        if (status == sf::Socket::Status::Disconnected)
        {
            connected = false;
            std::cout << "Disconnected from server." << std::endl;
			socket->disconnect();
            break;
		}
        else
        {
			if (received < 1028) static_buffer[received] = '\0';
			std::cout << "Received: " << static_buffer << std::endl;
        }
    }
}
