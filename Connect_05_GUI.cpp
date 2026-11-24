#include <iostream>
#include <raylib.h>
#include <cstdlib>
#include <string>
#include <cmath>
using namespace std;

char board[8][9];
int diceplace[9] = { 0,0,0,0,0,0,0,0,0 };

void print_board()
{
    system("cls");
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

static float SmoothEnv(int i, int len, float attackRatio = 0.08f)
{
    float p = (float)i / (float)len;
    float env = 1.0f;
    if (p < attackRatio) env = p / attackRatio;
    else env = powf(1.0f - ((p - attackRatio) / (1.0f - attackRatio)), 1.6f);
    return env < 0.0f ? 0.0f : env;
}

static Sound MakeDropSfx()
{
    const int rate = 44100, len = (int)(0.09f * rate);
    float* buf = (float*)calloc(len, sizeof(float));
    for (int i = 0; i < len; i++)
    {
        float p = (float)i / len;
        float freq = 260.0f + 220.0f * p; // soft upward pop
        float s = sinf(2.0f * PI * freq * ((float)i / rate)) * 0.7f + sinf(4.0f * PI * freq * ((float)i / rate)) * 0.3f;
        buf[i] = s * SmoothEnv(i, len, 0.05f) * 0.28f;
    }
    Wave w = { (unsigned int)len, (unsigned int)rate, 32, 1, buf };
    Sound s = LoadSoundFromWave(w);
    UnloadWave(w);
    return s;
}

static Sound MakeFullSfx()
{
    const int rate = 44100, len = (int)(0.14f * rate);
    float* buf = (float*)calloc(len, sizeof(float));
    for (int i = 0; i < len; i++)
    {
        float t = (float)i / rate;
        float s = sinf(2.0f * PI * 155.0f * t) * 0.75f + sinf(2.0f * PI * 110.0f * t) * 0.25f;
        buf[i] = s * SmoothEnv(i, len, 0.06f) * 0.26f;
    }
    Wave w = { (unsigned int)len, (unsigned int)rate, 32, 1, buf };
    Sound s = LoadSoundFromWave(w);
    UnloadWave(w);
    return s;
}

static Sound MakeWinSfx()
{
    const int rate = 44100;
    const float notes[5] = { 523.25f, 659.25f, 783.99f, 1046.50f, 1046.50f };
    const float durs[5] = { 0.11f,   0.11f,   0.11f,   0.14f,    0.36f };
    int total = 0;
    for (int k = 0; k < 5; k++) total += (int)(durs[k] * rate);
    float* buf = (float*)calloc(total, sizeof(float));

    int idx = 0;
    for (int k = 0; k < 5; k++)
    {
        int len = (int)(durs[k] * rate);
        for (int i = 0; i < len && idx < total; i++, idx++)
        {
            float t = (float)i / rate;
            float f = notes[k];
            // Add bright octave shimmer on the final held note
            float sig = sinf(2.0f * PI * f * t) * 0.65f + sinf(4.0f * PI * f * t) * 0.25f;
            if (k == 4) sig += sinf(2.0f * PI * (f * 1.5f) * t) * 0.2f;
            buf[idx] = sig * SmoothEnv(i, len, 0.08f) * 0.34f;
        }
    }
    Wave w = { (unsigned int)total, (unsigned int)rate, 32, 1, buf };
    Sound s = LoadSoundFromWave(w);
    UnloadWave(w);
    return s;
}

static Sound MakeDrawSfx()
{
    const int rate = 44100;
    const float notes[3] = { 392.00f, 349.23f, 261.63f };
    const float durs[3] = { 0.18f,   0.18f,   0.42f };
    int total = 0;
    for (int k = 0; k < 3; k++) total += (int)(durs[k] * rate);
    float* buf = (float*)calloc(total, sizeof(float));

    int idx = 0;
    for (int k = 0; k < 3; k++)
    {
        int len = (int)(durs[k] * rate);
        for (int i = 0; i < len && idx < total; i++, idx++)
        {
            float t = (float)i / rate;
            float f = notes[k];
            float sig = sinf(2.0f * PI * f * t) * 0.75f + sinf(4.0f * PI * f * t) * 0.25f;
            buf[idx] = sig * SmoothEnv(i, len, 0.12f) * 0.30f;
        }
    }
    Wave w = { (unsigned int)total, (unsigned int)rate, 32, 1, buf };
    Sound s = LoadSoundFromWave(w);
    UnloadWave(w);
    return s;
}

int main()
{
    const int cell = 72, bx = 110, by = 130;
    InitWindow(870, 790, "CONNECT FIVE - GUI BASED GAME");
    InitAudioDevice();
    SetTargetFPS(60);

    Sound dropSfx = MakeDropSfx();
    Sound fullSfx = MakeFullSfx();
    Sound winSfx = MakeWinSfx();
    Sound drawSfx = MakeDrawSfx();

    initialize_board();
    int turn = 1;
    bool gameOver = false;
    string msg = "PLAYER 1's TURN (X)";
    Color msgCol = RED;
    Rectangle restart = { 345.0f, 725.0f, 180.0f, 40.0f };

    while (!WindowShouldClose())
    {
        int num = (turn % 2 != 0) ? 1 : 2;
        char sym = (num == 1) ? 'X' : 'O';
        Vector2 m = GetMousePosition();

        int hover = -1;
        if (m.x >= bx && m.x < bx + 9 * cell && m.y >= by && m.y < by + 8 * cell)
            hover = (int)((m.x - bx) / cell);

        if (!gameOver && turn <= 72)
        {
            int col = (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && hover != -1) ? hover : -1;
            for (int k = 0; k < 9 && col == -1; k++)
                if (IsKeyPressed(KEY_ONE + k)) col = k;

            if (col != -1)
            {
                if (diceplace[col] == 8)
                {
                    msg = "Column " + to_string(col + 1) + " is Full!";
                    msgCol = MAROON;
                    PlaySound(fullSfx);
                }
                else
                {
                    dropdice(col, sym);
                    print_board();
                    if (checkWin(sym))
                    {
                        msg = "PLAYER " + to_string(num) + " (" + sym + ") WINS!";
                        msgCol = GREEN;
                        gameOver = true;
                        PlaySound(winSfx);
                    }
                    else if (turn == 72)
                    {
                        msg = "DRAW!";
                        msgCol = YELLOW;
                        gameOver = true;
                        PlaySound(drawSfx);
                    }
                    else
                    {
                        PlaySound(dropSfx);
                        turn++;
                        int nextN = (turn % 2 != 0) ? 1 : 2;
                        msg = "PLAYER " + to_string(nextN) + "'s TURN (" + (nextN == 1 ? "X)" : "O)");
                        msgCol = (nextN == 1) ? Color{ 255, 107, 107, 255 } : GOLD;
                    }
                }
            }
        }

        if (gameOver && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(m, restart))
        {
            for (int c = 0; c < 9; c++) diceplace[c] = 0;
            initialize_board();
            turn = 1;
            gameOver = false;
            msg = "PLAYER 1's TURN (X)";
            msgCol = VIOLET;
        }

        BeginDrawing();
        ClearBackground(Color{ 16, 22, 34, 255 });
        DrawText("CONNECT FIVE", bx, 24, 30, SKYBLUE);
        DrawText(msg.c_str(), bx, 64, 21, msgCol);

        for (int c = 0; c < 9; c++)
        {
            int cx = bx + c * cell + cell / 2;
            DrawText(to_string(c + 1).c_str(), cx - 5, by - 26, 20, (c == hover && !gameOver) ? SKYBLUE : LIGHTGRAY);
            if (c == hover && !gameOver && diceplace[c] < 8)
                DrawCircle(cx, by - 10, 9.0f, Fade(sym == 'X' ? RED : GOLD, 0.7f));
        }

        DrawRectangleRounded({ (float)(bx - 8), (float)(by - 8), (float)(9 * cell + 16), (float)(8 * cell + 16) }, 0.04f, 8, Color{ 30, 62, 120, 255 });
        if (hover != -1 && !gameOver)
            DrawRectangle(bx + hover * cell, by, cell, 8 * cell, Fade(WHITE, 0.08f));

        for (int r = 0; r < 8; r++)
        {
            for (int c = 0; c < 9; c++)
            {
                int cx = bx + c * cell + cell / 2;
                int cy = by + r * cell + cell / 2;
                float rad = cell * 0.37f;
                DrawCircle(cx, cy, rad, Color{ 12, 17, 26, 255 });

                if (board[r][c] == 'X')
                    DrawCircle(cx, cy, rad, RED);
                else if (board[r][c] == 'O')
                    DrawCircle(cx, cy, rad, GOLD);
            }
        }

        if (gameOver)
        {
            DrawRectangleRounded(restart, 0.25f, 8, CheckCollisionPointRec(m, restart) ? SKYBLUE : BLUE);
            DrawText("PLAY AGAIN", (int)restart.x + 34, (int)restart.y + 10, 20, WHITE);
        }

        EndDrawing();
    }

    UnloadSound(dropSfx);
    UnloadSound(fullSfx);
    UnloadSound(winSfx);
    UnloadSound(drawSfx);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}