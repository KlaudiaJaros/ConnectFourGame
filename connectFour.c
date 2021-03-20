/*
    connectFour.exe - version 1.0 - author: Klaudia Jaros, 40439268

  ConnectFour -  a command line game written in C based on the popular Connect Four
  board game. The application allows the user to choose between single player and
  multiplayer mode, adjust the board size and view previous games.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>


struct Player {
    char* name;
    int winner;
    char token;
};

struct Board {
    char* board;
    char* col1;

};

void populateColumns(char* col, char* board) {

    int idx = 0;
    for (int i = 35; i > 0; i - 7) {
        col[idx] = board[i];
        idx++;
    }

}

/*
  printMenu() - a void function that prints the application's menu
*/
void printMenu() {

    printf("\n ----------------------------");
    printf("\n\t   MENU \n");
    printf(" ----------------------------\n\n");
    printf(" Start a New Game: \n\n");
    printf(" 1 - Single Player \n");
    printf(" 2 - Multiplayer \n");
    printf("\n ...or choose an option below:\n\n");
    printf(" 3 - View game history\n");
    printf(" 4 - Settings\n");
    printf(" 5 - Exit\n");
    printf(" ----------------------------\n");
}

/*
  initBoard() - initialises the board for the game
*/
char* initBoard() {
    char* board = (char*)malloc(sizeof(char) * 42);
    for (int i = 0; i < 42; i++)
    {
        board[i] = ' ';
    }
    return board;
}

/*
  printBoard() - prints the current state of the board
*/
void printBoard(char* board)
{
    int count = 0;

    printf("\n");
    // print the header for column numbers:
    for (int i = 1; i < 8; i++)
    {
        printf(" %d  ", i);
    }
    printf(" column\n");

    // print the board:
    for (int i = 0; i < 42; i++)
    {
        printf(" %c |", board[i]);
        count++;
        if (count == 7) {
            printf("\n");
            count = 0;
        }
    }
    printf("\n");
}

void settings() {
    printf(" Settings: \n");
}

void gameHistory() {
    printf(" Game History: \n");
}


int movesLeft(char* board) {
    int len = 42;
    int spaceLeft = 0;
    for (int i = 0; i < len; i++) {
        if (board[i] == ' ') {
            spaceLeft = 1;
            break;
        }
    }
    return spaceLeft;
}

