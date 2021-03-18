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
    printf("\n  Welcome to Connect Four! \n");
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

void singlePlayer() {
    printf(" Single Player Game: \n");
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
    for (int i = 0; i < 6; i++) {
        for (int n = 35 + i; n > 0; n = n - 7) {
            if (board[n] != ' ') {
                continue;
            }
            else{
                if (board[n] == 'X') {
                    oCount = 0;
                    xCount++;
                    if (xCount >= 4) {
                        return 'X';
                    }
                }
                if (board[n] == 'O') {
                    xCount = 0;
                    oCount++;
                    if (oCount >= 4) {
                        return 'O';
                    }
                }
            }
        }
    }

    // check rows:

    return '-';
}

int takeTurnMultiplayer(char* board, char token, int column)
{
    int invalidMove = 0;
    if (column > 7 || column < 1)
    {
        invalidMove = 1;
        printf("\n No such column. Please choose a different one.\n");
        return invalidMove;
    }
    printf(" Move for: %c, column: %d\n", token, column);
    int offset = column - 1;

    for (int n = 35 + offset; n > 0; n=n-7) {
        if (board[n] != ' ') {
            invalidMove = 1;
        }
        else if (board[n] == ' ') {
            invalidMove = 0;
            board[n] = token;
            break;
        }
    }

    if(invalidMove)
        printf("\n This column is full. Please choose a different one.\n");

    return invalidMove;
}

void multiplayer() {
    struct Player player_1, player_2;
    struct Player* next_player;
    struct Player* previous_player;
    struct Player* tmp_player;

    char* p_1 = (char*)malloc(sizeof(char) * 20);
    char* p_2 = (char*)malloc(sizeof(char) * 20);

    printf(" Multiplayer game: \n\n");

    printf(" Player 1 name (max 20 char): ");
    scanf("%s", p_1);
    player_1.name = p_1;
    player_1.winner = 0;
    player_1.token = 'X';

    printf(" Player 2 name (max 20 char): ");
    scanf("%s", p_2);
    player_2.name = p_2;
    player_2.winner = 0;
    player_2.token = 'O';

    char* board;
    board = initBoard();
    /*struct Board boardStruct;
    boardStruct.board=board;

    char *col1[6];
    populateColumns(col1, board);
    for (int i=0; i<6; i++){
      printf(" %c ", *col1[i]);
    }*/

    next_player = &player_1;
    previous_player = &player_2;
    tmp_player = &player_2;

    // while not game_over
    int gameOver = 0;
    while (!gameOver) {
        printBoard(board);
        int userChoice = 0;
        printf(" Your turn %s. Type in the column number: ", next_player->name);

        int validInput = scanf("%d", &userChoice);
        while (validInput!=1) {
            int temp;
            while ((temp = getchar()) != EOF && temp != '\n');
            printf("\n Incorrect input. Please choose a valid column: ");
            validInput = scanf("%d", &userChoice);
        }

        int invalidMove = 0;
        invalidMove = takeTurnMultiplayer(board, next_player->token, userChoice);

        if (!invalidMove)
        {
            tmp_player = next_player;
            next_player = previous_player;
            previous_player = tmp_player;
        }

        // check if there are any moves left:
        char win = checkWin(board);
        int moves = movesLeft(board);
        if (win == '-' && moves) {
            gameOver = 0;
        }
        else if (moves == 0) {
            gameOver = 1;
            printf("It's a draw!");
        }
        else if (win == 'X') {
            gameOver = 1;
            printf("Player %s wins!", player_1.name);
        }
        else if (win == 'O') {
            gameOver = 1;
            printf("Player %s wins!", player_2.name);
        }
    }
}


int main() {

    printMenu();

    int gettingChoice = 1;
    printf("\n");

    while (gettingChoice) {
        int userChoice = 0;
        printf(" Your choice: ");
        fflush(stdin);
        int validInput= scanf("%d", &userChoice);
        while (validInput != 1) {
            int temp;
            while ((temp = getchar()) != EOF && temp != '\n');
            printf("\n Please select a valid option. Your choice: ");
            validInput = scanf("%d", &userChoice);
        }

        switch (userChoice) {
        case 1:
            gettingChoice = 0;
            singlePlayer();
            break;
        case 2:
            gettingChoice = 0;
            multiplayer();
            break;
        case 3:
            gettingChoice = 0;
            gameHistory();
            break;
        case 4:
            gettingChoice = 0;
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
