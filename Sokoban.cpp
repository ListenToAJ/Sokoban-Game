// Copyright 2023 <Anthony Simao>

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "Sokoban.hpp"
#include "TileObject.hpp"

void Sokoban::loadFile(std::vector<std::shared_ptr<sf::Texture>> texts) {
    wall    = texts[0];
    box     = texts[1];
    empty   = texts[2];
    storage = texts[3];
    pDown   = texts[4];
    pUp     = texts[5];
    pRight  = texts[6];
    pLeft   = texts[7];
}

void Sokoban::changePlayerPos(int deltaX, int deltaY) {
    sf::Vector2u newPCoords(pCoords.x + deltaX, pCoords.y + deltaY);

    // * Make sure player is not out of bounds or noclipping
    if (newPCoords.x >= 0 && newPCoords.x <= w-1
        && newPCoords.y >= 0 && newPCoords.y <= h-1
        && gameBoard[newPCoords.y][newPCoords.x].back().getTileType() != Wall) {
        // * If player is trying to move into a box
        if (gameBoard[newPCoords.y][newPCoords.x].back().getTileType() == Box) {
            // * If the box isn't going out of bounds
            if ((deltaX && newPCoords.x > 0 && newPCoords.x < w-1)
                || (deltaY && newPCoords.y > 0 && newPCoords.y < h-1)) {
                int yChck = deltaY != 0 ? pCoords.y + (2 * deltaY) : pCoords.y;
                int xChck = deltaX != 0 ? pCoords.x + (2 * deltaX) : pCoords.x;

                // * Check if the box can be moved
                if (std::none_of(gameBoard[yChck][xChck].begin(),
                    gameBoard[yChck][xChck].end(),
                    [](const TileObject& tile) {
                        tileType t = tile.getTileType();
                        return t == Wall || t == Box;})) {
                    gameBoard[yChck][xChck].push_back(gameBoard[newPCoords.y][newPCoords.x].back());
                    gameBoard[newPCoords.y][newPCoords.x].pop_back();
                    pCoords = newPCoords;
                }
            }
        } else {
            pCoords = newPCoords;
        }
    }
}

std::istream& operator>>(std::istream& in, Sokoban& game) {
    in >> game.h;
    in >> game.w;
    game.tCount = 0;

    // * Prepare gameboard with accurate size
    game.gameBoard.resize(game.h);
    for (int i = 0; i < game.h; i++) {
        game.gameBoard[i].resize(game.w);
    }

    // * Clear gameboard (used for resets)
    for (int i = 0; i < game.h; i++) {
        for (int j = 0; j < game.w; j++) {
            game.gameBoard[i][j].clear();
        }
    }

    for (int i = 0; i < game.h; i++) {
        std::string iLine;
        in >> iLine;

        for (int j = 0; j < game.w; j++) {
            char input = iLine.at(j);

            TileObject newFloor(Floor);

            newFloor.texture = game.empty;

            game.gameBoard[i][j].push_back(newFloor);

            switch (input) {
                // Player spawn
                case '@': {
                    game.pCoords.x = j;
                    game.pCoords.y = i;
                    game.player.setPosition(game.pCoords.x * 64, game.pCoords.y * 64);
                    break;
                }
                // Wall
                case '#': {
                    TileObject newWall(Wall);
                    newWall.texture = game.wall;
                    game.gameBoard[i][j].push_back(newWall);
                    break;
                }
                // Target
                case 'a': {
                    TileObject newTarget(Target);
                    newTarget.texture = game.storage;
                    game.tCount++;
                    game.gameBoard[i][j].push_back(newTarget);
                    break;
                }
                // Box
                case 'A': {
                    TileObject newBox(Box);
                    newBox.texture = game.box;
                    game.gameBoard[i][j].push_back(newBox);
                    break;
                }
                // Target then Box
                case '1': {
                    TileObject newTarget(Target);
                    newTarget.texture = game.storage;
                    game.tCount++;
                    game.gameBoard[i][j].push_back(newTarget);

                    TileObject newBox(Box);
                    newBox.texture = game.box;
                    game.gameBoard[i][j].push_back(newBox);
                    break;
                }
            }
        }
    }
    if (game.pDown) game.player.setTexture(*game.pDown);
    return in;
}

void Sokoban::movePlayer(const Direction dir) {
    if (!isWon()) {
        std::shared_ptr<sf::Texture> facing;
        switch (dir) {
            case Up:
                changePlayerPos(0, -1);
                facing = pUp;
                break;
            case Down:
                changePlayerPos(0, 1);
                facing = pDown;
                break;
            case Left:
                changePlayerPos(-1, 0);
                facing = pLeft;
                break;
            case Right:
                changePlayerPos(1, 0);
                facing = pRight;
                break;
        }
        if (facing) player.setTexture(*facing);
        player.setPosition(pCoords.x * 64, pCoords.y * 64);
    }
}

bool Sokoban::isWon() const {
    size_t bCount = 0;
    bool won = true;

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (gameBoard[i][j].back().getTileType() == Box
            && gameBoard[i][j].at(gameBoard[i][j].size() - 2).getTileType() != Target) {
                won = false;
            } else if (gameBoard[i][j].back().getTileType() == Box
            && gameBoard[i][j].at(gameBoard[i][j].size() - 2).getTileType() == Target) {
                bCount++;
            }
        }
    }
    if (bCount >= tCount) {
        return true;
    }
    return won;
}

void Sokoban::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // * Y - Axis
    for (int i = 0; i < h; i++) {
        // * X - Axis
        for (int j = 0; j < w; j++) {
            // * Z - Axis
            for (int k = 0; k < gameBoard[i][j].size(); k++) {
                sf::Sprite currentSprite;

                currentSprite.setTexture(*gameBoard[i][j].at(k).texture);

                sf::Vector2f coords;

                coords.y = i * 64;
                coords.x = j * 64;

                currentSprite.setPosition(coords);
                target.draw(currentSprite, states);
            }
        }
    }
    target.draw(player, states);
}
