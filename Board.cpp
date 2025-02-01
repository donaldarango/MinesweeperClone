#include "Board.h"
#include "TextureManager.h"
#include <iostream>
#include <random>
#include <algorithm>

Board::Board(const std::string& configFile) {
    std::ifstream config(configFile, std::ifstream ::in);
    std::string output;
    std::vector<std::string> data;

    for (unsigned int i = 0; i < 3; i++) {
        getline(config, output);
        data.push_back(output);
    }
    config.close();


    width = stoi(data.at(0));
    height = stoi(data.at(1));
    configMineCount = stoi(data.at(2));
    currentMineCount = configMineCount;

    LoadBoardRand();

    LoadBoardData();

}

void Board::LoadBoardData() {

    board.resize(height);
    int counter = 0;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            Tile * tile = new Tile();
            tile->setMine(mineData[counter]);
            tile->MoveSprite(j*32, i*32);
            board[i].push_back(tile);
            counter++;
        }
    }

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            board[i][j]->SetAdjacentTiles(board, i, j);
        }
    }
}

void Board::InitializeSprites(sf::RenderTarget *target) {

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            board[i][j]->Draw(target);
        }
    }


    sf::Sprite menuSprite;
    menuSprite.setTexture(TextureManager::GetTexture("face_happy"));
    menuSprite.setPosition(width * 32 / 2, height * 32);
    target->draw(menuSprite);
    faceBounds = menuSprite.getGlobalBounds();

    menuSprite.setTexture(TextureManager::GetTexture("debug"));
    menuSprite.move(128, 0);
    target->draw(menuSprite);
    debugBounds = menuSprite.getGlobalBounds();

    menuSprite.setTexture(TextureManager::GetTexture("test_1"));
    menuSprite.move(64, 0);
    target->draw(menuSprite);
    test1Bounds = menuSprite.getGlobalBounds();

    menuSprite.setTexture(TextureManager::GetTexture("test_2"));
    menuSprite.move(64, 0);
    target->draw(menuSprite);
    test2Bounds = menuSprite.getGlobalBounds();


    menuSprite.setTexture(TextureManager::GetTexture("test_3"));
    menuSprite.move(64, 0);
    target->draw(menuSprite);
    test3Bounds = menuSprite.getGlobalBounds();



}

void Board::MineCounter(sf::RenderTarget *target) const {
    sf::Sprite sprite;
    sprite.setPosition(21, height*32);

    std::string mineCountStr = std::to_string(currentMineCount);
    if (currentMineCount < 100 && currentMineCount > 0)
        mineCountStr = "0" + mineCountStr;
    if (currentMineCount < 10 &&  currentMineCount > 0)
        mineCountStr = "0" + mineCountStr;
    if (currentMineCount == 0)
        mineCountStr = "00" + mineCountStr;
    if (currentMineCount < 0 && currentMineCount > -100) {
        mineCountStr = mineCountStr.substr(1, mineCountStr.length() - 1);
        mineCountStr = "0" + mineCountStr;
        if (currentMineCount > -10)
            mineCountStr = "0" + mineCountStr;
        sprite.setTexture(TextureManager::GetTexture("digits-"));
        sprite.setPosition(0, height*32);
        target->draw(sprite);
        sprite.setPosition(21, height*32);
    }




    for (int i = 0; i < mineCountStr.length(); ++i) {
        char indexChar = mineCountStr.at(i);
        std::string numberString;
            numberString = std::to_string(indexChar - 48);
        sprite.setTexture(TextureManager::GetTexture("digits" + numberString));
        target->draw(sprite);
        sprite.move(21, 0);
    }

}

void Board::DecreaseMineCount(sf::Event *event) {
    if (event->type == sf::Event::MouseButtonPressed)
    {
        if (event->mouseButton.button == sf::Mouse::Right)
        {
            currentMineCount--;
        }
    }

}

void Board::IncreaseMineCount(sf::Event *event) {
    if (event->type == sf::Event::MouseButtonPressed)
    {
        if (event->mouseButton.button == sf::Mouse::Right)
        {
            currentMineCount++;
        }
    }

}

Tile * Board::MouseOnTile(sf::RenderWindow &window) {

    if (sf::Mouse::isButtonPressed(sf::Mouse::Right) or sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        // transform the mouse position from window coordinates to world coordinates
        sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        for (int i = 0; i < height; ++i) {

            for (int j = 0; j < width; ++j) {

                sf::FloatRect bounds = board[i][j]->GetSprite().getGlobalBounds();
                // hit test
                if (bounds.contains(mouse)) {

                    return board[i][j];
                }

            }

        }
    }
    return nullptr;
}

void Board::PlaceFlag(sf::RenderWindow &window, Tile * tile, sf::Event *event) {
    if (tile == nullptr)
        return;

    tile->Flag();
    DecreaseMineCount(event);

}

void Board::RemoveFlag(sf::RenderWindow &window, Tile *tile, sf::Event *event) {
    if (tile == nullptr)
        return;

    tile->Flag();
    IncreaseMineCount(event);

}

