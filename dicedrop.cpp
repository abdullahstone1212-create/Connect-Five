//#include <iostream>
//using namespace std;
//
//char board[8][9];
//int diceplace[9] = { 0,0,0,0,0,0,0,0,0 };
//
//bool dropdice(int col, char symbol)
//{
//    if (col < 0 || col > 8)
//    return false;
//    if (diceplace[col] == 8)
//    return false;
//    int emprow = 7 - diceplace[col];
//    board[emprow][col] = symbol;
//    diceplace[col]++;
//    return true;
//}