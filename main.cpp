// Copyright 2023 <Anthony Simao>

#include <iostream>
#include <fstream>
#include <memory>

#include <SFML/Graphics.hpp>
#include "Sokoban.hpp"

int main(int argc, char* argv[]) {
    // * Read in game
    Sokoban game;
    std::vector<std::shared_ptr<sf::Texture>> load;
    load.resize(8);
    std::generate(load.begin(), load.end(), std::make_shared<sf::Texture>);
    load[0]->loadFromFile("block_06.png");
    load[1]->loadFromFile("crate_03.png");
    load[2]->loadFromFile("ground_01.png");
    load[3]->loadFromFile("ground_04.png");
    load[4]->loadFromFile("player_05.png");
    load[5]->loadFromFile("player_08.png");
    load[6]->loadFromFile("player_17.png");
    load[7]->loadFromFile("player_20.png");
    game.loadFile(load);

    std::ifstream f(argv[1]);
    f >> game;

    // * Window Creation
    size_t windowWidth = game.width() * 64, windowHeight = game.height() * 64;
    sf::RenderWindow window(sf::VideoMode(windowWidth,
                                          windowHeight),
                            "Sokoban", sf::Style::Close);
    sf::Clock clock;

    // * Font / Text Creation
    sf::Font font;
    if (!font.loadFromFile("./pixel.ttf")) return EXIT_FAILURE;
    sf::Text text("YOU WON", font, 70);
    text.setFillColor(sf::Color::White);
    text.setLetterSpacing(.4);
    text.setOrigin(text.getLocalBounds().width / 2,
                   text.getLocalBounds().height / 2);
    text.setOutlineThickness(0);
    text.setPosition(windowWidth / 2 - 9, windowHeight / 2);

    // * Game Loop
    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }
        float delay = .15;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)
        && clock.getElapsedTime() > sf::seconds(delay)) {
            game.movePlayer(Up);
            clock.restart();
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)
        && clock.getElapsedTime() > sf::seconds(delay)) {
            game.movePlayer(Left);
            clock.restart();
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)
        && clock.getElapsedTime() > sf::seconds(delay)) {
            game.movePlayer(Down);
            clock.restart();
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)
        && clock.getElapsedTime() > sf::seconds(delay)) {
            game.movePlayer(Right);
            clock.restart();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            std::ifstream f(argv[1]);
            f >> game;
        }

        window.clear();
        window.draw(game);
        if (game.isWon()) {
            window.draw(text);
        }
        window.display();
    }
    return 0;
}
