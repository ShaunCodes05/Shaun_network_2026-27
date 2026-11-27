
#pragma once

#include <SFML/Network.hpp>
#include <iostream>
#include <atomic>
#include <thread>
#include <memory>
#include <string>

class Client
{
public:
    Client();

    // Connection
    void connect();

    bool connect(const std::string& ip, unsigned short port);

    // Client functionality
    void input();
    void run();

    // Client state
    std::atomic<bool> running{ false };
    std::atomic<bool> connected{ false };

private:
    std::unique_ptr<sf::TcpSocket> socket;
};
