/*
    connectFour.exe - version 1.0 - author: Klaudia Jaros, 40439268

  ConnectFour -  a command line game written in C based on the popular Connect Four
  board game. The application allows the user to choose between single player and
  multiplayer mode and view previous games.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include "linkedList.h"
#include "connectFour.h"
#include "doublyLinkedList.h"

/*
    Player struct to store information about the player
*/
struct Player {
    char* name;
    int winner;
    char token;
    int isComputer;
    int lastMove;
};

/*
    GameHistory struct (linked list) to store information about past games
*/
struct GameHistory {
    struct DoublyLinked* gameMovesList;
    struct GameHistory* next;
    struct Player* player_1;
    struct Player* player_2;
};

/*
    appendHistory() - adds a new GameHistory element to the end of a GameHistory list
    Parameters:
    struct GameHistory** history - a pointer to a GameHistory to be appended
    struct LinkedList* list - a pointer to a list of moves that were taken in a game to be stored in GameHistory
    char* player_1 - a pointer to player 1 name
    char* player_2 - a pointer to player 2 name
*/
void appendHistory(struct GameHistory** history, struct DoublyLinked* list, struct Player* player_1, struct Player* player_2)
{
    struct GameHistory* temp, * r;
    // if history is null, just add the first element:
    if (*history == NULL)
    {
        // allocate space for the new element and save the given parameters in the struct:
        temp = (struct GameHistory*)malloc(sizeof(struct GameHistory));
        temp->gameMovesList = list;
        temp->next = NULL;
        temp->player_1 = player_1;
        temp->player_2 = player_2;
        *history = temp;
    }
    else
    {
        temp = *history;
        // go to the end of the linked list:
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        // allocate space for the new element and save the given parameters within the struct:
        r = (struct GameHistory*)malloc(sizeof(struct GameHistory));
        r->gameMovesList = list;
        r->next = NULL;
        r->player_1 = player_1;
        r->player_2 = player_2;
        temp->next = r;
    }
}

/*
    countHistory() method counts how many games(elements) there are in the GameHistory list (linked list)
    Parameters: struct GameHistory* list - a pointer to a GameHistory to be counted
    Returns: int - the number of games stored within the list
*/
int countHistory(struct GameHistory* list)
{
    int count = 0;

    // while the pointer to the next node is not empty:
    while (list != NULL)
    {
        // list pointer becomes the next link
        list = list->next;
        // increment the count to count nodes/elements:
        count++;
    }
    return count;
}

/*
  initBoard() - initialises the board for the game
  Returns: a char pointer to the initialised board
*/
char* initBoard()
{
    // allocate space for the board:
    char* board = (char*)malloc(sizeof(char) * 42);
    // board size of 42, initialise its elements:
    for (int i = 0; i < 42; i++)
    {
        board[i] = ' ';
    }
    return board;
}

