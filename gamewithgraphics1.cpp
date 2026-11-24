#include <iostream>
#include <raylib.h>
#include <cstdlib>
#include <string>
using namespace std;

char board[8][9];
int diceplace[9] = { 0,0,0,0,0,0,0,0,0 };

void print_board()
{
    for (int p = 0; p < 8; p++)
    {
        for (int k = 0; k < 9; k++)
        {
            cout << board[p][k] << " ";
        }
        cout << endl;
    }
}

void initialize_board()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            board[i][j] = '.';
        }
    }
    print_board();
}

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

bool checkWin(char symbol)
{
    //  Horizontal
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c <= 4; c++)
        {
            if (board[r][c] == symbol && board[r][c + 1] == symbol && board[r][c + 2] == symbol && board[r][c + 3] == symbol && board[r][c + 4] == symbol)
            {
                return true;
            }
        }
    }
    //  Vertical
    for (int r = 0; r <= 3; r++)
    {
        for (int c = 0; c < 9; c++)
        {
            if (board[r][c] == symbol && board[r + 1][c] == symbol && board[r + 2][c] == symbol && board[r + 3][c] == symbol && board[r + 4][c] == symbol)
            {
                return true;
            }
        }
    }
    //  Right Diagonal 
    for (int r = 0; r <= 3; r++)
    {
        for (int c = 0; c <= 4; c++)
        {
            if (board[r][c] == symbol && board[r + 1][c + 1] == symbol && board[r + 2][c + 2] == symbol && board[r + 3][c + 3] == symbol && board[r + 4][c + 4] == symbol)
            {
                return true;
            }
        }
    }
    //  Left Diagonal 
    for (int r = 0; r <= 3; r++)
    {
        for (int c = 4; c < 9; c++)
        {
            if (board[r][c] == symbol && board[r + 1][c - 1] == symbol && board[r + 2][c - 2] == symbol && board[r + 3][c - 3] == symbol && board[r + 4][c - 4] == symbol)
            {
                return true;
            }
        }
    }
    return false;
}

void Player(int num)
{
    int col;
    cout << "PLAYER " << num << "'s TURN!\n";
    while (true)
    {
        cout << "Enter Column (1-9): ";
        cin >> col;
        if (col < 1 || col>9)
        {
            cout << "Invalid Column.\nRe-";
            continue;
        }
        col = col - 1;
        if (diceplace[col] == 8)
        {
            cout << "Column is Full.\nPlease Try Again.\n";
            continue;
        }

        break;
    }
    char symbol;
    if (num == 1)
        symbol = 'X';
    else
        symbol = 'O';
    dropdice(col, symbol);
}

