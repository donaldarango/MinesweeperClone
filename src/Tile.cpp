#include "Tile.h"

bool Tile::debuggerState = false;

void Tile::SetAdjacentTiles(const std::vector<std::vector<Tile *> > &board, int i, int j) {
    // i: height
    // j: width

    // top
    if (i - 1 < board.size() && j - 1 >= 0) {
        adjacentTiles.push_back(board[i - 1][j - 1]);
        if (board[i - 1][j - 1]->hasMine)
            surroundingMines++;
    }
    if (i - 1 < board.size()) {
        adjacentTiles.push_back(board[i - 1][j]);
        if (board[i - 1][j]->hasMine)
            surroundingMines++;
    }
    if (i - 1 < board.size() && j + 1 < board[i].size()) {
        adjacentTiles.push_back(board[i - 1][j + 1]);
        if (board[i - 1][j + 1]->hasMine)
            surroundingMines++;
    }

    // middle
    if (j - 1 >= 0) {
        adjacentTiles.push_back(board[i][j - 1]);
        if (board[i][j - 1]->hasMine)
            surroundingMines++;
    }
    if (j + 1 < board[i].size()) {
        adjacentTiles.push_back(board[i][j + 1]);
        if (board[i][j + 1]->hasMine)
            surroundingMines++;
    }

    // bottom
    if (i + 1 < board.size() && j - 1 >= 0) {
        adjacentTiles.push_back(board[i + 1][j - 1]);
        if (board[i + 1][j - 1]->hasMine)
            surroundingMines++;
    }
    if (i + 1 < board.size()) {
        adjacentTiles.push_back(board[i + 1][j]);
        if (board[i + 1][j]->hasMine)
            surroundingMines++;
    }
    if (i + 1 < board.size() && j + 1 < board[i].size()) {
        adjacentTiles.push_back(board[i + 1][j + 1]);
        if (board[i + 1][j + 1]->hasMine)
            surroundingMines++;
    }
}

void Tile::MoveSprite(float x, float y) {
    tileSprite.setPosition({x, y});
    xPos = x;
    yPos = y;
}

void Tile::Draw(sf::RenderTarget *target) {
    target->draw(tileSprite);

    if (flagged) {
        sf::Sprite flag = sf::Sprite(TextureManager::GetTexture("flag"));
        flag.setPosition({this->xPos, this->yPos});
        target->draw(flag);
    }
    if (revealed) {
        if (this->hasMine) {
            sf::Sprite mine = sf::Sprite(TextureManager::GetTexture("mine"));
            mine.setPosition({this->xPos, this->yPos});
            target->draw(mine);
        }
        else if (surroundingMines != 0) {
            sf::Sprite mineNumber = sf::Sprite(TextureManager::GetTexture("number_" + std::to_string(surroundingMines)));
            mineNumber.setPosition({this->xPos, this->yPos});
            target->draw(mineNumber);
        }
    }
    if (debuggerState) {
        if (this->hasMine) {
            sf::Sprite mine = sf::Sprite(TextureManager::GetTexture("mine"));
            mine.setPosition({this->xPos, this->yPos});
            target->draw(mine);
        }
    }
}

void Tile::ResetTile() {
    flagged = false;
    revealed = false;
    texture = texture = TextureManager::GetTexture("tile_hidden");
}

void Tile::SetTexture(const string &textureString) {
    texture = TextureManager::GetTexture(textureString);
}

void Tile::Flag() {
    flagged = !flagged;
}

bool Tile::isFlagged() const {
    return flagged;
}

void Tile::Debug() {
    debuggerState = !debuggerState;
}

void Tile::Reveal() {
    revealed = !revealed;
}

bool Tile::isRevealed() const {
    return revealed;
}

void Tile::setMine(bool mineValue) {
    hasMine = mineValue;
}

sf::Sprite Tile::GetSprite() {
    return tileSprite;
}

bool Tile::getMine() const {
    return hasMine;
}

std::vector<Tile*> Tile::getAdjacentTiles() const {
    return adjacentTiles;
}

int Tile::getSurroundingMines() const {
    return surroundingMines;
}



