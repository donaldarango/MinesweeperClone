#include <SFML/Graphics.hpp>
#include "Board.h"
#include "TextureManager.h"

int main()
{
    Board board("boards/config.cfg");

    sf::RenderWindow window(sf::VideoMode({(unsigned)board.getWidth() * 32, ((unsigned)board.getHeight() * 32) + 100}),
                            "Minesweeper");

    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::MouseButtonPressed>()) {
                if (!board.isGameOver() && (!board.CheckVictoryByFlag() || !board.CheckVictoryByReveal())) {
                    if (event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Right) {
                        Tile *clickedTile = board.MouseOnTile(window);
                        if (clickedTile == nullptr)
                            continue;
                        if (!clickedTile->isRevealed()) {
                            if (!clickedTile->isFlagged())
                                board.PlaceFlag(window, clickedTile, &event.value());
                            else if (clickedTile->isFlagged())
                                board.RemoveFlag(window, clickedTile, &event.value());
                        }
                    }
                    if (event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {
                        Tile *clickedTile = board.MouseOnTile(window);
                        if (clickedTile == nullptr) {
                            board.DebuggerClicked(window);
                            board.FaceClicked(window);
                            board.TestBoardClicked(window);
                        }
                        else if (clickedTile->isRevealed())
                            continue;
                        else if (!clickedTile->isFlagged())
                            board.Reveal(window, clickedTile, &event.value());
                    }
                }
                else {
                    if (event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {
                        Tile *clickedTile = board.MouseOnTile(window);
                        if (clickedTile == nullptr) {
                            board.FaceClicked(window);
                            board.TestBoardClicked(window);
                        }
                    }
                }
            }
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // clear the window with black color
        window.clear(sf::Color::White);
        board.InitializeSprites(&window);
        board.MineCounter(&window);
        if (board.CheckVictoryByFlag() || board.CheckVictoryByReveal()) {
            board.DrawVictoryFace(window);
        }
        if (board.isGameOver()) {
            board.DrawLoserFace(window);
        }

        // end the current frame
        window.display();
    }

    TextureManager::Clear();
    return 0;
}
