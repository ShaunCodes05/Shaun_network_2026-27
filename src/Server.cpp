
#include "Server.h"

#include <chrono>


Server::Server()
{
    listener = std::make_unique<sf::TcpListener>();
}


Server::~Server()
{
    stop();
}


void Server::init()
{
    start(53000);
}


bool Server::start(unsigned short port)
{
    if (running)
    {
        std::cout << "Server is already running!\n";
        return false;
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

void Server::run()
{
    if (!running)
    {
        std::cout << "Server is not running!\n";
        return;
    }

    std::cout << "Waiting for clients...\n";

    while (running)
    {
        auto newSocket = std::make_shared<sf::TcpSocket>();

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

            newSocket->setBlocking(true);

            {
                std::lock_guard<std::mutex> lock(mutex);

                if (!running)
                    break;

                connections.push_back(newSocket);

                workers.emplace_back(
                    [this, newSocket]()
                    {
                        listen(newSocket);
                    }
                );
            }
        }
        else if (status != sf::Socket::Status::NotReady)
        {
            std::cout << "Connection failed on server side\n";
        }

        std::this_thread::sleep_for(
            std::chrono::milliseconds(1)
        );
    }

    std::cout << "Server stopped.\n";
}

void Server::listen(std::shared_ptr<sf::TcpSocket> socket)
{
    char data[1028];

    while (running)
    {
        std::size_t received = 0;

        sf::Socket::Status status =
            socket->receive(data, sizeof(data), received);

        if (status == sf::Socket::Status::Done ||
            status == sf::Socket::Status::Partial)
        {
            if (received > 0)
            {
                std::string message(data, received);

                std::cout << "Received: " << message << std::endl;
              
                relay(message, *socket);
            }
        }
        else if (status == sf::Socket::Status::Disconnected)
        {
            std::cout << "Client disconnected.\n";
            break;
        }
        else if (status == sf::Socket::Status::NotReady)
        {
            std::this_thread::sleep_for(
                std::chrono::milliseconds(1)
            );
        }
        else
        {
            std::cout << "Socket receive error.\n";
            break;
        }
    }

    removeClient(socket);
}

bool Server::sendAll(
    sf::TcpSocket& socket,
    const char* data,
    std::size_t size)
{
    std::size_t totalSent = 0;

    while (totalSent < size)
    {
        std::size_t sent = 0;

        sf::Socket::Status status = socket.send(
            data + totalSent,
            size - totalSent,
            sent
        );

        if (status != sf::Socket::Status::Done &&
            status != sf::Socket::Status::Partial)
        {
            return false;
        }

        if (sent == 0)
            return false;

        totalSent += sent;
    }

    return true;
}

void Server::relay(
    const std::string& message,
    sf::TcpSocket& sender_socket)
{
    std::lock_guard<std::mutex> lock(mutex);

    for (auto it = connections.begin();
        it != connections.end();)
    {
        if (it->get() == &sender_socket)
        {
            ++it;
            continue;
        }

        if (!sendAll(
            **it,
            message.data(),
            message.size()))
        {
            std::cout << "Failed to send to a client.\n";

            (*it)->disconnect();

            it = connections.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void Server::removeClient(
    const std::shared_ptr<sf::TcpSocket>& socket)
{
    std::lock_guard<std::mutex> lock(mutex);

    socket->disconnect();

    for (auto it = connections.begin();
        it != connections.end();)
    {
        if (*it == socket)
        {
            it = connections.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void Server::input()
{
    while (running)
    {
        std::string message;

        if (!std::getline(std::cin, message))
            break;

        message += '\n';

        std::lock_guard<std::mutex> lock(mutex);

        for (auto it = connections.begin();
            it != connections.end();)
        {
            if (!sendAll(
                **it,
                message.data(),
                message.size()))
            {
                (*it)->disconnect();

                it = connections.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}

void Server::stop()
{
    running = false;

    std::cout << "Stopping server...\n";

    std::vector<std::thread> threads;

    {
        std::lock_guard<std::mutex> lock(mutex);

        for (auto& connection : connections)
        {
            connection->disconnect();
        }

        threads.swap(workers);
    }

    for (auto& worker : threads)
    {
        if (worker.joinable())
        {
            worker.join();
        }
    }

    {
        std::lock_guard<std::mutex> lock(mutex);
        connections.clear();
    }

    std::cout << "Server stopped successfully.\n";
}