int main()
{
    const int screenWidth = 1250;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "CONNECT FIVE - GUI BASED GAME");
    SetTargetFPS(60);

    initialize_board();

    int turn = 1;
    bool gameOver = false;
    string statusText = "PLAYER 1's TURN (X)";
    Color statusColor = VIOLET;

    const int cellSize = 76;
    const int boardWidth = 9 * cellSize;
    const int boardHeight = 8 * cellSize;
    const int boardX = (screenWidth - boardWidth) / 2;
    const int boardY = 120;

    Rectangle restartBtn = { (float)(screenWidth / 2 - 90), (float)(screenHeight - 62), 180.0f, 42.0f };

    while (!WindowShouldClose())
    {
        int num = (turn % 2 != 0) ? 1 : 2;
        char sym = (num == 1) ? 'X' : 'O';
        Vector2 mousePos = GetMousePosition();

        int hoveredCol = -1;
        if (!gameOver && turn <= 72)
        {
            if (mousePos.x >= boardX && mousePos.x < boardX + boardWidth && mousePos.y >= boardY && mousePos.y < boardY + boardHeight)
            {
                hoveredCol = (int)((mousePos.x - boardX) / cellSize);
            }

            int selectedCol = -1;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && hoveredCol != -1)
            {
                selectedCol = hoveredCol;
            }
            else
            {
                for (int k = 0; k < 9; k++)
                {
                    if (IsKeyPressed(KEY_ONE + k))
                    {
                        selectedCol = k;
                        break;
                    }
                }
            }

            if (selectedCol != -1)
            {
                if (selectedCol < 0 || selectedCol > 8)
                {
                    statusText = "Invalid Column. Try Again.";
                    statusColor = ORANGE;
                }
                else if (diceplace[selectedCol] == 8)
                {
                    statusText = "Column " + to_string(selectedCol + 1) + " is Full. Please Try Again.";
                    statusColor = ORANGE;
                }
                else
                {
                    dropdice(selectedCol, sym);
                    print_board();

                    if (checkWin(sym))
                    {
                        statusText = "PLAYER " + string(1, sym) == "X" ? "PLAYER 1 (X) WINS!" : "PLAYER 2 (O) WINS!";
                        statusText = "PLAYER " + to_string(num) + " (" + sym + ") WINS!";
                        statusColor = GREEN;
                        gameOver = true;
                    }
                    else if (turn == 72)
                    {
                        statusText = "DRAW!";
                        statusColor = YELLOW;
                        gameOver = true;
                    }
                    else
                    {
                        turn++;
                        int nextNum = (turn % 2 != 0) ? 1 : 2;
                        char nextSym = (nextNum == 1) ? 'X' : 'O';
                        statusText = string("PLAYER ") + to_string(nextNum) + "'s TURN (" + nextSym + ")";
                        statusColor = RAYWHITE;
                    }
                }
            }
        }

        if (gameOver && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, restartBtn))
        {
            for (int c = 0; c < 9; c++)
            {
                diceplace[c] = 0;
            }
            initialize_board();
            turn = 1;
            gameOver = false;
            statusText = "PLAYER 1's TURN (X) - Click a column or press 1-9";
            statusColor = RAYWHITE;
        }

        BeginDrawing();
        ClearBackground(Color{ 22, 27, 34, 255 });

        DrawText("8x9 DROP FIVE", boardX, 28, 32, RAYWHITE);
        DrawText(statusText.c_str(), boardX, 72, 22, statusColor);

        // Column headers 1-9 and hover preview piece
        for (int c = 0; c < 9; c++)
        {
            int cx = boardX + c * cellSize;
            Color headerCol = (c == hoveredCol) ? SKYBLUE : LIGHTGRAY;
            DrawText(to_string(c + 1).c_str(), cx + cellSize / 2 - 6, boardY - 26, 20, headerCol);

            if (c == hoveredCol && diceplace[c] < 8)
            {
                Color previewCol = (sym == 'X') ? Fade(RED, 0.45f) : Fade(GOLD, 0.45f);
                DrawCircle(cx + cellSize / 2, boardY - 12, 10.0f, previewCol);
            }
        }

        // Board frame
        DrawRectangle(boardX - 8, boardY - 8, boardWidth + 16, boardHeight + 16, Color{ 30, 64, 124, 255 });
        DrawRectangleLines(boardX - 8, boardY - 8, boardWidth + 16, boardHeight + 16, Color{ 90, 140, 220, 255 });

        if (hoveredCol != -1)
        {
            DrawRectangle(boardX + hoveredCol * cellSize, boardY, cellSize, boardHeight, Fade(WHITE, 0.08f));
        }

        // Cells and pieces ('X' and 'O')
        for (int r = 0; r < 8; r++)
        {
            for (int c = 0; c < 9; c++)
            {
                int centerX = boardX + c * cellSize + cellSize / 2;
                int centerY = boardY + r * cellSize + cellSize / 2;
                float radius = cellSize * 0.38f;

                // Slot background
                DrawCircle(centerX, centerY, radius, Color{ 15, 20, 28, 255 });

                if (board[r][c] == 'X')
                {
                    DrawCircle(centerX, centerY, radius, RED);
                    DrawCircleLines(centerX, centerY, radius - 5.0f, MAROON);
                    DrawText("X", centerX - 9, centerY - 12, 26, WHITE);
                }
                else if (board[r][c] == 'O')
                {
                    DrawCircle(centerX, centerY, radius, GOLD);
                    DrawCircleLines(centerX, centerY, radius - 5.0f, ORANGE);
                    DrawText("O", centerX - 10, centerY - 12, 26, BLACK);
                }
            }
        }

        if (gameOver)
        {
            bool hoverRestart = CheckCollisionPointRec(mousePos, restartBtn);
            DrawRectangleRec(restartBtn, hoverRestart ? SKYBLUE : Color{ 45, 95, 175, 255 });
            DrawRectangleLinesEx(restartBtn, 2.0f, RAYWHITE);
            DrawText("PLAY AGAIN", (int)restartBtn.x + 28, (int)restartBtn.y + 11, 20, WHITE);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}