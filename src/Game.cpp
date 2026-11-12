
#include "Game.h"
#include "Tile.h"
#include <iostream>

Game::Game(sf::RenderWindow& game_window)
    : window(game_window),
    hostText(font),
    joinText(font)
{
}

Game::~Game()
{
    
}

bool Game::init()
{
    if (!font.openFromFile("Data/Fonts/OpenSans-Bold.ttf"))
    {
        std::cout << "Font failed to load!" << std::endl;
        return false;
    }

    board = std::make_shared<Board>(window);
    board->init();

    host_button = std::make_unique<sf::RectangleShape>(
        sf::Vector2f(250.f, 80.f));

    join_button = std::make_unique<sf::RectangleShape>(
        sf::Vector2f(250.f, 80.f));

    host_button->setPosition({ 850.f, 300.f });
    join_button->setPosition({ 850.f, 450.f });

    host_button->setFillColor(sf::Color(80, 80, 80));
    join_button->setFillColor(sf::Color(80, 80, 80));

    hostText.setString("HOST GAME");
    hostText.setCharacterSize(30);
    hostText.setFillColor(sf::Color::White);
    hostText.setPosition({ 890.f, 320.f });

    joinText.setString("JOIN GAME");
    joinText.setCharacterSize(30);
    joinText.setFillColor(sf::Color::White);
    joinText.setPosition({ 890.f, 470.f });

    return true;
}

void Game::update(float dt)
{

}

void Game::render()
{
    if (game_state == MENU)
    {
        board->render();

        window.draw(*host_button);
        window.draw(*join_button);

        window.draw(hostText);
        window.draw(joinText);
    }
    else if (game_state == GAME)
    {
        board->render();
    }
}

void Game::mouseButtonPressed(
    const sf::Event::MouseButtonPressed* event)
{
    if (game_state == MENU)
    {
        sf::Vector2f mousePosition(
            static_cast<float>(event->position.x),
            static_cast<float>(event->position.y)
        );

        if (host_button->getGlobalBounds().contains(mousePosition))
        {
            game_state = GAME;

            std::cout << "Creating server." << std::endl;

            server = std::make_unique<Server>();

            if (!server->start(53000))
            {
                std::cout << "Server failed to start." << std::endl;
                server.reset();
                game_state = MENU;
            }
        }
        else if (join_button->getGlobalBounds().contains(mousePosition))
        {
            game_state = GAME;

            std::cout << "Attempting to connect to server." << std::endl;

            client = std::make_unique<Client>();

            if (!client->connect("127.0.0.1", 53000))
            {
                std::cout << "Connection failed." << std::endl;
                client.reset();
                game_state = MENU;
            }
        }
    }
    else if (game_state == GAME)
    {
        board->mouseButtonPressed(event);
    }
}

void Game::mouseButtonReleased(const sf::Event::MouseButtonReleased* event)
{
    board->mouseButtonReleased(event);    
}

void Game::keyPressed(const sf::Event::KeyPressed* event)
{

}

void Game::keyReleased(const sf::Event::KeyReleased* event)
{

}

void Game::mouseMoved(const sf::Event::MouseMoved* event)
{
    board->mouseMoved(event);
}




