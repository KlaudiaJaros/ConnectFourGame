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
#include "stack.h"

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
    struct Stack* gameMovesList;
    struct GameHistory* next;
    struct Player* player_1;
    struct Player* player_2;
};

/*
    appendHistory() - adds a new GameHistory element to the end of a GameHistory list
    Parameters:
    struct GameHistory** history - a pointer to a GameHistory to be appended
    struct Stack* list - a pointer to a list of moves that were taken in a game to be stored in GameHistory
    struct Player* - a pointer to player 1
    struct Player* - a pointer to player 2 
*/
void appendHistory(struct GameHistory** history, struct Stack* list, struct Player* player_1, struct Player* player_2)
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
    struct Stack* tempGame;
    tempGame = gameHistory->gameMovesList;
    int movesCount = tempGame->top + 1;

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
        int move = tempGame->moves[i];
        //int offset = move - 1; // column number to index 

        // check who is making the move:
        if (tempGame->tokens[i] == 'X') {
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
        //tempGame = tempGame->next;
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
            //displayDoublyLinked(chosenGame->gameMovesList); // debug
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
int takeTurn(char* board, struct Player** nextPlayer, struct Stack** moves, int column, int computerTurn)
{
    struct Player* player = *nextPlayer;
    struct Stack* s = *moves;
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
            push(s, n, token);
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
                board[movesStack->moves[movesStack->top]] = ' ';
                int move = pop(movesStack);
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
        board[movesStack->moves[movesStack->top]] = ' ';
        int move = pop(movesStack);

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
    if (undoneStack->top==-1) {
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
    else { // singleplayer or multiplayer case:
        int undoneMove = -1;

        // if the last move doesn't belong to the current player and stack size isn't 1, move to the next element in stack:
        if (undoneMoveToken != playerToken && undoneStack->top!=0) {
            undoneMove = popOneBefore(undoneStack);
        }
        // if the last move is current player's:
        else if (undoneMoveToken == playerToken){
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
    game() method handles the connectFour game logic
    Parameters:
    struct Player *player_1 -  a pointer to Player 1
    struct Player *player_2 -  a pointer to Player 2
    int singleplayer - a flag that indicates if the game is singleplayer or multiplayer
    Returns:
    struct Stack* - a pointer to a Stack struct that contains all game moves taken during the game time
*/
struct Stack* game(struct Player* player_1, struct Player* player_2, int singleplayer)
{
    // create the board and initialise it:
    char* board;
    board = initBoard();

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

    // ask the user if they want to undo their moves:
    printf("\n Would you like to play in the assisted mode? Assisted mode allows you to undo, redo and confirm your moves. Great for beginners!\n 1 - Yes \n 2 - No \n");
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

        // if singleplayer mode and the player is a computer taking a turn:
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
            if (supportUndoMoves && !singleplayer) {
                printf("\n To make a move, type in the column number \n or 0 if you want to undo your last move\n or 9 if you want to redo your undone move ");
            }
            else if (supportUndoMoves && singleplayer) {
                printf("\n To make a move, type in the column number \n or 0 if you want to undo the last played move\n or 9 if you want to redo the undone move ");
            }
            else {
                printf("\n To make a move, type in the column number \n");
            }

            printf("\n Your choice: ");
            int validInput = scanf("%d", &userChoice);

            // validate user's input:
            int temp;
            while ((temp = getchar()) != EOF && temp != '\n');
            
            if (userChoice == 0 && supportUndoMoves) {
                undoMove(board, &next_player, &gameMoves, &undoneMoves, singleplayer);

                if (!singleplayer) {
                    tmp_player = next_player;
                    next_player = previous_player;
                    previous_player = tmp_player;
                }
                            }
            else if (userChoice == 9 && supportUndoMoves) {
                int moveRedone = redoMove(board, &next_player, &gameMoves, &undoneMoves, singleplayer);
                if (moveRedone == 1 && !singleplayer) {
                    tmp_player = next_player;
                    next_player = previous_player;
                    previous_player = tmp_player;
                }
            }
            else
                invalidMove = takeTurn(board, &next_player, &gameMoves, userChoice, 0);
        }

        printBoard(board);

        // if the move is valid swap players:
        if (!invalidMove) {
            tmp_player = next_player;
            next_player = previous_player;
            previous_player = tmp_player;
        }

        display(gameMoves); // debug

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
            player_1->winner = 1;
            printf(" Player %s wins!", player_1->name);
        }
        // if the winner is O:
        else if (win == 'O') {
            gameOver = 1;
            player_1->winner = 1;
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
    struct Stack* gameMoves;
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
    struct Player* player_1, * player_2;
    player_1 = (struct Player*)malloc(sizeof(struct Player));
    player_2 = (struct Player*)malloc(sizeof(struct Player));

    // save moves:
    struct Stack* gameMoves;
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
