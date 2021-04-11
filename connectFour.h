#pragma once

/*
	Header file for ConnectFour methods' declarations
*/

void populateColumns(char* col, char* board);

char* initBoard();

void printBoard(char* board);

void printMenu();

void runMenu();

char checkWin(char* board);

int movesLeft(char* board);

int takeTurn(char* board, char token, int column, int computerTurn);

void undoMove(char* board, int column);

struct LinkedList* game(struct Player* player_1, struct Player* player_2, int singleplayer);

void multiplayer(struct GameHistory** history);

void singlePlayer(struct GameHistory** history);

void appendHistory(struct GameHistory** history, struct LinkedList* list, char* player_1, char* player_2);

int countHistory(struct GameHistory* list);

void replayGame(struct GameHistory *gameHistory);

void gameHistory(struct GameHistory* history);