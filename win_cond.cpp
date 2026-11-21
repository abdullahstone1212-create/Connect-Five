//bool checkWin(char symbol)
//{
//    //  Horizontal
//    for (int r = 0; r < 8; r++)
//    {
//        for (int c = 0; c <= 4; c++)
//        {
//            if (board[r][c] == symbol && board[r][c + 1] == symbol && board[r][c + 2] == symbol && board[r][c + 3] == symbol && board[r][c + 4] == symbol)
//            {
//                return true;
//            }
//        }
//    }
//    //  Vertical
//    for (int r = 0; r <= 3; r++)
//    {
//        for (int c = 0; c < 9; c++)
//        {
//            if (board[r][c] == symbol && board[r + 1][c] == symbol && board[r + 2][c] == symbol && board[r + 3][c] == symbol && board[r + 4][c] == symbol)
//            {
//                return true;
//            }
//        }
//    }
//    //  Right Diagonal 
//    for (int r = 0; r <= 3; r++)
//    {
//        for (int c = 0; c <= 4; c++)
//        {
//            if (board[r][c] == symbol && board[r + 1][c + 1] == symbol && board[r + 2][c + 2] == symbol && board[r + 3][c + 3] == symbol && board[r + 4][c + 4] == symbol)
//            {
//                return true;
//            }
//        }
//    }
//    //  Left Diagonal 
//    for (int r = 0; r <= 3; r++)
//    {
//        for (int c = 4; c < 9; c++)
//        {
//            if (board[r][c] == symbol && board[r + 1][c - 1] == symbol && board[r + 2][c - 2] == symbol && board[r + 3][c - 3] == symbol && board[r + 4][c - 4] == symbol)
//            {
//                return true;
//            }
//        }
//    }
//    return false;
//}