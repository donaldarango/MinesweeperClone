#include <SFML/Graphics.hpp>
#include "Board.h"
#include "TextureManager.h"

int main()
{

    Board board("boards/config.cfg");

    sf::RenderWindow window(sf::VideoMode(board.getWidth() * 32, (board.getHeight() * 32) + 100), "Minesweeper");
    // run the program as long as the window is open

    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::MouseButtonPressed) {
                if (!board.isGameOver() && (!board.CheckVictoryByFlag() || !board.CheckVictoryByReveal())) {

                    if (event.mouseButton.button == sf::Mouse::Right) {
                        Tile *clickedTile = board.MouseOnTile(window);
                        if (clickedTile == nullptr)
                            continue;

                        if (!clickedTile->isRevealed()) {
                            if (!clickedTile->isFlagged())
                                board.PlaceFlag(window, clickedTile, &event);
                            else if (clickedTile->isFlagged())
                                board.RemoveFlag(window, clickedTile, &event);

                        }

                    }

                    if (event.mouseButton.button == sf::Mouse::Left) {
                        Tile *clickedTile = board.MouseOnTile(window);
                        if (clickedTile == nullptr) {
                            board.DebuggerClicked(window);
                            board.FaceClicked(window);
                            board.TestBoardClicked(window);
                        }

                        else if(clickedTile->isRevealed())
                            continue;
                        else if (!clickedTile->isFlagged())
                            board.Reveal(window, clickedTile, &event);

                    }

                }

                else {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        Tile *clickedTile = board.MouseOnTile(window);
                        if (clickedTile == nullptr) {
                            board.FaceClicked(window);
                            board.TestBoardClicked(window);

                        }
                    }
                }
            }


            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
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
