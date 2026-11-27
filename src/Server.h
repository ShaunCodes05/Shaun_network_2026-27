
#pragma once

#include <SFML/Network.hpp>
#include <thread>
#include <vector>
#include <iostream>
#include <mutex>
#include <memory>
#include <atomic>
#include <string>

class Server
{
public:
    Server();
    ~Server();

    void init();
    void run();
    void stop();
    void input();

    void listen(std::shared_ptr<sf::TcpSocket> socket);

    void relay(
        const std::string& message,
        sf::TcpSocket& sender_socket
    );

    bool start(unsigned short port);

private:
    bool sendAll(
        sf::TcpSocket& socket,
        const char* data,
        std::size_t size
    );

    void removeClient(
        const std::shared_ptr<sf::TcpSocket>& socket
    );

    std::vector<std::thread> workers;

    std::vector<std::shared_ptr<sf::TcpSocket>> connections;

    std::unique_ptr<sf::TcpListener> listener;

    std::atomic<bool> running{ false };

    std::mutex mutex;
};
