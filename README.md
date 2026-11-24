CONNECT FIVE :

PROJECT PURPOSE AND OVERVIEW : 

Connect Five is a two-player strategy game. The board is 8 rows x 9 columns (72 cells). Player 1 plays X (Red) and Player 2 plays O (Yellow). Players take turns dropping a piece into a column. The piece falls to the lowest empty cell. The first player to get 5 in a row horizontally, vertically, or diagonally wins. If all 72 cells are filled with no winner, the game is a Draw. The game shows the turn, highlights the winning 5 cells, plays sounds, shows confetti, and has a Play Again button.

 PREREQUISITES :
 
 Windows 10 / 11, 64-bit MinGW g++ compiler, C++,Visual Studio Code,Raylib library,Basic C++ knowledge: Arrays, Loops, If-Else, Functions
 
 INSTALLATION AND SETUP :
 
 Install VS Code from code.visualstudio.com,Install MinGW and add its bin folder to Windows PATHDownload Raylib for Windows + MinGW from raylib.com
 Create project folder AND Install C/C++ Extension in VS Code.
 
 Steps to Compile and Run on a New PC :
 
 Open the project folder in VS CodeOpen Terminal: Ctrl + Compile:bashg++ main.cpp -o ConnectFive -I raylib/include -L raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm
 FOR RUN : .\ConnectFive.exe
 
 Configuration / Environment Setup :
 
 Window size is fixed: 940 x 860, 60 FPSBoard is char board[8][9] - . = empty, X / O = filleddiceplace[9] tracks how many pieces are in each columnAudio is created by code using InitAudioDevice(). No external sound files are needed.If using DLL version, keep raylib.dll in the same folder as the .exeUse 64-bit Raylib with 64-bit MinGW. 
 
 Troubleshooting Common Issues :
 
 1. raylib.h not found: Include path is wrong. Check -I raylib/include.
 2. cannot find -lraylib: Lib path is wrong. Check -L raylib/lib and libraylib.a.
 3. raylib.dll not found: Copy DLL from Raylib bin folder to the .exe folder.
 4. g++ not recognized: MinGW is not in PATH. Add it and restart VS Code.
 5. Click does nothing: Either the column is full, or the game is over. Try another column or click Play Again.
 6. No sound: Check PC volume. Sounds are generated in code, no sound files are required.
 7. Piece does not drop instantly: This is the drop animation. Wait for it to finish. New clicks are blocked during animation by isDropping.
 8. Win not detected: Do not change board indexing. It is board[8][9] - row 0-7, column 0-8. checkWin() checks all 4 directions.
