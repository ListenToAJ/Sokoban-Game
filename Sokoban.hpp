// Copyright 2023 <Anthony Simao>

#pragma once

#include <vector>
#include <deque>
#include <memory>

#include <SFML/Graphics.hpp>
#include "TileObject.hpp"

enum Direction{Up, Down, Left, Right};

class Sokoban : public sf::Drawable {
 public:
    size_t width() { return w; }
    size_t height() { return h; }
    void movePlayer(const Direction);
    bool isWon() const;
    void loadFile(std::vector<std::shared_ptr<sf::Texture>>);
 protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
 private:
    size_t h, w, tCount;
    sf::Vector2u pCoords;
    Direction playerDirection;
    sf::Sprite player;

    std::vector<std::vector<std::deque<TileObject>>> gameBoard;
    std::shared_ptr<sf::Texture> wall, box, empty, storage, pDown, pUp, pRight, pLeft;

    void changePlayerPos(int, int);
    friend std::istream& operator>>(std::istream& in, Sokoban&);
    friend std::istream& operator<<(std::istream& out, const Sokoban&);
};
