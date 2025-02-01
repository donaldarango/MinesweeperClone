#ifndef PROJECT3_TILE_H
#define PROJECT3_TILE_H
#include <SFML/Graphics.hpp>
#include "TextureManager.h"


class Tile {
    static bool debuggerState;
    sf::Texture texture;
    sf::Sprite tileSprite;
    float xPos;
    float yPos;
    bool hasMine = false;
    bool flagged = false;
    bool revealed = false;
    std::vector<Tile*> adjacentTiles;
    int surroundingMines = 0;


public:
    Tile() {
        
        texture = TextureManager::GetTexture("tile_hidden");
        tileSprite.setTexture(texture);
        xPos = tileSprite.getPosition().x;
        yPos = tileSprite.getPosition().y;

    }

    void SetAdjacentTiles(const std::vector<std::vector<Tile *>>& board, int i, int j);

    void MoveSprite(float x, float y);

    void Draw(sf::RenderTarget *target);

    void ResetTile();

    void SetTexture(const string& textureString);

    void Flag() {
        flagged = !flagged;
    }

    bool isFlagged() const {
        return flagged;
    }

    static void Debug() {
        debuggerState = !debuggerState;
    }

    void Reveal() {
        revealed = !revealed;
    }

    bool isRevealed() const {
        return revealed;
    }

    void setMine(bool mineValue) {
        hasMine = mineValue;
    }

    sf::Sprite GetSprite() {
        return tileSprite;
    }

    bool getMine() const {
        return hasMine;
    }

    std::vector<Tile*> getAdjacentTiles() const{
        return adjacentTiles;
    }

    int getSurroundingMines() const {
        return surroundingMines;
    }



};



#endif //PROJECT3_TILE_H
