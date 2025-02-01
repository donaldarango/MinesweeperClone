#ifndef PROJECT3_BOARD_H
#define PROJECT3_BOARD_H

#include <map>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <vector>
#include "Tile.h"

class Board {
private:
    int width;
    int height;
    int configMineCount;
    int currentMineCount;
    int revealedTiles = 0;
    bool gameOver = false;
    sf::FloatRect faceBounds;
    sf::FloatRect debugBounds;
    sf::FloatRect test1Bounds;
    sf::FloatRect test2Bounds;
    sf::FloatRect test3Bounds;
    std::vector<std::vector<Tile *>> board;
    std::vector<bool> mineData;


public:
    explicit Board(const std::string& configFile);

    void LoadBoardData();

    void InitializeSprites(sf::RenderTarget *target);

    void MineCounter(sf::RenderTarget *target) const;

    void DecreaseMineCount(sf::Event *event);

    void IncreaseMineCount(sf::Event *event);

    Tile * MouseOnTile(sf::RenderWindow &window);

    void PlaceFlag(sf::RenderWindow &window, Tile * tile, sf::Event *event);

    void RemoveFlag(sf::RenderWindow &window, Tile * tile, sf::Event *event);

    void Reveal(sf::RenderWindow &window, Tile * tile, sf::Event *event);

    void DebuggerClicked(sf::RenderWindow &window);

    void FaceClicked(sf::RenderWindow &window);

    void TestBoardClicked(sf::RenderWindow &window);

    void LoadBoardRand();

    void LoadSetBoard(const string& boardName);

    void ResetGame();

    bool CheckVictoryByFlag();

    bool CheckVictoryByReveal();

    void DrawVictoryFace(sf::RenderWindow &window) const;

    void DrawLoserFace(sf::RenderWindow &window) const;

    void ShowMines(sf::RenderWindow &window);

    // GETTERS

    int getWidth() const;

    int getHeight() const;

    bool isGameOver() const;

};


#endif //PROJECT3_BOARD_H