/*
  printBoard() - prints the current state of the board
  Parameters: char *board - a pointer to the board to be printed
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

/*
  printMenu() - a void function that prints the application's menu
*/
void printMenu()
{

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

void settings() {
    printf(" Settings: \n");
}

/*
    Given a pointer to a GameHistory struct, replayGame() method replays the game for the user.
    Parameters: struct GameHistory *gameHistory - a pointer to a game history to be replayed
*/
void replayGame(struct GameHistory* gameHistory) {

    // get the list of moves played in the game:
    struct DoublyLinked* tempGame;
    tempGame = gameHistory->gameMovesList;
    int movesCount = countDoublyLinked(tempGame);

    // initialise the board for the game:
    char* board;
    board = initBoard();

    // get the players' names saved in GameHistory:
    struct Player* player1 = gameHistory->player_1;
    struct Player* player2 = gameHistory->player_2;
    struct Player* nextPlayer = player1;
    struct Player* previousPlayer = NULL;

    // for every move taken:
    for (int i = 0; i < movesCount; i++) {

        // get the move:
        int move = tempGame->data;
        //int offset = move - 1; // column number to index 

        // check who is making the move:
        if (tempGame->token == 'X') {
            nextPlayer = player1;
        }
        else {
            nextPlayer = player2;
        }

        int column = -1;
        for (int n = move; n >= 0; n = n - 7) {
            column = n + 1;
        }

        printf(" Player %s move: token %c to column %d \n", nextPlayer->name, nextPlayer->token, column);

        if (board[move] == ' ') {
            // save the move:
            board[move] = nextPlayer->token;
        }
        printBoard(board);

        // save previous player:
        previousPlayer = nextPlayer;

        // move to the next link(move):
        tempGame = tempGame->next;
    }

    // check who won:
    char win = checkWin(board);
    if (win == '-') {
        printf("Game over! It's a draw!");
    }
    else if (win == 'X' || win == 'O') {
        printf("\n Game over! Player %s wins\n", previousPlayer->name);
    }
}

/*
    Given a pointer to a GameHistory struct, gameHistory() method lists all available past games for the user to choose from
    Once the user chooses a game to replay, it calls replayGame() method
    Parameters: struct GameHistory *history - a pointer to a GameHistory struct containind past games(or empty) to be listed
*/
void gameHistory(struct GameHistory** gameHistory) {
    printf("\n Game History: \n");
    struct GameHistory* history = *gameHistory;
    // if there are no previous games saved:
    if (history == NULL) {
        printf(" No game history found.\n");
        return;
    }

    int count = 1; // for the list 
    int gameCount = countHistory(history); // count how many games there were
    struct GameHistory* tempHistory;
    tempHistory = history;

    // while the pointer to the next element is not empty:
    while (tempHistory != NULL)
    {
        // print the list:
        printf(" %d. %s vs %s \n", count, tempHistory->player_1->name, tempHistory->player_2->name);

        // point to the next game:
        tempHistory = tempHistory->next;
        count++;
    }
    printf("\n");
    printf(" Choose the game number to see a game replay or 0 to exit... \n");


    int userChoice = -1;
    int temp;

    // ask a user to choose a game to replay or press 0 to exit:
    while (userChoice < 0) {
        printf("\n Your choice: ");
        while ((temp = getchar()) != EOF && temp != '\n'); // to get rid of any white characters
        scanf("%d", &userChoice);

        // return to main menu:
        if (userChoice == 0) {
            return;
        } // replay the game:
        else if (userChoice > 0 && userChoice <= gameCount) {

            printf("\n Game number %d replay: \n", userChoice);

            int i = 1;
            struct GameHistory* tempGame, * chosenGame = NULL;
            tempGame = history;

            // find the game choosen by the user:
            while (i <= userChoice) {
                chosenGame = tempGame;
                tempGame = chosenGame->next;
                i++;
            }
            // display the game choose by the user:
            displayDoublyLinked(chosenGame->gameMovesList); // debug
            replayGame(chosenGame);
        }
        else {
            printf(" Please choose a valid option. \n");
            userChoice = -1;
        }
    }

}

/*
    A method that checks if there are any moves left on the game board
    Parameters: a pointer to a game board (char *) to be checked for available moves
    Returns: 1 - if there is at least one move left, 0 - if there are no moves left.
*/
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

/*
    A method to check if there are any combinations on the game board that makes one of the players a winner
    it checks if either 'X' or 'O' connects four times
    Parameters: a pointer to a game board (char *) to be checked for winners
    Returns: char 'X' if the player with token 'X' wins, 'O' if the player with token 'O' wins, '-' if there are no winners
*/
char checkWin(char* board) {

    // count 'X' and 'O' that connect:
    int xCount = 0;
    int oCount = 0;

    // 1. check columns:

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
        // reset count:
        xCount = 0;
        oCount = 0;
    }

    // reset count:
    xCount = 0;
    oCount = 0;

    // 2. check rows:
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
    // reset count:
    xCount = 0;
    oCount = 0;

    // 3. check diagonally up from the beginning of the rows / :
    int column = 0;

    // start at each row of the board,
    // plus 7 because there are 7 columns:
    for (int row = 0; row < 42; row = row + 7) {
        //check indexes diagonally up from the beginning of the row:
        for (int idx = row; idx >= column; idx = idx - 6) {

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
    // reset count:
    xCount = 0;
    oCount = 0;

    // 4. check diagonally up from the bottom of columns / :
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
        column = column + 7;
    }
    // reset count:
    xCount = 0;
    oCount = 0;

    // 5. check diagonally down starting from the top of columns \ :

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
    // reset count:
    xCount = 0;
    oCount = 0;

    // 6. check diagonally down staring from the beginning of the rows:

    for (int row = 7; row < 42; row = row + 7) {
        for (int idx = row; idx < 42; idx = idx + 8) {

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
        // reset count:
        xCount = 0;
        oCount = 0;
    }

    return '-';
}

/*
    takeTurn() method deals with the logic of making a move, it checks if the move is valid, if yes, it saves it
    Parameters: char *board - playing board, char token - user's token ('X' or 'O'), int column - the column to where the move is being made
    Returns: 0 if the move is valid and saved, 1 if the move is invalid
*/
int takeTurn(char* board, struct Player** nextPlayer, struct DoublyLinked** moves, int column, int computerTurn)
{
    struct Player* player = *nextPlayer;
    char token = player->token;
    int invalidMove = 0;
    if (column > 7 || column < 1)
    {
        invalidMove = 1;
        // if it is not a computer turn, display information for the player:
        if (!computerTurn)
            printf("\n No such column. Please choose a different one.\n");
        return invalidMove;
    }

    int offset = column - 1;

    // starting from the bottom of the specified column
    // offset to move to the next element in the column and minus 7 because there are 7 columns:
    for (int n = 35 + offset; n >= 0; n = n - 7) {

        // if no space left:
        if (board[n] != ' ') {
            invalidMove = 1;
        }
        // if there is space:
        else if (board[n] == ' ') {
            invalidMove = 0;
            // save the move:
            board[n] = token;
            appendDoublyLinked(moves, n, token);
            player->lastMove = n;
            break;
        }
    }

    // if no space left in the chosen column and if it is not a computer turn:
    if (invalidMove && !computerTurn) {
        printf("\n This column is full. Please choose a different one.\n");
    }
    if (!invalidMove) {
        printf("\n Move: placed token %c into column %d\n", token, column);
    }


    return invalidMove;
}

/*
    undoMove() undo the latest move in the specified column
    Paremeters: char *board - a pointer to the game board, int column - the column number to where the move was made and to be undone
*/
void undoMove(char* board, struct Player** player, struct DoublyLinked** moves)
{
    struct Player* lastPlayer = *player;

    // based on the user's last move, delete it from the board and game moves history:
    int lastMove = lastPlayer->lastMove;
    board[lastMove] = ' ';
    deleteDoublyLinked(moves, lastMove);

    // amend the player's last move:
    char token = lastPlayer->token;
    int newLastMove = findLastMove(*moves, token);
    lastPlayer->lastMove = newLastMove;
}

/*
    game() method handles the connectFour game logic
    Parameters:
    struct Player *player_1 -  a pointer to Player 1
    struct Player *player_2 -  a pointer to Player 2
    int singleplayer - a flag that indicates if the game is singleplayer or multiplayer
    Returns:
    struct LinkedList* - a pointer to a LinkedList struct that contains all game moves taken during the game time
*/
struct DoublyLinked* game(struct Player* player_1, struct Player* player_2, int singleplayer)
{
    // create the board and initialise it:
    char* board;
    board = initBoard();
    struct DoublyLinked* gameMoves;
    gameMoves = NULL;

    // create pointers to next, previous and temporary player:
    struct Player* next_player;
    struct Player* previous_player;
    struct Player* tmp_player;

    // set the next and previous players:
    next_player = player_1;
    previous_player = player_2;
    tmp_player = player_2;

    // ask the user if they want to undo their moves:
    printf("\n Would you like to be able to undo your moves?\n 1 - Yes \n 2 - No \n");
    printf("\n Your choice: ");
    int supportUndoMoves = 0;
    int userAnswer = 0;
    int temp;
    while ((temp = getchar()) != EOF && temp != '\n');
    scanf("%d", &userAnswer);
    if (userAnswer == 1) {
        supportUndoMoves = 1;
    }

    printBoard(board);

    // while not game over:
    int gameOver = 0;
    while (!gameOver) {

        // get the next player choice:
        int userChoice = 0;
        int randNumber = -1; // computer choice

        printf(" Your turn %s. ", next_player->name);
        int invalidMove = 1;

        // if singleplayer mode and the player is a computer:
        if (singleplayer == 1 && next_player->isComputer == 1) {
            printf("\n");
            while (invalidMove) {
                srand(time(0));
                randNumber = rand() % 7;
                invalidMove = takeTurn(board, &next_player, &gameMoves, randNumber, 1);
            }
        }
        // if not singleplayer or the player is not a computer:
        else {
            printf("\n To make a move, type in the column number or 0 if you want to undo your last move: ");
            printf("\n Your choice: ");
            int validInput = scanf("%d", &userChoice);

            // validate user's input:
            while (validInput != 1) {
                int temp;
                while ((temp = getchar()) != EOF && temp != '\n');
                printf("\n Incorrect input. Please choose a valid column: ");
                validInput = scanf("%d", &userChoice);
            }
            if (userChoice == 0) {

                if (next_player->lastMove == -1) {
                    printf("\n No moves to undo. \n");
                }
                else {
                    //undo the move:
                    undoMove(board, &next_player, &gameMoves);
                    tmp_player = next_player;
                    next_player = previous_player;
                    previous_player = tmp_player;
                }
            }
            else
                invalidMove = takeTurn(board, &next_player, &gameMoves, userChoice, 0);
        }

        printBoard(board);

        // if the move is valid and the player is not a computer, confirm the move and swap players or undo move:
        if (!invalidMove && next_player->isComputer == 0 && supportUndoMoves && userChoice != 0)
        {
            printf(" Confirm your move:\n");
            printf(" 1 - Keep my move\n");
            printf(" 2 - Undo my move\n");
            printf(" Your choice: ");
            int confirmMove = 0;

            while (!confirmMove) {
                // validate user's input:
                int validInput = scanf("%d", &confirmMove);

                // based on user input:
                switch (confirmMove) {
                case 1:
                    // keep playing
                    tmp_player = next_player;
                    next_player = previous_player;
                    previous_player = tmp_player;
                    break;
                case 2:
                    //undo the move:
                    undoMove(board, &next_player, &gameMoves);
                    tmp_player = next_player;
                    next_player = previous_player;
                    previous_player = tmp_player;
                    break;
                default:
                    confirmMove = 0;
                    break;
                }
            }
            printf("\n");
        } // if the move is valid swap players:
        else if (!invalidMove) {
            tmp_player = next_player;
            next_player = previous_player;
            previous_player = tmp_player;

        } 

        displayDoublyLinked(gameMoves); // debug

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
            player_1->winner=1;
            printf(" Player %s wins!", player_1->name);
        }
        // if the winner is O:
        else if (win == 'O') {
            gameOver = 1;
            player_1->winner=1;
            printf(" Player %s wins!", player_2->name);
        }
        // if there are no moves left:
        else if (moves == 0) {
            gameOver = 1;
            printf(" It's a draw!");
        }
    }

    printf("\n");
    return gameMoves;
}

/*
    multiplayer() method - prompts both users for their name, saves them to gameHistory, creates Player structs and calls game() method
    Parameters: struct GameHistory** history - a struct to store game information
*/
void multiplayer(struct GameHistory** history)
{

    printf("\n Multiplayer game: \n\n");

    // define players:
    struct Player* player_1, * player_2;
    player_1 = (struct Player*)malloc(sizeof(struct Player));
    player_2 = (struct Player*)malloc(sizeof(struct Player));

    // save moves:
    struct DoublyLinked* gameMoves;
    gameMoves = NULL;

    // allocate space for players' names:
    char* p_1 = (char*)malloc(sizeof(char) * 20);
    char* p_2 = (char*)malloc(sizeof(char) * 20);

    // ask for users' names:

    printf(" Player 1 name (max 20 char): ");
    int temp;
    while ((temp = getchar()) != EOF && temp != '\n');
    scanf("%s", p_1);

    // initialise player 1:
    player_1->name = p_1;
    player_1->winner = 0;
    player_1->token = 'X';
    player_1->isComputer = 0;
    player_1->lastMove = -1;

    printf(" Player 2 name (max 20 char): ");
    while ((temp = getchar()) != EOF && temp != '\n');
    scanf("%s", p_2);

    // initialise player 2:
    player_2->name = p_2;
    player_2->winner = 0;
    player_2->token = 'O';
    player_2->isComputer = 0;
    player_2->lastMove = -1;

    // call game() and document the players and their moves in game history:
    gameMoves = game(player_1, player_2, 0);
    appendHistory(history, gameMoves, player_1, player_2);

}

/*
    singlePlayer() method - prompts the user for their name, saves it to gameHistory, creates Player structs and calls game() method
    Parameters: struct GameHistory** history - a struct to store game information
*/
void singlePlayer(struct GameHistory** history)
{

    printf("\n Singleplayer game: \n\n");

    // define players:
    struct Player *player_1, *player_2;
    player_1 = (struct Player*)malloc(sizeof(struct Player));
    player_2 = (struct Player*)malloc(sizeof(struct Player));

    // save moves:
    struct DoublyLinked* gameMoves;
    gameMoves = NULL;

    // allocate space for players' names:
    char* p_1 = (char*)malloc(sizeof(char) * 20);
    char* computerName = (char*)malloc(sizeof(char) * 10);

    // ask for user's name:
    printf(" Player 1 name (max 20 char): ");
    int temp;
    while ((temp = getchar()) != EOF && temp != '\n');
    scanf("%s", p_1);

    // initialise player 1:
    player_1->name = p_1;
    player_1->winner = 0;
    player_1->token = 'X';
    player_1->isComputer = 0;
    player_1->lastMove = -1;

    // initialise player 2:
    strcpy(computerName, "Computer\0");
    player_2->name = computerName;
    player_2->winner = 0;
    player_2->token = 'O';
    player_2->isComputer = 1;

    // call game() and document the players and their moves in game history:
    gameMoves = game(player_1, player_2, 1);
    appendHistory(history, gameMoves, player_1, player_2);

}

/*
    A method with a loop that displays the game menu for the user and asks for user's choice
*/
void runMenu() {

    // get user's menu choice:
    int gettingChoice = 1;

    // save game history:
    struct GameHistory* history;
    history = NULL;

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
            singlePlayer(&history);
            break;
        case 2:
            multiplayer(&history);
            break;
        case 3:
            gameHistory(&history);
            break;
        case 4:
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
