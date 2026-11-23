#include "raylib.h"
#include <iostream>
using namespace std;

// Board size
const int ROWS = 8;
const int COLS = 9;

// Window size
const int SCREEN_WIDTH = 1050;
const int SCREEN_HEIGHT = 800;

// Board settings
const int CELL_SIZE = 75;
const int BOARD_X = 40;
const int BOARD_Y = 130;

// Game variables
char board[8][9];
int diceplace[9] = { 0,0,0,0,0,0,0,0,0 };

int currentPlayer = 1;
bool gameOver = false;
bool drawGame = false;

// Function to initialize board
void initialize_board()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            board[i][j] = '.';
        }
    }

    for (int i = 0; i < 9; i++)
    {
        diceplace[i] = 0;
    }

    currentPlayer = 1;
    gameOver = false;
    drawGame = false;
}

// Function to drop ball
bool dropdice(int col, char symbol)
{
    if (col < 0 || col > 8)
        return false;

    if (diceplace[col] == 8)
        return false;

    int emprow = 7 - diceplace[col];

    board[emprow][col] = symbol;

    diceplace[col]++;

    return true;
}

// Function to check winner
bool checkWin(char symbol)
{
    // Horizontal
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c <= 4; c++)
        {
            if (board[r][c] == symbol &&
                board[r][c + 1] == symbol &&
                board[r][c + 2] == symbol &&
                board[r][c + 3] == symbol &&
                board[r][c + 4] == symbol)
            {
                return true;
            }
        }
    }

    // Vertical
    for (int r = 0; r <= 3; r++)
    {
        for (int c = 0; c < 9; c++)
        {
            if (board[r][c] == symbol &&
                board[r + 1][c] == symbol &&
                board[r + 2][c] == symbol &&
                board[r + 3][c] == symbol &&
                board[r + 4][c] == symbol)
            {
                return true;
            }
        }
    }

    // Right diagonal
    for (int r = 0; r <= 3; r++)
    {
        for (int c = 0; c <= 4; c++)
        {
            if (board[r][c] == symbol &&
                board[r + 1][c + 1] == symbol &&
                board[r + 2][c + 2] == symbol &&
                board[r + 3][c + 3] == symbol &&
                board[r + 4][c + 4] == symbol)
            {
                return true;
            }
        }
    }

    // Left diagonal
    for (int r = 0; r <= 3; r++)
    {
        for (int c = 4; c < 9; c++)
        {
            if (board[r][c] == symbol &&
                board[r + 1][c - 1] == symbol &&
                board[r + 2][c - 2] == symbol &&
                board[r + 3][c - 3] == symbol &&
                board[r + 4][c - 4] == symbol)
            {
                return true;
            }
        }
    }

    return false;
}
// Function to draw game board
void drawBoard()
{
    // Black board background
    DrawRectangle(
        BOARD_X,
        BOARD_Y,
        COLS * CELL_SIZE,
        ROWS * CELL_SIZE,
        GRAY
    );

    // Draw balls
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            int x = BOARD_X + c * CELL_SIZE + CELL_SIZE / 2;
            int y = BOARD_Y + r * CELL_SIZE + CELL_SIZE / 2;

            if (board[r][c] == 'X')
            {
                // Player 1 - Red
                DrawCircle(x, y, 27, YELLOW);
            }
            else if (board[r][c] == 'O')
            {
                // Player 2 - Blue
                DrawCircle(x, y, 27, RED);
            }
            else
            {
                // Empty spaces
                DrawCircle(x, y, 27, LIGHTGRAY);
            }
        }
    }
}

// Function to draw buttons
void drawButton(int x, int y, int width, int height, const char text[])
{
    Rectangle button = {
        (float)x,
        (float)y,
        (float)width,
        (float)height
    };

    Vector2 mouse = GetMousePosition();

    if (CheckCollisionPointRec(mouse, button))
    {
        DrawRectangleRounded(button, 0.2, 8, GOLD);
    }
    else
    {
        DrawRectangleRounded(button, 0.2, 8, LIGHTGRAY);
    }

    DrawRectangleRoundedLines(button, 0.2, 8, DARKGRAY);

    int textWidth = MeasureText(text, 20);

    DrawText(
        text,
        x + (width - textWidth) / 2,
        y + 15,
        20,
        BLACK
    );
}

// Function to handle player move
void handlePlayerMove()
{
    Vector2 mouse = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        if (mouse.x >= BOARD_X &&
            mouse.x < BOARD_X + COLS * CELL_SIZE &&
            mouse.y >= BOARD_Y &&
            mouse.y < BOARD_Y + ROWS * CELL_SIZE)
        {
            if (gameOver)
                return;

            int col = (mouse.x - BOARD_X) / CELL_SIZE;

            char symbol;

            if (currentPlayer == 1)
                symbol = 'X';
            else
                symbol = 'O';

            if (dropdice(col, symbol))
            {
                if (checkWin(symbol))
                {
                    gameOver = true;
                }
                else
                {
                    bool fullBoard = true;

                    for (int i = 0; i < COLS; i++)
                    {
                        if (diceplace[i] < ROWS)
                        {
                            fullBoard = false;
                            break;
                        }
                    }

                    if (fullBoard)
                    {
                        gameOver = true;
                        drawGame = true;
                    }
                    else
                    {
                        if (currentPlayer == 1)
                            currentPlayer = 2;
                        else
                            currentPlayer = 1;
                    }
                }
            }
        }
    }
}

// Main function
int main()
{
    // Initialize window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Connect 5 Game");

    SetTargetFPS(60);

    initialize_board();

    // Main game loop
    while (!WindowShouldClose())
    {
        // Handle mouse clicks
        handlePlayerMove();

        Vector2 mouse = GetMousePosition();

        // Rematch button
        Rectangle rematchButton = { 750, 300, 200, 55 };

        if (CheckCollisionPointRec(mouse, rematchButton) &&
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            initialize_board();
        }

        // Exit button
        Rectangle exitButton = { 750, 380, 200, 55 };

        if (CheckCollisionPointRec(mouse, exitButton) &&
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            break;
        }

        // Start drawing
        BeginDrawing();

        // Light grey background
        ClearBackground(BLACK);

        // Game title
        DrawText("CONNECT 5", 380, 35, 40, ORANGE);

        // Draw board
        drawBoard();

        // Player turn information
        if (!gameOver)
        {
            if (currentPlayer == 1)
            {
                DrawText("PLAYER 1 TURN", 750, 150, 25, YELLOW);
            }
            else
            {
                DrawText("PLAYER 2 TURN", 750, 150, 25, RED);
            }
        }

        // Player information
        DrawText("PLAYER 1", 750, 210, 22, YELLOW);
        DrawText("RED BALL", 750, 240, 18, WHITE);

        DrawText("PLAYER 2", 750, 450, 22, RED);
        DrawText("BLUE BALL", 750, 480, 18, WHITE);

        // Buttons
        drawButton(750, 300, 200, 55, "REMATCH");
        drawButton(750, 380, 200, 55, "EXIT");

        // Winner message
        if (gameOver)
        {
            if (drawGame)
            {
                DrawText("GAME DRAW!", 750, 550, 25, WHITE);
            }
            else if (currentPlayer == 1)
            {
                DrawText("PLAYER 1 WINS!", 750, 550, 25, YELLOW);
            }
            else
            {
                DrawText("PLAYER 2 WINS!", 750, 550, 25, RED);
            }
        }

        EndDrawing();
    }

    // Close window
    CloseWindow();

    return 0;
}