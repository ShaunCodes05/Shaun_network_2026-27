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

    void connect();
    void input();
    void run();

    std::atomic<bool> _running = false;
    std::atomic<bool> connected = false;

    bool connect(const std::string& ip, unsigned short port);

private:
    std::unique_ptr<sf::TcpSocket> socket;
};