/* Alex Perrino #100251503
*   CPSC1160 SEC 001
*   Name: FiveStones.cpp
*   A game which is played by one human player against the computer. Players take turns
*   placing either an X or O stone onto a 9x9 board, aiming to make a row that is at least
*   5 stones long either vertically, horizontally or diagonally. Once a stone is placed it
*   cannot be moved or removed, and if all spaces are in use and there is no winner it is
*   declared a tie.
*/
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

const int COLUMN_SIZE = 9;
const int BOARD_SPACES = COLUMN_SIZE * COLUMN_SIZE;

//Function Headers
void initializeBoard(char a[][COLUMN_SIZE]);
void displayBoard(const char a[][COLUMN_SIZE]);
void drawLine();
void drawColumns();
bool findSequence(char a[][COLUMN_SIZE], int move[], int length);
bool horizontalSearch(char a[][COLUMN_SIZE], int move[], int length);
bool verticalSearch(char a[][COLUMN_SIZE], int move[], int length);
bool diagonalSearch1(char a[][COLUMN_SIZE], int move[], int length);
bool diagonalSearch2(char a[][COLUMN_SIZE], int move[], int length);
void makeMove(char a[][COLUMN_SIZE], int moveList[]);
void compMove(char a[][COLUMN_SIZE], int moveList[]);
void randomMove(char a[][COLUMN_SIZE], int moveList[]);
void blockPotentialWins(char a[][COLUMN_SIZE], int moveList[]);
bool isValidMove(char a[][COLUMN_SIZE], int moveList[], char flag);
void playGame(char a[][COLUMN_SIZE]);

int main()  {
   char gameBoard[9][9];
   initializeBoard(gameBoard);
   playGame(gameBoard);

   return 0;
}

//Function which is used to draw the current state of the game board.
void displayBoard(const char a[][COLUMN_SIZE])  {
   drawColumns();
   drawLine();
   for (int i = 0; i < COLUMN_SIZE; i++)  {
      cout << i + 1 << " |";
      for (int j = 0; j < COLUMN_SIZE; j++)  {
         cout << " " << a[i][j] << " |";
      }
      cout << endl;
      drawLine();
   }
}

//Helper Function to the displayBoard function which helps to draw the grid.
void drawLine()   {
   cout << "  -------------------------------------" << endl;
}

//Helper Function to the displayBoard function which helps to draw the colum headers.
void drawColumns()   {
   cout << "   ";
   for (int i = 0; i < COLUMN_SIZE; i++)
      cout << " " << char(i + 'A') << "  ";
   cout << endl;
}

//Used to initialize the board to its default values.
void initializeBoard(char a[][COLUMN_SIZE])  {
   for (int i = 0; i < COLUMN_SIZE; i++)  {
      for (int j = 0; j < COLUMN_SIZE; j++)  
         a[i][j] = ' ';
   }
}

//Takes a user's move in the format "A1" and places the x symbol on the corresponding place.
//This is only done if the space is available and is not already taken. The function also updates
//the last made move.
void makeMove(char a[][COLUMN_SIZE], int moveList[])   {
   string move;
   do {
      cout << "Input a move (eg. G7): ";
      cin >> move;
      if (move.compare("QUIT") == 0)
         exit(0);
      moveList[1] = move[0] - 'A';  //Column
      char tmp[1] = {move[1]};
      moveList[0] = atoi(tmp) - 1;  //Row
   } while (!isValidMove(a, moveList, 'H'));
   a[moveList[0]][moveList[1]] = 'x';
   cout << "Player places token at: " << char(moveList[1] + 'A')
      << moveList[0] + 1 << endl;
}

//Randomly generates a move for the computer and tests to see whether it is a valid move.
//This function keeps generating until it is valid and then makes the move.
void randomMove(char a[][COLUMN_SIZE], int moveList[])  {
   srand (time(NULL));
   do {
      moveList[1] = rand() % 9;
      moveList[0] = rand() % 9;
   } while (!isValidMove(a, moveList, 'C'));
   a[moveList[0]][moveList[1]] = 'o';
   cout << "Computer places token at: " << char(moveList[1] + 'A')
      << moveList[0] + 1 << endl;
}

//More or less a helper function which states where a computer's move is being made and
//makes the move
void compMove(char a[][COLUMN_SIZE], int moveList[])  {
   a[moveList[0]][moveList[1]] = 'o';
   cout << "Computer places token at: " << char(moveList[1] + 'A')
      << moveList[0] + 1 << endl;
}

