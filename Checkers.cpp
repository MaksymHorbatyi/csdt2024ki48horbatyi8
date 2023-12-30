#include <iostream>
#include <windows.h>

using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

enum ConsoleColor {
    Black,
    Blue,
    Green,
    Cyan,
    Red,
    Magenta,
    Brown,
    LightGray,
    DarkGray,
    LightBlue,
    LightGreen,
    LightCyan,
    LightRed,
    LightMagenta,
    Yellow,
    White
};

void setColor(unsigned fg, unsigned bg) {
    SetConsoleTextAttribute(hConsole, (WORD)((bg << 4) | fg));
}

char board[8][8] =
{
    {' ', 'b', ' ', 'b', ' ', 'b', ' ', 'b'},
    {'b', ' ', 'b', ' ', 'b', ' ', 'b', ' '},
    {' ', 'b', ' ', 'b', ' ', 'b', ' ', 'b'},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'r', ' ', 'r', ' ', 'r', ' ', 'r', ' '},
    {' ', 'r', ' ', 'r', ' ', 'r', ' ', 'r'},
    {'r', ' ', 'r', ' ', 'r', ' ', 'r', ' '},
};

char turn = 'B';
bool leap;
bool game_running = true;
int row1, row2, column1, column2;

void display_board();
void input();
bool check_move();
void move();
void do_leap();
void king();
void game_over();

int main()
{
    cout << "---CHECKERS GAME---\n";
    cout << "Player 1 [b]\n";
    cout << "Player 2 [r]\n\n";
    cout << "A capital letter represents a king piece.\n";
    cout << "Please resize your command-line window so that this line fits on the screen:\n";
    cout << "_____________________________________________________________________________________\n\n";
    cout << "Press enter to begin...";
    cin.get(); // Wait for the user to press enter before continuing

    // The game loop
    while (game_running)
    {
        display_board();

        if (turn == 'B')
        {
            cout << "--Player 1 [B]--\n";
        }
        else if (turn == 'R')
        {
            cout << "--Player 2 [R]--\n";
        }

        input();
        system("cls");
        move();
        king();
        game_over();
    }

    // Display the winner at the end
    if (turn == 'B')
    {
        cout << endl << endl << "Player 2 [Red] wins!\n";
    }
    else if (turn == 'R')
    {
        cout << endl << endl << "Player 1 [Black] wins!\n";
    }

    cout << "GAME OVER!\n";
}

// Display the checkers board
void display_board()
{
    system("cls");
    cout << "    0   1   2   3   4   5   6   7\n";;
    cout << "  +---+---+---+---+---+---+---+---+\n";

    for (int i = 0; i < 8; i++)
    {
        cout << i << " |";
        for (int j = 0; j < 8; j++)
        {
            if (board[i][j] == 'b' || board[i][j] == 'B')
            {
                setColor(Blue, Black);
                cout << " " << board[i][j];
                setColor(White, Black);
                cout << " |";
            }
            else
            {
                setColor(Red, Black);
                cout << " " << board[i][j];
                setColor(White, Black);
                cout << " |";
            }
        }

        cout << endl;
        cout << "  +---+---+---+---+---+---+---+---+\n";
    }
    cout << endl;
}

// Takes in user input
void input()
{   
    cout << "Move piece\n";
    cout << "Row: ";
    cin >> row1;

    while(cin.fail() || row1 < 0 || row1 > 7)
    {
        cin.clear();
        while (cin.get() != '\n');
        cout << "Incorrect input. Enter numbers between 0 and 7.\n";
        cout << "Row: ";
        cin >> row1;
    }

    cout << "Column: ";
    cin >> column1;

    while (cin.fail() || column1 < 0 || column1 > 7)
    {
        cin.clear();
        while (cin.get() != '\n');
        cout << "Incorrect input. Enter numbers between 0 and 7.\n";
        cout << "Column: ";
        cin >> column1;
    }

    cout << "To box\n";
    cout << "Row: ";
    cin >> row2;

    while (cin.fail() || row2 < 0 || row2 > 7)
    {
        cin.clear();
        while (cin.get() != '\n');
        cout << "Incorrect input. Enter numbers between 0 and 7.\n";
        cout << "Row: ";
        cin >> row2;
    }

    cout << "Column: ";
    cin >> column2;

    while (cin.fail() || column2 < 0 || column2 > 7)
    {
        cin.clear();
        while (cin.get() != '\n');
        cout << "Incorrect input. Enter numbers between 0 and 7.\n";
        cout << "Column: ";
        cin >> column2;
    }

    while (check_move() == false)
    {
        cout << "ILLEGAL MOVE!\n";
        input();
    }
}

