#include <iostream>
#include <raylib.h>
#include <cmath>
#include <cstdlib>
#include <string>
#include <vector>

using namespace std;

constexpr int ROWS = 8, COLS = 9, CELL = 72;

struct Cell { int r, c; };
struct Confetti { float x, y, vx, vy, size, rot, vrot; Color color; };

char board[ROWS][COLS];
int colHeight[COLS] = { 0 };

void PrintBoard()
{
    system("cls");
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++) 
            cout << board[r][c] << " ";

        cout << endl;
    }
}

void ResetBoard()
{
    for (int c = 0; c < COLS; c++)
        colHeight[c] = 0;

    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++) board[r][c] = '.';

    PrintBoard();
}

bool DropPiece(int col, char sym)
{
    if (col < 0 || col >= COLS || colHeight[col] >= ROWS)
        return false;
    board[ROWS - 1 - colHeight[col]++][col] = sym;
    return true;
}

vector<Cell> GetWinningCells(char sym)
{
    const int dirs[4][2] = { { 0, 1 },{ 1, 0 },{ 1, 1 },{ 1, -1 } };
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            for (auto& d : dirs)
            {
                vector<Cell> line;
                for (int k = 0; k < 5; k++)
                {
                    int nr = r + d[0] * k, nc = c + d[1] * k;
                    if (nr < 0 || nr >= ROWS || nc < 0 || nc >= COLS || board[nr][nc] != sym) break;
                    line.push_back({ nr, nc });
                }
                if (line.size() == 5) return line;
            }
        }
    }
    return {};
}

Sound MakeTone(initializer_list<float> freqs, initializer_list<float> durs, float vol)
{
    constexpr int rate = 44100;
    auto fIt = freqs.begin(), dIt = durs.begin();
    vector<float> pcm;

    for (; fIt != freqs.end() && dIt != durs.end(); ++fIt, ++dIt)
    {
        int n = (int)(*dIt * rate);
        float rel = *dIt * 0.35f;
        for (int i = 0; i < n; i++)
        {
            float t = (float)i / rate;
            float env = (t < 0.015f) ? t / 0.015f : (t > *dIt - rel) ? fmaxf(0.0f, (*dIt - t) / rel) : 1.0f;
            float wave = 0.75f * sinf(2 * PI * (*fIt) * t) + 0.25f * sinf(4 * PI * (*fIt) * t);
            pcm.push_back(wave * env * vol);
        }
    }

    Wave w = { (unsigned int)pcm.size(), rate, 32, 1, malloc(pcm.size() * sizeof(float)) };
    memcpy(w.data, pcm.data(), pcm.size() * sizeof(float));
    Sound s = LoadSoundFromWave(w);
    UnloadWave(w);
    return s;
}

void DrawPiece(int cx, int cy, float radius, char sym)
{
    bool isX = (sym == 'X');
    Color fill = isX ? Color{ 229, 57, 53, 255 } : Color{ 245, 185, 25, 255 };
    Color ring = isX ? Color{ 255, 180, 180, 255 } : Color{ 255, 245, 190, 255 };
    Color textCol = isX ? WHITE : Color{ 25, 20, 10, 255 };

    DrawCircle(cx, cy, radius, fill);
    DrawCircleLines(cx, cy, radius - 4.0f, ring);
    DrawText(isX ? "X" : "O", cx - (isX ? 9 : 10), cy - 12, 26, textCol);
}

