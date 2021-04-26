#pragma once

/*
	Header file for ConnectFour structs and methods' declarations
*/

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
    GameHistory struct (based on a linked list) to store information about past games
*/
struct GameHistory {
    struct Stack* gameMovesList;
    struct GameHistory* next;
    struct Player* player_1;
    struct Player* player_2;
    int columns;
    int rows;
};

// GameHistory.c methods:

void appendHistory(struct GameHistory** history, struct Stack* list, struct Player* player_1, struct Player* player_2, int columns, int rows);

int countHistory(struct GameHistory* list);

void displayGameHistory(struct GameHistory* list);

struct GameHistory *getElementAt(struct GameHistory *list, int location);

// ConnectFour.c methods:

char* initBoard(int columns, int rows);

void printBoard(char* board, int columns, int rows);

void printMenu();

void runMenu();

char checkWin(char* board, int columns, int rows);

int movesLeft(char* board, int columns, int rows);

int takeTurn(char* board, struct Player** nextPlayer, struct Stack** moves, int column, int columnSize, int rowSize);

void undoMove(char* board, struct Player** player, struct Stack** moves, struct Stack** undoneMoves, int singleplayer);

int redoMove(char* board, struct Player** player, struct Stack** moves, struct Stack** undoneMoves, int singleplayer);

struct Stack* game(struct Player* player_1, struct Player* player_2, int singleplayer, int columns, int rows);

void setUp(struct GameHistory** history, int singleplayer);

void replayGame(struct GameHistory* gameHistory);

void gameHistory(struct GameHistory** history);