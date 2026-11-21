#include <iostream>
#include <cstdlib>
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
}

bool dropdice(int col, char symbol)
{
    if (col < 0 || col > 8) return false;
    if (diceplace[col] == 8) return false;
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
    char symbol = (num == 1) ? 'X' : 'O';

    while (true)
    {
        cout << "PLAYER " << num << " (" << symbol << ") TURN!\n";
        do
        {
            cout << "Enter column (1-9): ";
            cin >> col;
            col = col - 1;
            if ((col < 9) || (col > -1))
                break;
            else
            {
                cout << "Invalid Column.";
                cout << "\nRe-";
                continue;
            }
        if (dropdice(col, symbol)) 
            break;
        cout << "Column has Fulled.\nRe-";
        } while (true);
    }
}

int main()
{
    initialize_board();
    print_board();

    for (int i = 1; i <= 72; i++)
    {
        int num = (i % 2 != 0) ? 1 : 2;
        Player(num);
        system("cls");
        print_board();

        char sym = (num == 1) ? 'X' : 'O';
        if (checkWin(sym))
        {
            cout << "\nPLAYER " << num << " (" << sym << ") WINS!\n";
            break;
        }
        if (i == 72)
        {
            cout << "\nDRAW! Board Full!\n";
        }
    }
    return 0;
}