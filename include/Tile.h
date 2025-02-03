#ifndef PROJECT3_TILE_H
#define PROJECT3_TILE_H
#include <SFML/Graphics.hpp>
#include "TextureManager.h"


class Tile {
    static bool debuggerState;
    sf::Texture texture = TextureManager::GetTexture("tile_hidden");
    sf::Sprite tileSprite = sf::Sprite(texture);
    float xPos;
    float yPos;
    bool hasMine = false;
    bool flagged = false;
    bool revealed = false;
    std::vector<Tile*> adjacentTiles;
    int surroundingMines = 0;

public:
    Tile() {
        tileSprite.setTexture(texture);
        xPos = tileSprite.getPosition().x;
        yPos = tileSprite.getPosition().y;

    }
    void SetAdjacentTiles(const std::vector<std::vector<Tile*> >& board, int i, int j);
    void MoveSprite(float x, float y);
    void Draw(sf::RenderTarget *target);
    void ResetTile();
    void SetTexture(const string& textureString);
    void Flag();
    bool isFlagged() const;
    static void Debug();
    void Reveal();
    bool isRevealed() const;
    void setMine(bool mineValue);
    sf::Sprite GetSprite();
    bool getMine() const;
    std::vector<Tile*> getAdjacentTiles() const;
    int getSurroundingMines() const;

};

#endif //PROJECT3_TILE_H