int main()
{
    constexpr int W = 940, H = 860;
    constexpr int boardW = COLS * CELL, boardH = ROWS * CELL;
    constexpr int boardX = (W - boardW) / 2, boardY = 148;
    Rectangle restartBtn = { W / 2.0f - 105, H - 64.0f, 210, 44 };

    InitWindow(W, H, "CONNECT FIVE - GUI BASED GAME");
    InitAudioDevice();
    SetTargetFPS(60);

    Sound winSfx = MakeTone({ 523.25f, 659.25f, 783.99f, 1046.5f }, { 0.13f, 0.13f, 0.13f, 0.38f }, 0.35f);
    Sound drawSfx = MakeTone({ 329.63f, 293.66f, 261.63f }, { 0.18f, 0.18f, 0.36f }, 0.30f);
    Sound dropSfx = MakeTone({ 420.0f }, { 0.07f }, 0.18f);
    Sound errSfx = MakeTone({ 190.0f, 150.0f }, { 0.08f, 0.10f }, 0.20f);

    ResetBoard();

    int turn = 1, animRow = -1, animCol = -1;
    float animY = 0, targetY = 0;
    bool gameOver = false, isDropping = false;
    string status = "PLAYER 1's TURN (X)";
    Color statusCol = { 255, 107, 107, 255 };
    vector<Cell> winCells;
    vector<Confetti> confetti;
    Color palette[5] = { RED, GOLD, SKYBLUE, GREEN, MAGENTA };

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime(), now = (float)GetTime();
        int player = (turn % 2) ? 1 : 2;
        char sym = (player == 1) ? 'X' : 'O';
        Vector2 mouse = GetMousePosition();

        if (isDropping && (animY += 2400.0f * dt) >= targetY)
        {
            animY = targetY;
            isDropping = false;
        }

        int hoverCol = -1;
        if (!gameOver && !isDropping && turn <= ROWS * COLS)
        {
            if (mouse.x >= boardX && mouse.x < boardX + boardW && mouse.y >= boardY && mouse.y < boardY + boardH)
                hoverCol = (int)((mouse.x - boardX) / CELL);

            int pick = (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && hoverCol != -1) ? hoverCol : -1;
            for (int k = 0; k < COLS && pick == -1; k++)
                if (IsKeyPressed(KEY_ONE + k)) pick = k;

            if (pick != -1) {
                if (colHeight[pick] >= ROWS) {
                    status = "Column " + to_string(pick + 1) + " is Full. Please Try Again.";
                    statusCol = { 255, 159, 67, 255 };
                    PlaySound(errSfx);
                }
                else {
                    animRow = ROWS - 1 - colHeight[pick];
                    animCol = pick;
                    DropPiece(pick, sym);
                    PrintBoard();
                    PlaySound(dropSfx);

                    targetY = (float)(boardY + animRow * CELL + CELL / 2);
                    animY = (float)(boardY - CELL / 2);
                    isDropping = true;

                    winCells = GetWinningCells(sym);
                    if (!winCells.empty()) {
                        status = "PLAYER " + to_string(player) + " (" + sym + ") WINS!";
                        statusCol = { 72, 219, 132, 255 };
                        gameOver = true;
                        PlaySound(winSfx);
                        confetti.clear();
                        for (int i = 0; i < 90; i++)
                        {
                            confetti.push_back({
                                (float)GetRandomValue(boardX, boardX + boardW),
                                (float)GetRandomValue(-220, 40),
                                (float)GetRandomValue(-90, 90),
                                (float)GetRandomValue(110, 290),
                                (float)GetRandomValue(6, 12),
                                (float)GetRandomValue(0, 360),
                                (float)GetRandomValue(-220, 220),
                                palette[i % 5]
                                });
                        }
                    }
                    else if (turn == ROWS * COLS)
                    {
                        status = "DRAW GAME!";
                        statusCol = { 255, 214, 102, 255 };
                        gameOver = true;
                        PlaySound(drawSfx);
                    }
                    else {
                        int nextP = (++turn % 2) ? 1 : 2;
                        char nextS = (nextP == 1) ? 'X' : 'O';
                        status = "PLAYER " + to_string(nextP) + "'s TURN (" + nextS + ")";
                        statusCol = (nextP == 1) ? Color{ 255, 107, 107, 255 } : Color{ 255, 212, 59, 255 };
                    }
                }
            }
        }

        for (auto& p : confetti)
        {
            p.x += p.vx * dt; p.y += p.vy * dt; p.rot += p.vrot * dt;
            if (p.y > H + 20)
                p.y = -15.0f; p.x = (float)GetRandomValue(boardX, boardX + boardW);

        }

        if (gameOver && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, restartBtn))
        {
            ResetBoard();
            turn = 1; animRow = animCol = -1;
            gameOver = isDropping = false;
            winCells.clear(); confetti.clear();
            status = "PLAYER 1's TURN (X)";
            statusCol = { 255, 107, 107, 255 };
        }

        BeginDrawing();
        ClearBackground({ 14, 19, 28, 255 });

        DrawRectangleGradientH(0, 0, W, 104, { 22, 31, 48, 255 }, { 16, 22, 34, 255 });
        DrawLine(0, 104, W, 104, { 52, 78, 122, 255 });
        DrawText("CONNECT FIVE", boardX, 18, 32, SKYBLUE);
        DrawText(status.c_str(), boardX, 58, 22, statusCol);

        Rectangle p1 = { (float)(boardX + boardW - 225), 22, 105, 32 };
        Rectangle p2 = { (float)(boardX + boardW - 110), 22, 110, 32 };
        bool p1On = (!gameOver && player == 1), p2On = (!gameOver && player == 2);
        DrawRectangleRounded(p1, 0.35f, 8, p1On ? Color{ 220, 53, 69, 255 } : Color{ 32, 42, 60, 255 });
        DrawRectangleRoundedLines(p1, 0.35f, 8, p1On ? WHITE : Color{ 70, 88, 118, 255 });
        DrawText("P1 (X)", (int)p1.x + 22, (int)p1.y + 7, 18, WHITE);

        DrawRectangleRounded(p2, 0.35f, 8, p2On ? Color{ 230, 175, 25, 255 } : Color{ 32, 42, 60, 255 });
        DrawRectangleRoundedLines(p2, 0.35f, 8, p2On ? BLACK : Color{ 70, 88, 118, 255 });
        DrawText("P2 (O)", (int)p2.x + 24, (int)p2.y + 7, 18, p2On ? BLACK : RAYWHITE);

        for (int c = 0; c < COLS; c++)
        {
            int cx = boardX + c * CELL + CELL / 2;
            bool hov = (c == hoverCol && !gameOver);
            DrawText(to_string(c + 1).c_str(), cx - 6, boardY - 30, 20, hov ? SKYBLUE : Color{ 148, 163, 184, 255 });
            if (hov && colHeight[c] < ROWS)
                DrawCircle(cx, boardY - 12, 11.0f, Fade(sym == 'X' ? RED : GOLD, 0.65f));
        }
        if (hoverCol != -1 && !gameOver)
            DrawRectangle(boardX + hoverCol * CELL, boardY, CELL, boardH, Fade(SKYBLUE, 0.09f));

        Rectangle frame = { (float)(boardX - 10), (float)(boardY - 10), (float)(boardW + 20), (float)(boardH + 20) };
        DrawRectangleRounded(frame, 0.04f, 12, { 28, 58, 115, 255 });
        DrawRectangleRoundedLines(frame, 0.04f, 12, { 88, 145, 235, 255 });

        float rad = CELL * 0.38f;
        for (int r = 0; r < ROWS; r++)
        {
            for (int c = 0; c < COLS; c++)
            {
                int cx = boardX + c * CELL + CELL / 2, cy = boardY + r * CELL + CELL / 2;
                DrawCircle(cx + 2, cy + 3, rad, { 8, 12, 18, 255 });
                DrawCircle(cx, cy, rad, { 13, 19, 30, 255 });
                DrawCircleLines(cx, cy, rad, { 38, 64, 108, 255 });

                if ((isDropping && r == animRow && c == animCol) || (board[r][c] != 'X' && board[r][c] != 'O')) continue;

                bool win = false;
                for (auto& wc : winCells) if (wc.r == r && wc.c == c) { win = true; break; }
                float drawR = win ? rad * (1.0f + 0.08f * sinf(now * 8.0f)) : rad;
                if (win) DrawCircle(cx, cy, drawR + 6.0f, Fade(WHITE, 0.35f + 0.25f * sinf(now * 8.0f)));
                DrawPiece(cx, cy, drawR, board[r][c]);
            }
        }

        if (isDropping && animRow >= 0 && animCol >= 0)
            DrawPiece(boardX + animCol * CELL + CELL / 2, (int)animY, rad, board[animRow][animCol]);

        for (auto& p : confetti)
            DrawRectanglePro({ p.x, p.y, p.size, p.size * 0.65f }, { p.size * 0.5f, p.size * 0.32f }, p.rot, p.color);

        if (gameOver)
        {
            bool hovR = CheckCollisionPointRec(mouse, restartBtn);
            DrawRectangleRounded(restartBtn, 0.3f, 10, hovR ? Color{ 56, 189, 248, 255 } : Color{ 37, 99, 235, 255 });
            DrawRectangleRoundedLines(restartBtn, 0.3f, 10, WHITE);
            DrawText("PLAY AGAIN", (int)restartBtn.x + 42, (int)restartBtn.y + 12, 20, hovR ? BLACK : WHITE);
        }

        EndDrawing();
    }

    UnloadSound(winSfx); UnloadSound(drawSfx); UnloadSound(dropSfx); UnloadSound(errSfx);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}