//Checks to see whether a given move is valid or not. A move is said to be valid if the space in
//question is not already taken and is within the bounds (A-I, and 1-9). The flag variable is used
//to toggle whether error messages are displayed (only displayed for humans, not the computer).
bool isValidMove(char a[][COLUMN_SIZE], int moveList[], char flag)  {
   int row = moveList[0];
   char col = char(moveList[1] + 'A');
   if (row < 0 || col < 'A' || col > char(COLUMN_SIZE - 1 + 'A') ) {
      if (flag == 'H')
         cout << "ERROR: Space is out of bounds." << endl;
      return false;      
   }
   if (a[row][moveList[1]] != ' ') {
      if (flag == 'H')
         cout << "ERROR: Space is already taken." << endl;
      return false;      
   }
   return true;
}

//This function uses all the searching functions (horizontal, vertical, and both diagonals) starting with a length of 4. 
//If the human player doesn’t have a sequence of 4 the loop moves to 3 and so on until it finds a sequence that it can
//block (will continue searching if it finds a sequence but the spaces on either side of it are already taken). This keeps
//going until it checks all sequences of 1 and since the human always goes first the computer will always place next to
//the human. If in some rare cases no move can be made adjacent to the human's move, a random move will be
//generated and used instead. The function uses modified versions of the already implemented search functions to
//make the moves.
void blockPotentialWins(char a[][COLUMN_SIZE], int moveList[])   {
   int potentialMove[] = {0, 0};
   char symbol = a[moveList[0]][moveList[1]];
   for (int i = 4; i >= 1; i--)   {
      if (horizontalSearch(a, moveList, i))  {   //Horizontal Search
         potentialMove[0] = moveList[0];
         int col = moveList[1];
         while (col >= 0 && a[moveList[0]][col - 1] == symbol)
            col--;
         potentialMove[1] = col - 1;
         if (isValidMove(a, potentialMove, 'C'))   //Place to left
            break;
         col = moveList[1];
         while (col <= COLUMN_SIZE - 1 && a[moveList[0]][col + 1] == symbol)
            col++;
         potentialMove[1] = col + 1;
         if (isValidMove(a, potentialMove, 'C'))   //Place to right
            break;
      }
      if (verticalSearch(a, moveList, i)) {  //Vertical Search
         potentialMove[1] = moveList[1];
         int row = moveList[0];
         while (row >= 0 && a[row - 1][moveList[1]] == symbol)
            row--;
         potentialMove[0] = row - 1;
         if (isValidMove(a, potentialMove, 'C'))   //Place  up
            break;
         row = moveList[0];
         while (row <= COLUMN_SIZE - 1 && a[row + 1][moveList[1]] == symbol)
            row++;
         potentialMove[0] = row + 1;
         if (isValidMove(a, potentialMove, 'C'))//Place down
            break;
      }
      if (diagonalSearch1(a, moveList, i))   {  //Diagonal Search (Upper Left - Lower Right)
         int row = moveList[0];
         int col = moveList[1];
         while (row >= 0 && col >= 0 && a[row - 1][col - 1] == symbol)   {
            row--;
            col--;
         }
         potentialMove[0] = row - 1;
         potentialMove[1] = col - 1;
         if (isValidMove(a, potentialMove, 'C'))   //Place to upper left
            break;
         row = moveList[0];
         col = moveList[1];
         while (row <= COLUMN_SIZE - 1 && col <= COLUMN_SIZE 
            && a[row + 1][col + 1] == symbol)   {
            row++;
            col++;
         }
         potentialMove[0] = row + 1;
         potentialMove[1] = col + 1;
         if (isValidMove(a, potentialMove, 'C'))   //Place to lower right
            break;
      }
      if (diagonalSearch2(a, moveList, i))   {  //Diagonal Search (Lower Left - Upper Right)
         int row = moveList[0];
         int col = moveList[1];
         while (row >= 0 && col >= 0 && a[row + 1][col - 1] == symbol)   {
            row++;
            col--;
         }
         potentialMove[0] = row + 1;
         potentialMove[1] = col - 1;
         if (isValidMove(a, potentialMove, 'C'))   //Place to lower left
            break;
         row = moveList[0];
         col = moveList[1];
         while (row <= COLUMN_SIZE - 1 && col <= COLUMN_SIZE 
            && a[row - 1][col + 1] == symbol)   {
            row--;
            col++;
         }
         potentialMove[0] = row - 1;
         potentialMove[1] = col + 1;
         if (isValidMove(a, potentialMove, 'C'))   //Place to upper right
            break;
      }
   }
   moveList[0] = potentialMove[0];
   moveList[1] = potentialMove[1];
   if (isValidMove(a, potentialMove, 'C'))
      compMove(a, potentialMove);
   else
      randomMove(a, potentialMove);
}

