#pragma once

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
};

void appendHistory(struct GameHistory** history, struct Stack* list, struct Player* player_1, struct Player* player_2);

int countHistory(struct GameHistory* list);

void displayGameHistory(struct GameHistory* list);

struct GameHistory *getElementAt(struct GameHistory *list, int location);