#pragma once
#include <SFML/Network.hpp>
#include <thread>
#include <vector>
#include <iostream>
#include <mutex>
#include <memory>

class Server
{
public:
	void init();
	void run();
	void listen(sf::TcpSocket& c_socket);
	void send(std::string buffer, sf::TcpSocket& c_socket);
	void input();

	Server();

	bool start(unsigned short port);
	bool acceptClient();

private:
	std::vector <std::thread> workers;
	std::vector<std::unique_ptr<sf::TcpSocket>> connections;

	std::unique_ptr<sf::TcpListener> listener;
	std::unique_ptr<sf::TcpSocket> clientSocket;

	bool running = true;
	std::mutex mutex;
};

