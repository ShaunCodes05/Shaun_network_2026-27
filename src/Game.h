#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "Board.h"
#include "Server.h"
#include "Client.h"

class Game
{
public:
    Game(sf::RenderWindow& window);
    ~Game();

    bool init();
    void update(float dt);
    void render();

    void mouseButtonPressed(const sf::Event::MouseButtonPressed* event);
    void mouseButtonReleased(const sf::Event::MouseButtonReleased* event);
    void keyPressed(const sf::Event::KeyPressed* event);
    void keyReleased(const sf::Event::KeyReleased* event);
    void mouseMoved(const sf::Event::MouseMoved* event);

private:
    sf::RenderWindow& window;

    std::shared_ptr<Board> board;
    sf::Font font;

    

    sf::Text hostText {font};
    sf::Text joinText {font};

    std::unique_ptr<Server> server;
    std::unique_ptr<Client> client;

    enum GameState
    {
        MENU,
        GAME
    };

    GameState game_state = MENU;

    std::unique_ptr<sf::RectangleShape> host_button;
    std::unique_ptr<sf::RectangleShape> join_button;

  

};