char checkWin(char* board) {

    int xCount = 0;
    int oCount = 0;

    // check columns:

    // first loop: iterate through the number of columns (7):
    for (int column = 0; column < 7; column++) {
        // second loop: start from the bottom of column 1 (index 35) because it's most likely to find 4 starting from the bottom
        // add i to move to the next column, n>0 because the last index is 0, n-7 because there are 7 columns
        for (int idx = 35 + column; idx > 0; idx = idx - 7) {

            // if empty, continue:
            if (board[idx] == ' ') {
                xCount = 0;
                oCount = 0;
                continue;
            }
            else {
                if (board[idx] == 'X') {
                    oCount = 0;
                    xCount++;
                    if (xCount >= 4) {
                        return 'X';
                    }
                }
                if (board[idx] == 'O') {
                    xCount = 0;
                    oCount++;
                    if (oCount >= 4) {
                        return 'O';
                    }
                }
            }
        }
        xCount = 0;
        oCount = 0;
    }

    xCount = 0;
    oCount = 0;

    // check rows:
    // loop through the entire board:
    for (int idx = 0; idx < 42; idx++) {

        // if at the beginning of a new row
        if (idx % 7 == 0) {
            // reset the count:
            xCount = 0;
            oCount = 0;
        }

        // if empty, continue:
        if (board[idx] == ' ') {
            xCount = 0;
            oCount = 0;
            continue;
        }
        else {
            if (board[idx] == 'X') {
                oCount = 0;
                xCount++;
                if (xCount >= 4) {
                    return 'X';
                }
            }
            if (board[idx] == 'O') {
                xCount = 0;
                oCount++;
                if (oCount >= 4) {
                    return 'O';
                }
            }
        }
    }

    xCount = 0;
    oCount = 0;

    // check diagonally up rows:
    int column = 0;

    // start at each row of the board,
    // plus 7 because there are 7 columns:
    for (int row = 0; row < 42; row = row + 7) {
        //check indexes diagonally up from the beginning of the row:
        for (int idx=row; idx >= column; idx = idx - 6) {

            // if empty, continue:
            if (board[idx] == ' ') {
                xCount = 0;
                oCount = 0;
                continue;
            }
            else {
                if (board[idx] == 'X') {
                    oCount = 0;
                    xCount++;
                    if (xCount >= 4) {
                        return 'X';
                    }
                }
                if (board[idx] == 'O') {
                    xCount = 0;
                    oCount++;
                    if (oCount >= 4) {
                        return 'O';
                    }
                }
            }
        }
        xCount = 0;
        oCount = 0;
        column++;
    }

    xCount = 0;
    oCount = 0;

    // check diagonally up from bottom columns:
    for (int bottom = 36; bottom < 42; bottom++) {
        //check indexes diagonally up:
        for (int idx = bottom; idx >= column; idx = idx - 6) {

            // if empty, continue:
            if (board[idx] == ' ') {
                xCount = 0;
                oCount = 0;
                continue;
            }
            else {
                if (board[idx] == 'X') {
                    oCount = 0;
                    xCount++;
                    if (xCount >= 4) {
                        return 'X';
                    }
                }
                if (board[idx] == 'O') {
                    xCount = 0;
                    oCount++;
                    if (oCount >= 4) {
                        return 'O';
                    }
                }
            }
        }
        xCount = 0;
        oCount = 0;
        column=column+7;
    }

    xCount = 0;
    oCount = 0;

    // check diagonally down starting from top columns:

    for (int column = 0; column < 7; column++) {
        for (int idx = column; idx < 42; idx = idx + 8) {
            // if empty, continue:
            if (board[idx] == ' ') {
                xCount = 0;
                oCount = 0;
                continue;
            }
            else {
                if (board[idx] == 'X') {
                    oCount = 0;
                    xCount++;
                    if (xCount >= 4) {
                        return 'X';
                    }
                }
                if (board[idx] == 'O') {
                    xCount = 0;
                    oCount++;
                    if (oCount >= 4) {
                        return 'O';
                    }
                }
            }
        }
        xCount = 0;
        oCount = 0;
    }

    xCount = 0;
    oCount = 0;

    // check diagonally down rows:

    for (int row = 7; row < 42; row = row + 7) {
        for (int idx = row; row < 42; row = row + 8) {

            // if empty, continue:
            if (board[idx] == ' ') {
                xCount = 0;
                oCount = 0;
                continue;
            }
            else {
                if (board[idx] == 'X') {
                    oCount = 0;
                    xCount++;
                    if (xCount >= 4) {
                        return 'X';
                    }
                }
                if (board[idx] == 'O') {
                    xCount = 0;
                    oCount++;
                    if (oCount >= 4) {
                        return 'O';
                    }
                }
            }
        }
        xCount = 0;
        oCount = 0;
    }

    return '-';
}

// takeTurn method deals with the logic of making a move, it checks if the move is valid, if yes, it saves it
// returns: 0 if the move is valid and saved, 1 if the move is invalid
// params: char *board - playing board, char token - user's token (X or O), int column - the column to where the move is being made
int takeTurn(char* board, char token, int column, int computerTurn)
{
    int invalidMove = 0;
    if (column > 7 || column < 1)
    {
        invalidMove = 1;
        // if it is not a computer turn, display information for the player:
        if(!computerTurn)
            printf("\n No such column. Please choose a different one.\n");
        return invalidMove;
    }
    printf(" Move: placed token %c into column %d\n", token, column);
    int offset = column - 1;

    // starting from the bottom of the first column
    // offset to move to the next column and minus 7 because there are 7 columns:
    for (int n = 35 + offset; n > 0; n = n - 7) {

        // if no space left:
        if (board[n] != ' ') {
            invalidMove = 1;
        }
        // if there is space:
        else if (board[n] == ' ') {
            invalidMove = 0;
            // save the move:
            board[n] = token;
            break;
        }
    }

    // if no space left in the chosen column and if it is not a computer turn:
    if (invalidMove && !computerTurn)
        printf("\n This column is full. Please choose a different one.\n");

    return invalidMove;
}



