
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

    void init();
    void run();
    void stop();

    void listen(sf::TcpSocket& c_socket);
    void send(std::string buffer, sf::TcpSocket& c_socket);
    void input();

    bool start(unsigned short port);
    bool acceptClient();

private:
    std::vector<std::thread> workers;

    std::vector<std::unique_ptr<sf::TcpSocket>> connections;

    std::unique_ptr<sf::TcpListener> listener;
    std::unique_ptr<sf::TcpSocket> clientSocket;

    std::atomic<bool> running{ false };

    std::mutex mutex;
};
