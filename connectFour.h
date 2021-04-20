#pragma once

/*
	Header file for ConnectFour methods' declarations
*/

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