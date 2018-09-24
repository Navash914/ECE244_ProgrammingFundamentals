/*
 *  ECE244_Lab2
 *  TicTacToe
 *
 *  main.cpp
 *
 *  Created by Naveed Ashfaq
 *
 */

#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>

#include "playMove.h"

using namespace std;
using namespace sf;

int main() {

    // The game board array
    int gameBoard[3][3] = {
            Empty, Empty, Empty,
            Empty, Empty, Empty,
            Empty, Empty, Empty
    };

    // A boolean that represents the turn in the game
    bool isXTurn = true;

    // A boolean to represent if the move is valid
    bool isMoveValid = true;

    // A boolean to represent if the game is over
    bool isGameOver = false;

    // An integer that represents the win code
    int winCode = 0;

    /************************************************************************************************************/
    /* Loads the various images used by the game and creates the sprites                                        */
    /************************************************************************************************************/

    // Instantiate textures
    sf::Texture X_texture, O_texture, blank_texture;

    // Get the X image
    if (!X_texture.loadFromFile("X.jpg"))
        return EXIT_FAILURE;

    // Get the O image
    if (!O_texture.loadFromFile("O.jpg"))
        return EXIT_FAILURE;

    // Get the blank image
    if (!blank_texture.loadFromFile("blank.jpg"))
        return EXIT_FAILURE;

    // Instantiate sprites
    sf::Sprite X_sprite(X_texture), O_sprite(O_texture), blank_sprite(blank_texture);

    /************************************************************************************************************/
    /* Creates the window and displays it                                                                       */
    /************************************************************************************************************/

    // The tile size is the size of the images.
    const int tileSize = X_texture.getSize().x;

    // The sizes of the dividing rectangles.
    const int barWidth = tileSize/20;
    const int barHeight = boardSize*tileSize + (boardSize-1)*barWidth;

    // May add a buffer on window edges for better presentation.
    const int buffer = 0;

    // The size of the window.
    const int windowSize = boardSize*tileSize + (boardSize-1)*barWidth + 2 * buffer;

    // Create the main window: it has a title bar and a close button, but is not resizable.
    sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "ECE244 Tic-Tac-Toe", sf::Style::Titlebar | sf::Style::Close);

    // Set the Icon that appears in the task bar when the window is minimized.
    sf::Image windowIcon;
    if (!windowIcon.loadFromFile("icon.jpg"))
        return EXIT_FAILURE;

    window.setIcon(windowIcon.getSize().x, windowIcon.getSize().y, windowIcon.getPixelsPtr());


    // Create the vertical (horizontal) borders as rectangles of sizes barWidth and barHeight (barHeight and barWidth).
    sf::RectangleShape bar(sf::Vector2f(barWidth, barHeight));
    bar.setFillColor(sf::Color::Black);

    // Vertical bars
    bar.setPosition(tileSize, buffer);
    window.draw(bar);
    bar.setPosition(2 * tileSize + barWidth, buffer);
    window.draw(bar);

    // Horizontal bars
    bar.setSize(sf::Vector2f(barHeight, barWidth));
    bar.setPosition(buffer, tileSize);
    window.draw(bar);
    bar.setPosition(buffer, 2 * tileSize + barWidth);
    window.draw(bar);

    // Display initial (empty) board.
    window.display();

    /************************************************************************************************************/
    /* Main event loop                                                                                          */
    /************************************************************************************************************/

    // Start the game loop that runs as long as the window is open
    while (window.isOpen()) {
        // The event
        sf::Event event;

        // Process the events
        while (window.pollEvent(event)) {

            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();

            // Escape pressed: exit
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                window.close();

            // Left mouse button pressed: get the click position and play the move
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                // Don't process input if game is over.
                if (isGameOver)
                    continue;

                // left mouse button is pressed: get the coordinates in pixels.
                sf::Vector2i mousePosition = Mouse::getPosition(window);

                // Convert the pixel coordinates into game board rows and columns.
                int row, col; // row and column

                int x = mousePosition.x - buffer;
                if (x < 0) // Left Buffer Area.
                    continue;
                else if (x < tileSize) // First Column Tile.
                    col = 0;
                else if (x < tileSize + barWidth) // First Vertical Bar.
                    continue;
                else if (x < 2 * tileSize + barWidth) // Second Column Tile.
                    col = 1;
                else if (x < 2 * tileSize + 2 * barWidth) // Second Vertical Bar.
                    continue;
                else if (x < 3 * tileSize + 2 * barWidth) // Third Column Tile.
                    col = 2;
                else // Right Buffer Area.
                    continue;

                int y = mousePosition.y - buffer;
                if (y < 0) // Upper Buffer Area.
                    continue;
                else if (y < tileSize) // First Row Tile.
                    row = 0;
                else if (y < tileSize + barWidth) // First Horizontal Bar.
                    continue;
                else if (y < 2 * tileSize + barWidth) // Second Row Tile.
                    row = 1;
                else if (y < 2 * tileSize + 2 * barWidth) // Second Horizontal Bar.
                    continue;
                else if (y < 3 * tileSize + 2 * barWidth) // Third Row Tile.
                    row = 2;
                else // Lower Buffer Area.
                    continue;

                // Play the move.
                playMove(gameBoard, row, col, isXTurn, isMoveValid, isGameOver, winCode);
            }
        }

        // Draw the game tiles.
        int x, y;
        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                x = buffer + j * (tileSize + barWidth);
                y = buffer + i * (tileSize + barWidth);
                sf::Sprite tile;
                switch(gameBoard[i][j]) {
                    case X :
                        tile = X_sprite;
                        break;

                    case O :
                        tile = O_sprite;
                        break;

                    default :
                        tile = blank_sprite;
                        break;
                }
                tile.setPosition(x, y);
                window.draw(tile);
            }
        }

        // Draw win line when game is over, unless there is no winner.
        if (isGameOver && winCode != 0) {
            const int winLineWidth = windowSize;
            const int winLineHeight = 10;
            sf::RectangleShape winLine(sf::Vector2f(winLineWidth, winLineHeight));
            winLine.setFillColor(sf::Color::White);

            const int rotationAngle = winCode < 4 ? 0 : // Horizontal
                                      winCode < 7 ? 90 : // Vertical
                                      winCode == 7 ? 45 : -45; // Diagonal
            if (winCode >= 7)
                winLine.setScale(1.415, 1.0);

            winLine.setOrigin(winLine.getSize().x / 2, winLine.getSize().y / 2);
            winLine.setRotation(rotationAngle);
            float posX = (float) windowSize / 2, posY = (float) windowSize / 2;
            switch (winCode) {
                case 1 : // Row 0
                    posY -= tileSize + barWidth;
                    break;

                case 3 : // Row 2
                    posY += tileSize + barWidth;
                    break;

                case 4 : // Column 0
                    posX -= tileSize + barWidth;
                    break;

                case 6 : // Column 2
                    posX += tileSize + barWidth;
                    break;

                default :
                    break;
            }
            winLine.setPosition(posX, posY);
            window.draw(winLine);
        }

        // Display to the window
        window.display();
    }

    return EXIT_SUCCESS;
}