void game(struct Player *player_1, struct Player *player_2, int singleplayer) {
    // create the board and initialise it:
    char* board;
    board = initBoard();
    /*struct Board boardStruct;
    boardStruct.board=board;

    char *col1[6];
    populateColumns(col1, board);
    for (int i=0; i<6; i++){
      printf(" %c ", *col1[i]);
    }*/

    // create pointers to next, previous and temporary player:
    struct Player* next_player;
    struct Player* previous_player;
    struct Player* tmp_player;

    // set the next and previous players:
    next_player = player_1;
    previous_player = player_2;
    tmp_player = player_2;

    printBoard(board);

    // while not game over:
    int gameOver = 0;
    while (!gameOver) {

        // get the next player choice:
        int userChoice = 0;
        printf(" Your turn %s. ", next_player->name);
        int invalidMove = 1;

        if (singleplayer==1 && !strcmp(next_player->name, "Computer")) {
            while (invalidMove) {
                srand(time(0));
                int randNumber = rand() % 7;
                printf("\n");
                invalidMove = takeTurn(board, next_player->token, randNumber,1);
            }
        }
        else {
            printf("Type in the column number: ");
            int validInput = scanf("%d", &userChoice);

            // validate user's input:
            while (validInput != 1) {
                int temp;
                while ((temp = getchar()) != EOF && temp != '\n');
                printf("\n Incorrect input. Please choose a valid column: ");
                validInput = scanf("%d", &userChoice);
            }
            invalidMove = takeTurn(board, next_player->token, userChoice,0);
        }

        // if the move is valid, swap players:
        if (!invalidMove)
        {
            tmp_player = next_player;
            next_player = previous_player;
            previous_player = tmp_player;
        }

        printBoard(board);

        // check if there are any moves left:
        char win = checkWin(board);
        int moves = movesLeft(board);

        // if there is no winner and moves left:
        if (win == '-' && moves) {
            gameOver = 0;
        }
        // if the winner is X:
        else if (win == 'X') {
            gameOver = 1;
            printf(" Player %s wins!", player_1->name);
        }
        // if the winner is O:
        else if (win == 'O') {
            gameOver = 1;
            printf(" Player %s wins!", player_2->name);
        }
        // if there are no moves left:
        else if (moves == 0) {
            gameOver = 1;
            printf(" It's a draw!");
        }
    }

    printf("\n");
}

// multiplayer method - logic behind multiplayer game
void multiplayer() {

    printf("\n Multiplayer game: \n\n");

    // define players:
    struct Player player_1, player_2;

    // allocate space for players' names:
    char* p_1 = (char*)malloc(sizeof(char) * 20);
    char* p_2 = (char*)malloc(sizeof(char) * 20);

    // ask for users' names:

    printf(" Player 1 name (max 20 char): ");
    scanf("%s", p_1);

    // initialise player 1:
    player_1.name = p_1;
    player_1.winner = 0;
    player_1.token = 'X';

    printf(" Player 2 name (max 20 char): ");
    scanf("%s", p_2);

    // initialise player 2:
    player_2.name = p_2;
    player_2.winner = 0;
    player_2.token = 'O';

    game(&player_1, &player_2, 0);

}

void singlePlayer() {

    printf("\n Singleplayer game: \n\n");

    // define players:
    struct Player player_1, player_2;

    // allocate space for players' names:
    char* p_1 = (char*)malloc(sizeof(char) * 20);

    // ask for users' names:

    printf(" Player 1 name (max 20 char): ");
    scanf("%s", p_1);

    // initialise player 1:
    player_1.name = p_1;
    player_1.winner = 0;
    player_1.token = 'X';

    // initialise player 2:
    char computerName[9] = "Computer";
    player_2.name = &computerName[0];
    player_2.winner = 0;
    player_2.token = 'O';

    game(&player_1, &player_2, 1);

}

// runMenu -  a method with a loop that displays the game menu for the user
void runMenu() {

    // get user's menu choice:
    int gettingChoice = 1;

    while (gettingChoice) {

        printMenu();
        printf("\n");

        int userChoice = 0;
        printf(" Your choice: ");
        fflush(stdin);
        int validInput = scanf("%d", &userChoice);

        // validate user's input:
        while (validInput != 1) {
            int temp;
            while ((temp = getchar()) != EOF && temp != '\n');
            printf("\n Please select a valid option. Your choice: ");
            validInput = scanf("%d", &userChoice);
        }

        // call the right method based on user's choice:
        switch (userChoice) {
        case 1:
            //gettingChoice = 0;
            singlePlayer();
            break;
        case 2:
            //gettingChoice = 0;
            multiplayer();
            break;
        case 3:
            //gettingChoice = 0;
            gameHistory();
            break;
        case 4:
            //gettingChoice = 0;
            settings();
            break;
        case 5:
            gettingChoice = 0;
            printf("\n----------------------------");
            printf("\n Exiting...");
            printf("\n Goodbye!\n");
            break;
        default:
            printf("\n Please select a valid option.");
        }
    }
}

int main() {

    printf("\n ****************************");
    printf("\n * Welcome to Connect Four! * \n");
    printf(" ****************************\n\n");

    runMenu();

}
