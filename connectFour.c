/*
    connectFour.exe - version 1.0 - author: Klaudia Jaros, 40439268

  ConnectFour -  a command line game written in C based on the popular Connect Four
  board game. The application allows the user to choose between single player and
  multiplayer mode, pick board sizes and view previous games.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include "connectFour.h"
#include "stack.h"
#include "gameHistory.h"

/*
  initBoard() - initialises the board for the game
  Parameters: int columns - number of columns, int rows - number of rows
  Returns: a char pointer to the initialised board
*/
char* initBoard(int columns, int rows)
{
    // allocate space for the board:
    int space = columns * rows;
    char* board = (char*)malloc(sizeof(char) * space);
    //  initialise its elements:
    for (int i = 0; i < space; i++)
    {
        board[i] = ' ';
    }
    return board;
}

/*
  printBoard() - prints the current state of the board
  Parameters:
  char *board - a pointer to the board to be printed
  int columns - number of columns
  int rows - number of rows
*/
void printBoard(char* board, int columns, int rows)
{
    int count = 0;
    int size = columns * rows;

    printf("\n");
    // print the header for column numbers:
    for (int i = 1; i <= columns; i++)
    {
        printf(" %d  ", i);
    }
    printf(" column\n");

    // print the board:
    for (int i = 0; i < size; i++)
    {
        printf(" %c |", board[i]);
        count++;
        if (count == columns) {
            // print \n after the end of each row:
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
    printf(" 4 - Exit\n");
    printf(" ----------------------------\n");
}

/*
    Given a pointer to a GameHistory struct, replayGame() method replays the game for the user.
    Parameters: struct GameHistory *gameHistory - a pointer to a game history to be replayed
*/
void replayGame(struct GameHistory* gameHistory) {

    // get the list of moves played in the game:
    struct Stack* gameMoves;
    gameMoves = gameHistory->gameMovesList;
    int movesCount = gameMoves->top + 1; // top of the stack (top is an index so +1 to get the count)

    // get the board size:
    int columns = gameHistory->columns;
    int rows = gameHistory->rows;

    // initialise the board for the game:
    char* board;
    board = initBoard(columns, rows);

    // get the players' saved in GameHistory:
    struct Player* player1 = gameHistory->player_1;
    struct Player* player2 = gameHistory->player_2;
    struct Player* nextPlayer = player1;
    struct Player* previousPlayer = NULL;

    // for every move taken:
    for (int i = 0; i < movesCount; i++) {

        // get the move:
        // gameMoves is a stack but it is still an array and can be used as a queue for the purposes of the game re-play
        // I don't use pop(), just first in, first out logic using direct access by index:
        int move = gameMoves->moves[i];

        // check who is making the move:
        if (gameMoves->tokens[i] == 'X') {
            nextPlayer = player1;
        }
        else {
            nextPlayer = player2;
        }

        // calculate the column of the move just to display it:
        int column = -1;
        for (int n = move; n >= 0; n = n - columns) {
            column = n + 1;
        }

        printf(" Player %s move: token %c to column %d \n", nextPlayer->name, nextPlayer->token, column);

        if (board[move] == ' ') {
            // save the move:
            board[move] = nextPlayer->token;
        }
        printBoard(board, columns, rows);

        // save previous player:
        previousPlayer = nextPlayer;
    }

    // display who won:
    if (player1->winner == 0 && player2->winner == 0) {
        printf("Game over! It's a draw!");
    }
    else if (previousPlayer->winner == 1) {
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

    int gameCount = countHistory(history); // count how many games there were
    displayGameHistory(history); // display the saved games

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

            printf("\n Game number %d replay: \n\n", userChoice);
            struct GameHistory* chosenGame;
            // get the chosen game and pass it to replay:
            chosenGame = getElementAt(history, (userChoice - 1));
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
    Parameters: char* board - a pointer to a game board to be checked for available moves
    int columns - number of columns
    int rows - number of rows
    Returns: 1 - if there is at least one move left, 0 - if there are no moves left.
*/
int movesLeft(char* board, int columns, int rows) {
    int size = columns * rows; // board size
    int spaceLeft = 0;

    // look for space:
    for (int i = 0; i < size; i++) {
        // if one empty space is found:
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
    Parameters: char* board - a pointer to a game board to be checked for winners
    int columns - number of columns
    int rows - number of rows
    Returns: char 'X' if the player with token 'X' wins, 'O' if the player with token 'O' wins, '-' if there are no winners
*/
char checkWin(char* board, int columns, int rows) {

    int size = columns * rows;
    int lastRow = size - columns;
    // count 'X' and 'O' that connect:
    int xCount = 0;
    int oCount = 0;

    // 1. check columns:

    // first loop: iterate through the number of columns:
    for (int column = 0; column < columns; column++) {
        // second loop: start from the bottom of column 1 because it's most likely to find 4 starting from the bottom
        for (int idx = lastRow + column; idx > 0; idx = idx - columns) {

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
    for (int idx = 0; idx < size; idx++) {

        // if at the beginning of a new row
        if (idx % columns == 0) {
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

    // start at each row of the board:
    for (int row = 0; row < size; row = row + columns) {
        //check indexes diagonally up from the beginning of the row:
        for (int idx = row; idx >= column; idx = idx - (columns - 1)) {

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
    column = columns - 1; // last index in a row

    // 4. check diagonally up from the bottom of columns / :
    for (int bottom = lastRow + 1; bottom < size; bottom++) {
        //check indexes diagonally up:
        for (int idx = bottom; idx >= column; idx = idx - (columns - 1)) {

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
        column = column + columns;
    }
    // reset count:
    xCount = 0;
    oCount = 0;

    // 5. check diagonally down starting from the top of columns \ :
    int lastIndex = size - 1;
    for (int column = 1; column < columns; column++) {
        for (int idx = column; idx <= lastIndex; idx = idx + (columns + 1)) {
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
        lastIndex = lastIndex - 8;
        xCount = 0;
        oCount = 0;
    }
    // reset count:
    xCount = 0;
    oCount = 0;

    // 6. check diagonally down staring from the beginning of the rows:
    // start at the beginning of a row:
    for (int row = 0; row < size; row = row + columns) {
        // move diagonally down by adding offset:
        for (int idx = row; idx < size; idx = idx + (columns + 1)) {

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
    Parameters:
    char *board - a pointer to the playing board
    struct Player** nextPlayer - player that takes the turn
    struct Stack** moves - list of moves
    int column - the column to where the move is being made
    int columnSize - column size in the game board
    int rowSize - row size in the game board
    Returns: 0 if the move is valid and saved, 1 if the move is invalid
*/
int takeTurn(char* board, struct Player** nextPlayer, struct Stack** moves, int column, int columnSize, int rowSize)
{
    struct Player* player = *nextPlayer;
    int computerTurn = player->isComputer;
    struct Stack* gameMoves = *moves;
    char token = player->token;
    int invalidMove = 0;

    // check if the move is valid by checking columns:
    if (column > columnSize || column < 1)
    {
        invalidMove = 1;
        // if it is not a computer turn, display information for the player:
        if (!computerTurn)
            printf("\n No such column. Please choose a valid column.\n");
        return invalidMove;
    }

    int size = columnSize * rowSize; // board size
    int lastRow = size - columnSize; // index of the first element in the last row
    int offset = column - 1; // offset to move to the next element up in the column

    // starting from the bottom of the specified column and moving up:
    for (int n = lastRow + offset; n >= 0; n = n - columnSize) {

        // if no space left:
        if (board[n] != ' ') {
            invalidMove = 1;
        }
        // if there is space:
        else if (board[n] == ' ') {
            invalidMove = 0;
            // save the move:
            board[n] = token;
            push(gameMoves, n, token);
            player->lastMove = n;
            break;
        }
    }

    // if no space left in the chosen column and it is not a computer turn:
    if (invalidMove && !computerTurn) {
        printf("\n This column is full. Please choose a different one.\n");
    } // if the move is valid:
    if (!invalidMove) {
        printf("\n Move: placed token %c into column %d\n", token, column);
    }

    return invalidMove;
}

/*
    undoMove() undo the latest move
    Paremeters: char *board - a pointer to the game board
    struct Player** player -  the player who undoes the move
    struct Stack** moves - list of moves
    struct Stack** undoneMoves - list of undone moves
    int singleplayer - a flag indicating if the game is in singleplayer mode
*/
void undoMove(char* board, struct Player** player, struct Stack** moves, struct Stack** undoneMoves, int singleplayer)
{
    struct Player* lastPlayer = *player;
    struct Stack* movesStack = *moves;
    struct Stack* undoneStack = *undoneMoves;

    // get the player's token, last move token and the move count:
    char token = lastPlayer->token;
    char lastMoveToken = whichToken(movesStack);
    int amountOfMoves = movesStack->top + 1;

    // if the stack is empty:
    if (movesStack->top == -1) {
        printf("\n No moves to undo. \n");
    }
    else if (!singleplayer) {

        // if the amount of moves is even or not even but the last move belongs to the next player:
        if (amountOfMoves % 2 == 0 || (amountOfMoves % 2 != 0 && lastMoveToken == token)) {

            // check if the current player owns the last move:
            if (lastMoveToken == token) {
                // undo the move on the board:
                board[movesStack->moves[movesStack->top]] = ' ';
                // pop the move from the saved moves stack:
                int move = pop(movesStack);
                // push it onto the undone moves stack:
                push(undoneStack, move, token);
            }
            else { // if not, pop the one before:
                int move = popOneBefore(movesStack);
                board[move] = ' ';
                push(undoneStack, move, token);
            }
        }
        else {
            printf("\n Cannot undo your previous move. The other player has to undo theirs first. \n");
        }
    }
    else if (singleplayer) { // if singleplayer:
    // undo the move on the board:
        board[movesStack->moves[movesStack->top]] = ' ';
        // pop a move from the saved moves stack:
        int move = pop(movesStack);

        // push the move onto the undone moves stack:
        // player's move:
        if (lastMoveToken == token) {
            push(undoneStack, move, token);
        }
        else { // computer move:
            push(undoneStack, move, lastMoveToken);
        }
    }
}

/*
    redoMove() redo the latest undone move
    Paremeters: char *board - a pointer to the game board
    struct Player** player -  the player who redoes the move
    struct Stack** moves - list of moves
    struct Stack** undoneMoves - list of undone moves
    int singleplayer - a flag indicating if the game is in singleplayer mode
    Returns:
    int 0 if no moves to redo or move taken
    int 1 if move was redone
*/
int redoMove(char* board, struct Player** player, struct Stack** moves, struct Stack** undoneMoves, int singleplayer)
{
    struct Player* lastPlayer = *player;
    struct Stack* movesStack = *moves;
    struct Stack* undoneStack = *undoneMoves;

    // get the player's token and the last move token:
    char playerToken = lastPlayer->token;
    char undoneMoveToken = undoneStack->tokens[undoneStack->top];

    // if the stack is empty:
    if (undoneStack->top == -1) {
        printf("\n No moves to redo. \n");
        return 0;
    }

    // singleplayer case when a computer move is being redone:
    if (singleplayer && (undoneMoveToken != playerToken)) {

        // get the last move from the stack:
        int undoneMove = pop(undoneStack);

        // save the move if the space is not occupied:
        if (board[undoneMove] == ' ') {
            board[undoneMove] = undoneMoveToken;
            push(movesStack, undoneMove, undoneMoveToken);
            return 1;
        }
        else {
            printf("\n Unable to redo the move. Space already occupied. \n");
            return 0;
        }
    }
    else { // singleplayer(not a computer) or multiplayer case:
        int undoneMove = -1;

        // if the last move doesn't belong to the current player and stack size isn't 1, move to the next element in stack:
        if (undoneMoveToken != playerToken && undoneStack->top != 0) {
            undoneMove = popOneBefore(undoneStack);
        }
        // if the last move is current player's:
        else if (undoneMoveToken == playerToken) {
            // save the move and pop it from stack:
            undoneMove = pop(undoneStack);
        }

        // if no valid undo move was found:
        if (undoneMove == -1) {
            printf("\n No moves to redo. \n");
            return 0;
        }
        // save the move if the space is not occupied:
        else if (board[undoneMove] == ' ') {
            board[undoneMove] = playerToken;
            push(movesStack, undoneMove, playerToken);
            lastPlayer->lastMove = undoneMove;
            return 1;
        }
        else {
            printf("\n Unable to redo the move. Space already occupied. \n");
            return 0;
        }
    }
}

/*
    game() method handles the connectFour game logic, saves moves and returns them as a Stack
    Parameters:
    struct Player *player_1 -  a pointer to Player 1
    struct Player *player_2 -  a pointer to Player 2
    int singleplayer - a flag that indicates if the game is singleplayer or multiplayer
    int columns - number of columns chosen by the player
    int rows - number of rows chosen by the player
    Returns:
    struct Stack* - a pointer to a Stack struct that contains all game moves taken during the game time
*/
struct Stack* game(struct Player* player_1, struct Player* player_2, int singleplayer, int columns, int rows)
{
    // create the board and initialise it:
    char* board;
    board = initBoard(columns, rows);

    // track moves:
    struct Stack* gameMoves;
    gameMoves = (struct Stack*)malloc(sizeof(struct Stack));
    initStack(gameMoves);
    struct Stack* undoneMoves;
    undoneMoves = (struct Stack*)malloc(sizeof(struct Stack));
    initStack(undoneMoves);

    // create pointers to next, previous and temporary player:
    struct Player* next_player;
    struct Player* previous_player;
    struct Player* tmp_player;

    // set the next and previous players:
    next_player = player_1;
    previous_player = player_2;
    tmp_player = player_2;

    // ask the user if they want to play in assisted mode:
    printf("\n Would you like to play in assisted mode? \n Assisted mode allows you to undo and redo your moves. Great for beginners!\n\n 1 - Yes \n 2 - No \n");
    printf("\n Your choice: ");
    int supportUndoMoves = 0;
    int userAnswer = 0;
    int temp;
    while ((temp = getchar()) != EOF && temp != '\n');
    scanf("%d", &userAnswer);
    if (userAnswer == 1 && userAnswer != 2) {
        supportUndoMoves = 1;
        printf("\n You have chosen assisted mode.\n");
    }
    else {
        printf("\n You haven't chosen assisted mode.\n");
    }

    printBoard(board, columns, rows);

    // while not game over:
    int gameOver = 0;
    while (!gameOver) {

        // get the next player choice:
        int userChoice = -1;
        int randNumber = -1; // computer choice

        printf(" Your turn %s. ", next_player->name);
        int invalidMove = 1;

        // if singleplayer mode and the player is a computer taking a turn:
        if (singleplayer == 1 && next_player->isComputer == 1) {
            printf("\n");
            while (invalidMove) {
                // generate a random move:
                srand(time(0));
                randNumber = rand() % columns;
                invalidMove = takeTurn(board, &next_player, &gameMoves, randNumber, columns, rows);
            }
        }
        // if not singleplayer or the player is not a computer:
        else {
            if (supportUndoMoves && !singleplayer) {
                printf("\n To make a move, type in the column number \n or 0 if you want to undo your last move\n or 99 if you want to redo your undone move \n");
            }
            else if (supportUndoMoves && singleplayer) {
                printf("\n To make a move, type in the column number \n or 0 if you want to undo the last played move\n or 99 if you want to redo the undone move \n");
            }
            else {
                printf("\n To make a move, type in the column number \n");
            }

            int temp;
            while ((temp = getchar()) != EOF && temp != '\n'); // get rid of white space chars

            printf("\n Your choice: ");
            scanf("%d", &userChoice);

            // player chooses to undo the move:
            if (userChoice == 0 && supportUndoMoves) {
                undoMove(board, &next_player, &gameMoves, &undoneMoves, singleplayer);
                // if multiplayer, swap players:
                if (!singleplayer) {
                    tmp_player = next_player;
                    next_player = previous_player;
                    previous_player = tmp_player;
                }
            } // player chooses to redo the move:
            else if (userChoice == 99 && supportUndoMoves) {
                int moveRedone = redoMove(board, &next_player, &gameMoves, &undoneMoves, singleplayer);
                // if multiplayer, swap players:
                if (moveRedone == 1 && !singleplayer) {
                    tmp_player = next_player;
                    next_player = previous_player;
                    previous_player = tmp_player;
                }
            } // normal move:
            else
                invalidMove = takeTurn(board, &next_player, &gameMoves, userChoice, columns, rows);
        }

        printBoard(board, columns, rows);

        // if the move is valid swap players:
        if (!invalidMove) {
            tmp_player = next_player;
            next_player = previous_player;
            previous_player = tmp_player;
        }

        // check if there are any moves left:
        char win = checkWin(board, columns, rows);
        int moves = movesLeft(board, columns, rows);

        // if there is no winner and moves left:
        if (win == '-' && moves) {
            gameOver = 0;
        }
        // if the winner is X:
        else if (win == 'X') {
            gameOver = 1;
            player_1->winner = 1;
            printf(" Player %s wins!", player_1->name);
        }
        // if the winner is O:
        else if (win == 'O') {
            gameOver = 1;
            player_2->winner = 1;
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
    setUp() method - sets up players and calls game() method, saves the game details in game history
    Parameters:
    struct GameHistory** history - a struct to store game information
    int singleplayer - a flag to indicate whether it is a singleplayer game
*/
void setUp(struct GameHistory** history, int singleplayer)
{
    if (singleplayer) {
        printf("\n Singleplayer game: \n\n");
    }
    else {
        printf("\n Multiplayer game: \n\n");
    }

    // define players:
    struct Player* player_1, * player_2;
    player_1 = (struct Player*)malloc(sizeof(struct Player));
    player_2 = (struct Player*)malloc(sizeof(struct Player));

    // save moves:
    struct Stack* gameMoves;
    gameMoves = NULL;

    // allocate space for players' names:
    char* p_1 = (char*)malloc(sizeof(char) * 20);
    char* computerName, * p_2;

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

    if (singleplayer) {
        computerName = (char*)malloc(sizeof(char) * 10);
        // initialise player 2:
        strcpy(computerName, "Computer\0");
        player_2->name = computerName;
        player_2->winner = 0;
        player_2->token = 'O';
        player_2->isComputer = 1;
    }
    else {
        p_2 = (char*)malloc(sizeof(char) * 20);
        printf(" Player 2 name (max 20 char): ");
        while ((temp = getchar()) != EOF && temp != '\n');
        scanf("%s", p_2);

        // initialise player 2:
        player_2->name = p_2;
        player_2->winner = 0;
        player_2->token = 'O';
        player_2->isComputer = 0;
        player_2->lastMove = -1;
    }

    printf("\n Choose the board size: \n");
    printf(" 1 - classic board: 7 columns 6 rows (7x6) \n 2 - board: 5 columns x 4 rows (5x4) \n 3 - board: 6 columns x 5 rows (6x5) \n");
    printf(" 4 - board: 8 columns x 7 rows (8x7) \n 5 - board: 8 columns x 8 rows (8x8) \n 6 - board: 9 columns x 7 rows (9x7) \n 7 - board: 10 columns x 7 rows (10x7) \n");
    printf("\n Your choice: ");
    while ((temp = getchar()) != EOF && temp != '\n');
    int boardSize = 0;
    scanf("%d", &boardSize);

    // default size:
    int columns = 7;
    int rows = 6;

    // set the board size based on user's choice:
    switch (boardSize) {
    case 2:
        columns = 5;
        rows = 4;
        break;
    case 3:
        columns = 6;
        rows = 5;
        break;
    case 4:
        columns = 8;
        rows = 7;
        break;
    case 5:
        columns = 8;
        rows = 8;
        break;
    case 6:
        columns = 9;
        rows = 7;
        break;
    case 7:
        columns = 10;
        rows = 7;
        break;
    }

    // call game() and document the players and their moves in game history:
    gameMoves = game(player_1, player_2, singleplayer, columns, rows);
    appendHistory(history, gameMoves, player_1, player_2, columns, rows);

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
            setUp(&history, 1);
            break;
        case 2:
            setUp(&history, 0);
            break;
        case 3:
            gameHistory(&history);
            break;
        case 4:
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
