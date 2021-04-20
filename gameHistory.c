#include "gameHistory.h"
#include <stdio.h>
#include <stdlib.h>

/*
    appendHistory() - adds a new GameHistory element to the end of a GameHistory list
    Parameters:
    struct GameHistory** history - a pointer to a GameHistory to be appended
    struct Stack* list - a pointer to a list of moves that were taken in a game to be stored in GameHistory
    struct Player* - a pointer to player 1
    struct Player* - a pointer to player 2
*/
void appendHistory(struct GameHistory** history, struct Stack* list, struct Player* player_1, struct Player* player_2, int columns, int rows)
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
        temp->rows=rows;
        temp->columns=columns;
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
        r->rows=rows;
        r->columns=columns;
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
  Prints the elements of GameHistory LinkedList as a list
  Parameters: struct GameHistory *list -  a pointer to the game history to be displayed
*/
void displayGameHistory(struct GameHistory* list)
{
    int count =1;

    // while the pointer to the next element is not empty:
    while (list != NULL)
    {
        // print the stored data:
        printf(" %d. %s vs %s \n", count, list->player_1->name, list->player_2->name);
        // point to the next element:
        list = list->next;
        count++;
    }
    printf("\n");
}


/*
  Gets a pointer to the element at the specified location
  Parameters:
  struct LinkedList *list - a pointer to the list
  int location - location of the element to be returned  
  struct GameHistory *gameHistory - a pointer to the specified gameHistory element
*/
struct GameHistory *getElementAt(struct GameHistory *list, int location)
{
  struct GameHistory *temp, *r;
  int i;
  temp=list;

  // get the link at the location and also check if the location is valid:
  for(i=0; i<location; i++)
  {
    temp=temp->next;
    if(temp==NULL)
    {
      printf("Length of the list is %d but supplied location is %d\n", i, location );
      return NULL;
    }
  }
    return temp;
}