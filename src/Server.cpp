
#include "Server.h"
#include <chrono>

Server::Server()
{
    listener = std::make_unique<sf::TcpListener>();
    clientSocket = std::make_unique<sf::TcpSocket>();
}

void Server::init()
{
    start(53000);
}

bool Server::start(unsigned short port)
{
    if (running)
    {
        std::cout << "Server is already running!" << std::endl;
        return false;
    }

    if (listener == nullptr)
    {
        listener = std::make_unique<sf::TcpListener>();
    }

    if (listener->listen(port) != sf::Socket::Status::Done)
    {
        std::cout << "Couldn't bind listener port: "
            << port << std::endl;
        return false;
    }

    listener->setBlocking(false);

    running = true;

    std::cout << "Server started on port: "
        << port << std::endl;

    return true;
}

bool Server::acceptClient()
{
    if (!running)
    {
        return false;
    }

    return listener->accept(*clientSocket)
        == sf::Socket::Status::Done;
}

void Server::run()
{
    if (!running)
    {
        std::cout << "Server is not running!" << std::endl;
        return;
    }

    std::cout << "Waiting for clients..." << std::endl;

    while (running)
    {
        auto newSocket = std::make_unique<sf::TcpSocket>();

        sf::Socket::Status status = listener->accept(*newSocket);

        if (status == sf::Socket::Status::Done)
        {
            auto address = newSocket->getRemoteAddress();

            std::cout << "Client connected: ";

            if (address)
            {
                std::cout << address->toString();
            }

            std::cout << ":"
                << newSocket->getRemotePort()
                << std::endl;

            newSocket->setBlocking(false);

            {
                std::lock_guard<std::mutex> lock(mutex);

                connections.push_back(std::move(newSocket));
            }
        }
        else if (status != sf::Socket::Status::NotReady)
        {
            std::cout << "Connection failed on server side"
                << std::endl;
        }

        std::this_thread::sleep_for(
            std::chrono::milliseconds(1)
        );
    }

    std::cout << "Server stopped." << std::endl;
}

void Server::stop()
{
    running = false;

    std::cout << "Stopping server..." << std::endl;
}