// Checks if a move is legal
bool check_move()
{
    // Checks if a non-king piece is moving backwards
    if (board[row1][column1] != 'B' && board[row1][column1] != 'R')
    {
        if ((turn == 'B' && row2 < row1) || (turn == 'R' && row2 > row1))
        {
            leap = false;
            return false;
        }
    }

    // Checks if the location the piece is moving to is already taken
    if (board[row2][column2] != ' ')
    {
        leap = false;
        return false;
    }

    // Checks if location entered by the user contains a piece to be moved
    if (board[row1][column1] == ' ')
    {
        leap = false;
        return false;
    }

    // Checks if the piece isn't moving diagonally
    if (column1 == column2 || row1 == row2)
    {
        leap = false;
        return false;
    }

    // Checks if the piece is moving by more than 1 column and only 1 row
    if ((column2 > column1 + 1 || column2 < column1 - 1) && (row2 == row1 + 1 || row2 == row1 - 1))
    {
        leap = false;
        return false;
    }

    // Checks if the piece is leaping
    if (row2 > row1 + 1 || row2 < row1 - 1)
    {
        // Checks if the piece is leaping too far
        if (row2 > row1 + 2 || row2 < row1 - 2)
        {
            leap = false;
            return false;
        }

        // Checks if the piece isn't moving by exactly 2 columns
        if (column2 != column1 + 2 && column2 != column1 - 2)
        {
            leap = false;
            return false;
        }

        // Checks if the piece is leaping over another piece from the other player
        char otherPlayer;
        if (toupper(board[row1][column1]) == 'R') {
            otherPlayer = 'B';
        }
        else {
            otherPlayer = 'R';
        }

        if (row2 > row1 && column2 > column1)
        {
            if (toupper(board[row2 - 1][column2 - 1]) != otherPlayer)
            {
                leap = false;
                return false;
            }
        }
        else if (row2 > row1 && column2 < column1)
        {
            if (toupper(board[row2 - 1][column2 + 1]) != otherPlayer)
            {
                leap = false;
                return false;
            }
        }
        else if (row2 < row1 && column2 > column1)
        {
            if (toupper(board[row2 + 1][column2 - 1]) != otherPlayer)
            {
                leap = false;
                return false;
            }
        }
        else if (row2 < row1 && column2 < column1)
        {
            if (toupper(board[row2 + 1][column2 + 1]) != otherPlayer)
            {
                leap = false;
                return false;
            }
        }

        leap = true;
        return true;
    }

    leap = false;
    return true;
}

// Performs a move
void move()
{
    bool king_piece = false;

    if (board[row1][column1] == 'B' || board[row1][column1] == 'R')
    {
        king_piece = true;
    }

    board[row1][column1] = ' ';

    if (turn == 'B')
    {
        if (king_piece == false)
        {
            board[row2][column2] = 'b';
        }
        else if (king_piece == true)
        {
            board[row2][column2] = 'B';
        }

        turn = 'R';
    }
    else if (turn == 'R')
    {
        if (king_piece == false)
        {
            board[row2][column2] = 'r';
        }
        else if (king_piece == true)
        {
            board[row2][column2] = 'R';
        }

        turn = 'B';
    }

    if (leap == true)
    {
        do_leap();
    }
}

// Performs a leap over a checkers piece
void do_leap()
{
    char answer;

    // Removes the checker piece after leap
    if (row2 > row1 && column2 > column1)
    {
        board[row2 - 1][column2 - 1] = ' ';
    }
    else if (row2 > row1 && column2 < column1)
    {
        board[row2 - 1][column2 + 1] = ' ';
    }
    else if (row2 < row1 && column2 > column1)
    {
        board[row2 + 1][column2 - 1] = ' ';
    }
    else if (row2 < row1 && column2 < column1)
    {
        board[row2 + 1][column2 + 1] = ' ';
    }

    display_board(); // Displays the board after the changes

    // Asks if the user wants to leap again
    do
    {
        cout << "You just leaped once. Do you want to try a second (y/n): ";
        cin >> answer;
    } while (answer != 'Y' && answer != 'y' && answer != 'N' && answer != 'n');

    if (answer == 'y' || answer == 'Y')
    {
        row1 = row2;
        column1 = column2;
        cout << "Leap piece: row: " << row1 << ", column: " << column1 << endl;
        cout << "To box\n";
        cout << "row: ";
        cin >> row2;
        cout << "column: ";
        cin >> column2;

        while (row2 < 0 || row2 > 7 || column2 < 0 || column2 > 7)
        {
            cout << "Incorrect input. Enter numbers between 0 and 7.\n";
            cout << "To box\n";
            cout << "Row: ";
            cin >> row2;
            cout << "Column: ";
            cin >> column2;
        }

        if (turn == 'B')
        {
            turn = 'R';
        }
        else if (turn == 'R')
        {
            turn = 'B';
        }

        check_move();

        if (leap == false)
        {
            cout << "INVALID LEAP!\n";

            if (turn == 'B')
            {
                turn = 'R';
            }
            else if (turn == 'R')
            {
                turn = 'B';
            }
        }
        else if (leap == true)
        {
            move();
        }
    }
}

// Converts regular checkers pieces that have reached the other side of the board to king pieces
void king()
{
    for (int i = 0; i < 8; i++)
    {
        if (board[0][i] == 'r')
        {
            board[0][i] = 'R';
        }

        if (board[7][i] == 'b')
        {
            board[7][i] = 'B';
        }
    }
}

// Determins if the game is over
void game_over()
{
    int counter = 0;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (board[i][j] != ' ')
            {
                counter++;
            }
        }
    }

    if (counter > 1)
    {
        game_running = true;
    }
    else if (counter == 1)
    {
        game_running = false;
    }
}