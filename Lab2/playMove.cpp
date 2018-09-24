/*
 *  ECE244_Lab2
 *  TicTacToe
 *
 *  playMove.cpp
 *
 *  Created by Naveed Ashfaq
 *
 */

#include <iostream>
#include "playMove.h"

using namespace std;

/*enum WinCode {
    No_Win,
    Row_0,
    Row_1,
    Row_2,
    Column_0,
    Column_1,
    Column_2,
    Left_To_Right_Diagonal,
    Right_To_Left_Diagonal
};*/

void checkGameOver(int board[3][3], bool& gameOver, int& winCode);

void playMove(int gameBoard[3][3], int row, int col, bool& isXTurn, bool& isValidMove, bool& isGameOver, int& winCode) {

    // If the game is over, do nothing
    if (isGameOver)
        return;

    // Check if move is valid.
    bool rowInBounds = row >= 0 && row < boardSize;
    bool colInBounds = col >= 0 && col < boardSize;

    isValidMove = rowInBounds && colInBounds && gameBoard[row][col] == Empty;

    if (isValidMove) {
        // Move is valid.

        if (isXTurn)
            gameBoard[row][col] = X;
        else
            gameBoard[row][col] = O;

        isXTurn = !isXTurn;
        checkGameOver(gameBoard, isGameOver, winCode);
    }

    // Print output
    for (int i = 0; i < boardSize; i++)
        for (int j = 0; j < boardSize; j++)
            cout << gameBoard[i][j] << " ";

    cout << row << " " << col << " "
         << isXTurn << " " << isValidMove << " "
         << isGameOver << " " << winCode << endl;

}

// Checks if game is over and updates winCode.
void checkGameOver(int gameBoard[3][3], bool& isGameOver, int& winCode) {

    /*
     * Win Codes:
     *  0 - No Win
     *  1 - Row 0
     *  2 - Row 1
     *  3 - Row 2
     *  4 - Column 0
     *  5 - Column 1
     *  6 - Column 2
     *  7 - Left to Right Diagonal
     *  8 - Right to Left Diagonal
     */

    int emptyCount = 0; // How many empty tiles left.
    int rowStart, colStart, diag1Start, diag2Start; // Starting tile values to match to.
    // Number of matches along each diagonal. diag1 is left to right and diag2 is right to left
    int diag1Count = 0, diag2Count = 0;
    for (int i = 0; i < boardSize; i++) {
        rowStart = gameBoard[i][0], colStart = gameBoard[0][i],
        diag1Start = gameBoard[0][0], diag2Start = gameBoard[0][boardSize-1];

        int rowCount = 0, colCount = 0; // Number of matches in row or col.

        for (int j = 0; j < boardSize; j++) {
            int tile = gameBoard[i][j];

            if (tile == Empty)
                emptyCount++; // Found empty tile.

            if (tile == rowStart)
                rowCount++; // Found match with starting row.
            if (gameBoard[j][i] == colStart)
                colCount++; // Found match with starting column.

            if (i == j && tile == diag1Start)
                diag1Count++; // Found match with starting diag1.
            if (i == boardSize - j - 1 && tile == diag2Start)
                diag2Count++; // Found match with starting diag2.
        }

        // Check for win.
        // First tile must not be empty for win.
        if (rowStart != Empty && rowCount == boardSize)
            winCode = i + 1; // Row win.

        if (colStart != Empty && colCount == boardSize)
            winCode = i + 4; // Column win.

        if (diag1Start != Empty && diag1Count == boardSize)
            winCode = 7; // Diag1 win.

        if (diag2Start != Empty && diag2Count == boardSize)
            winCode = 8; // Diag2 win.

    }

    // Check game over.
    if (winCode > 0)
        isGameOver = true; // Game over if a player won.
    else
        isGameOver = emptyCount == 0; // Game over if no empty tiles left.

}
