#include "Server.h"

Server::Server()
{
    listener = std::make_unique<sf::TcpListener>();
    clientSocket = std::make_unique<sf::TcpSocket>();
}

void Server::init()
{
    if (listener == nullptr)
    {
        listener = std::make_unique<sf::TcpListener>();
    }

    if (listener->listen(5300) != sf::Socket::Status::Done)
    {
        std::cout << "Couldn't bind listener port" << std::endl;
    }
}

bool Server::start(unsigned short port)
{
    return listener->listen(port) == sf::Socket::Status::Done;
}

bool Server::acceptClient()
{
    return listener->accept(*clientSocket) == sf::Socket::Status::Done;
}

void Server::run()
{
    while (running)
    {
        sf::TcpSocket& c_sock =
            *connections.emplace_back(std::make_unique<sf::TcpSocket>());

        if (listener->accept(c_sock) != sf::Socket::Status::Done)
        {
            connections.pop_back();

            std::cout << "Connection failed on server side" << std::endl;
            continue;
        }

        std::cout << "Client connected @ "
            << c_sock.getRemotePort() << std::endl;
    }
}