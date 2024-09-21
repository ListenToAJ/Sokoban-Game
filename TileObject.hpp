// Copyright 2023 <Anthony Simao>

#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

enum tileType{Wall, Box, Target, Floor};

class TileObject{
 public:
    explicit TileObject(tileType);
    tileType getTileType() const { return type; }
    std::shared_ptr<sf::Texture> texture;
 private:
    tileType type;
};