//Given a space on the board, searches in all directions for a sequence of the given length.
//Returns true if there is a sequence, false otherwise.
bool findSequence(char a[][COLUMN_SIZE], int move[], int length) {
   if (horizontalSearch(a, move, length))   //Horizontal Search
      return true;
   if (verticalSearch(a, move, length))   //Vertical Search
      return true;
   if (diagonalSearch1(a, move, length))   //Diagonal Search (Upper Left - Lower Right)
      return true;
   if (diagonalSearch2(a, move, length))   //Diagonal Search (Lower Left - Upper Right)
      return true;
   return false;
}

//Given a space on the board, searches horizontally going left from the starting point and then right.
//This will return true if a sequence of the given length is found.
bool horizontalSearch(char a[][COLUMN_SIZE], int move[], int length) {
   int currentSeq = 1;
   int col = move[1];
   char symbol = a[move[0]][move[1]];
   while (col >= 0 && a[move[0]][col - 1] == symbol)   {
      currentSeq++;
      col--;
   }
   col = move[1];
   while (col <= COLUMN_SIZE - 1 && a[move[0]][col + 1] == symbol)   {
      currentSeq++;
      col++;
   }
   if (currentSeq >= length)
      return true;
   return false;
}

//Given a space on the board, searches vertically going up from the starting point and then down.
//This will return true if a sequence of the given length is found.
bool verticalSearch(char a[][COLUMN_SIZE], int move[], int length) {
   int currentSeq = 1;
   int row = move[0];
   char symbol = a[move[0]][move[1]];
   while (row >= 0 && a[row - 1][move[1]] == symbol)   {
      currentSeq++;
      row--;
   }
   row = move[0];
   while (row <= COLUMN_SIZE - 1 && a[row + 1][move[1]] == symbol)   {
      currentSeq++;
      row++;
   }
   if (currentSeq >= length)
      return true;
   return false;
}

//Given a space on the board, searches diagonally going upper left from the starting point and to lower right.
//This will return true if a sequence of the given length is found.
bool diagonalSearch1(char a[][COLUMN_SIZE], int move[], int length) {
   int currentSeq = 1;
   int row = move[0];
   int col = move[1];
   char symbol = a[row][col];
   while (row >= 0 && col >= 0 && a[row - 1][col - 1] == symbol)   {
      currentSeq++;
      row--;
      col--;
   }
   row = move[0];
   col = move[1];
   while (row <= COLUMN_SIZE - 1 && col <= COLUMN_SIZE 
      && a[row + 1][col + 1] == symbol)   {
      currentSeq++;
      row++;
      col++;
   }
   if (currentSeq >= length)
      return true;
   return false;
}

//Given a space on the board, searches diagonally going lower left from the starting point and to upper right.
//This will return true if a sequence of the given length is found.
bool diagonalSearch2(char a[][COLUMN_SIZE], int move[], int length) {
   int currentSeq = 1;
   int row = move[0];
   int col = move[1];
   char symbol = a[row][col];
   while (row >= 0 && col >= 0 && a[row + 1][col - 1] == symbol)   {
      currentSeq++;
      row++;
      col--;
   }
   row = move[0];
   col = move[1];
   while (row <= COLUMN_SIZE - 1 && col <= COLUMN_SIZE 
      && a[row - 1][col + 1] == symbol)   {
      currentSeq++;
      row--;
      col++;
   }
   if (currentSeq >= length)
      return true;
   return false;
}

//Function that manages the game. Makes the moves for both players and determines whether there is
//a win or tie.
void playGame(char a[][COLUMN_SIZE])   {
   int winFlag = -1;
   int movesMade = 0;
   int lastMove[] = {0, 0};
   displayBoard(a);
   while (winFlag == -1)   {
      if (movesMade == BOARD_SPACES)   {
         winFlag = 2;
         break;
      }
      makeMove(a, lastMove);
      if (findSequence(a, lastMove, 5))  {
         winFlag = 1;
         break;
      }
      movesMade++;
      if (movesMade == BOARD_SPACES)   {
         winFlag = 2;
         break;
      }
      blockPotentialWins(a, lastMove);
      if (findSequence(a, lastMove, 5))  {
         winFlag = 0;
         break;
      }
      displayBoard(a);
      movesMade++;
   }
   if (winFlag == 2)
      cout << "No more moves can be made; it is a tie." << endl;
   if (winFlag == 1)
      cout << "Human Player has won the match." << endl;
   if (winFlag == 0)
      cout << "Computer Player has won the match." << endl;
   displayBoard(a);
}