void Board::Reveal(sf::RenderWindow &window, Tile *tile, sf::Event *event) {
    if (tile == nullptr)
        return;

    std::vector<Tile*> adjacentTiles = tile->getAdjacentTiles();

    if (tile->getMine()) {
        gameOver = true;
        ShowMines(window);
        return;
    }

    if (tile->getSurroundingMines() > 0) {
        if (!tile->isRevealed()) {
            tile->Reveal();
            revealedTiles++;
        }
        tile->SetTexture("tile_revealed");
        return;
    }

    if (!tile->isRevealed() && tile->getSurroundingMines() == 0) {
        if (!tile->isRevealed()) {
            tile->Reveal();
            revealedTiles++;
        }
        tile->SetTexture("tile_revealed");
    }

    for (int i = 0; i < adjacentTiles.size(); ++i) {
        if (adjacentTiles[i]->isFlagged())
            continue;
        if (adjacentTiles[i]->getMine())
            continue;
        if (adjacentTiles[i]->isRevealed())
            continue;
        else {
            if (!adjacentTiles[i]->isRevealed()) {
                adjacentTiles[i]->Reveal();
                revealedTiles++;
            }
            adjacentTiles[i]->SetTexture("tile_revealed");
            Reveal(window, adjacentTiles[i], event);
        }

    }


}

void Board::DebuggerClicked(sf::RenderWindow &window) {

    sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (debugBounds.contains(mouse)) {

        Tile::Debug();

        }
}

void Board::FaceClicked(sf::RenderWindow &window) {

    sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (faceBounds.contains(mouse)) {

        ResetGame();
        board.clear();

        LoadBoardRand();
        LoadBoardData();

    }

}

void Board::TestBoardClicked(sf::RenderWindow &window) {

    sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (test1Bounds.contains(mouse)) {

        ResetGame();
        board.clear();

        LoadSetBoard("boards/testboard1.brd");
        LoadBoardData();

    }

    else if (test2Bounds.contains(mouse)) {

        ResetGame();
        board.clear();

        LoadSetBoard("boards/testboard2.brd");
        LoadBoardData();

    }

    else if (test3Bounds.contains(mouse)) {

        ResetGame();
        board.clear();

        LoadSetBoard("boards/testboard3.brd");
        LoadBoardData();

    }

}

void Board::LoadBoardRand() {

    int mineLimit = 0;
    mineData.clear();
    currentMineCount = 0;

    for (int i = 0; i < height; ++i) {

        for (int j = 0; j < width; ++j) {

            int a = rand()%2;
            if (mineLimit == configMineCount)
                a = 0;
            if (a == 0)
                mineData.push_back(false);
            else {
                mineData.push_back(true);
                currentMineCount++;
                mineLimit++;
            }

        }

    }

    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(mineData.begin(), mineData.end(), g);

}

void Board::LoadSetBoard(const string& boardName) {

    mineData.clear();
    currentMineCount = 0;
    std::ifstream mines(boardName, std::ifstream::in);
    char ch;
    while (mines >> std::noskipws >> ch) {
        if (ch == '0')
            mineData.push_back(false);
        else if (ch == '1') {
            mineData.push_back(true);
            currentMineCount++;
        }
        else
            continue;
    }
    mines.close();

}

void Board::ResetGame() {


    for (int i = 0; i < board.size(); ++i) {

        for (int j = 0; j < board[i].size(); ++j) {

            board[i][j]->ResetTile();
            revealedTiles = 0;
            gameOver = false;

        }

    }

}

bool Board::CheckVictoryByFlag() {
    int mineCounter = currentMineCount;

    for (int i = 0; i < height; ++i) {

        for (int j = 0; j < width; ++j) {

            if (board[i][j]->getMine()) {
                if (board[i][j]->isFlagged())
                    mineCounter--;
                else
                    return false;
            }
            else
                continue;
        }

    }
    if (mineCounter <= 0)
        return  true;
    else
        return false;
}

bool Board::CheckVictoryByReveal() {

    if (height * width - revealedTiles == configMineCount)
        return true;
    else
        return false;

}

void Board::DrawVictoryFace(sf::RenderWindow &window) const {
    sf::Sprite menuSprite;
    menuSprite.setTexture(TextureManager::GetTexture("face_win"));
    menuSprite.setPosition(width * 32 / 2, height * 32);
    window.draw(menuSprite);

    for (int i = 0; i < height; ++i) {

        for (int j = 0; j < width; ++j) {

            if (board[i][j]->getMine() && !board[i][j]->isFlagged())
                board[i][j]->Flag();
        }
    }
}

void Board::DrawLoserFace(sf::RenderWindow &window) const {
    sf::Sprite menuSprite;
    menuSprite.setTexture(TextureManager::GetTexture("face_lose"));
    menuSprite.setPosition(width * 32 / 2, height * 32);
    window.draw(menuSprite);
}

void Board::ShowMines(sf::RenderWindow &window) {
    for (int i = 0; i < height; ++i) {

        for (int j = 0; j < width; ++j) {

            if (board[i][j]->getMine()) {
                board[i][j]->SetTexture("tile_revealed");
                board[i][j]->Reveal();
                board[i][j]->Draw(&window);

            }

        }

    }
}


int Board::getWidth() const {
    return width;
}

int Board::getHeight() const {
    return height;
}

bool Board::isGameOver() const {
    return gameOver;
}





