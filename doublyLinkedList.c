#include <stdio.h>
#include <stdlib.h>
#include "doublyLinkedList.h"

/*
	Doubly LinkedList methods source file
*/

/*
  Counts the elements of a Doubly Linked List
  Parameters:
  struct DoublyLinked *list - a pointer to a DoublyLinkedList to be counted
  Returns: int - number of elements
*/
int countDoublyLinked(struct DoublyLinked *list)
{
  int count=0;
  // iterate throught the list:
  while (list!=NULL)
  {
    list=list->next;
    count++;
  }
  return count;
}

/*
  Prints the elements of DoublyLinkedList
  Parameters: struct DoublyLinked *list -  a pointer to the list to be displayed
*/
void displayDoublyLinked(struct DoublyLinked *list)
{
  // iterate throught the list:
  while (list!=NULL)
  {
    // print the contained data and token:
    printf("%d %c\n", list->data, list->token );
    list=list->next;
  }
  printf("\n");
}

/*
  Adds a new element to the end of the DoublyLinkedList
  Parameters:
  struct DoublyLinked **list - a pointer to the list  
  int num - data to be saved 
  char token - char token to be saved
*/
void appendDoublyLinked(struct DoublyLinked **list, int num, char token)
{
  struct DoublyLinked *temp, *current = *list;
  if(*list==NULL) // if the list is empty:
  {
    // create a new element and saved the given data:
    *list=(struct DoublyLinked *)malloc(sizeof(struct DoublyLinked));
    (*list)->prev=NULL;
    (*list)->data=num;
    (*list)->token=token;
    (*list)->next=NULL;
  }
  else // if the list is not empty:
  {
    while (current->next!=NULL)
    {
      current=current->next; // get to the last element
    }

    // create a new element and add it at the end:
    temp = (struct DoublyLinked*)malloc(sizeof(struct DoublyLinked));
    temp->data=num;
    temp->token=token;
    temp->next=NULL;
    temp->prev=current;
    current->next=temp;
  }
}

/*
  Adds a new element at the beginning of the DoublyLinkedList
  Parameters:
  struct DoublyLinked **list - a pointer to the list  
  int num - data to be saved 
  char token - char token to be saved
*/
void prependDoublyLinked(struct DoublyLinked **list, int num, char token)
{
  // create a new element:
  struct DoublyLinked *temp;
  temp=(struct DoublyLinked*)malloc(sizeof(struct DoublyLinked));
  temp->prev=NULL;
  temp->data=num;
  temp->token=token;
  temp->next=*list; // make next point to the list

  (*list)->prev=temp; // make list previous point to the new element
  *list=temp; // the beginning of the list is now the new element

}

/*
  Adds a new element after the specified location
  Parameters:
  struct DoublyLinked **list - a pointer to the list
  int location - location where the element should be added  
  int num - data to be saved 
  char token - char token to be saved
*/
void insertAfterDoublyLinked(struct DoublyLinked * list , int location , int num, char token )
{
  struct DoublyLinked * temp ;
  int i;

  // go to the given location:
  for (i =0; i< location ; i++)
  {
    list = list -> next ;
    if( list == NULL ) // if the given location is greater than the length of the list
    {
       printf (" Length of list is %d but supplied location is %d\n", i, location );
       return ;
    }
  }
  list = list -> prev ; // point to one element before the location
  // create a new element:
  temp = (struct DoublyLinked*)malloc(sizeof(struct DoublyLinked ));
  temp -> data = num ;
  temp -> token = token;
  temp -> prev = list ; // point to the one before
  temp -> next = list -> next ; // one after
  temp -> next -> prev = temp ; // next element previous points to temp
  list -> next = temp ;
}

/*
  Deletes an element with the specified value
  Parameters:
  struct DoublyLinked ** list - a pointer to the list to be amended
  int num - the value to be deleted
*/
void deleteDoublyLinked(struct DoublyLinked ** list , int num )
{
  struct DoublyLinked * temp = * list ;

  // find where the value is located:
  while ( temp != NULL )
  {
    if( temp->data == num )
    {
      if( temp==*list ) // if it's the first element
      {
        if((*list)->next==NULL){ // if there are no more elements in the list
          *list = NULL ; // set list as empty
        }
        else{
          *list = (*list)->next ; // point to the next one
          (*list)->prev = NULL ; // set previous as null
        }

      }
      else
      {
        if( temp->next == NULL ) // if it's the last element
        {
          temp->prev->next = NULL; // just set the one before to point to null
        }
        else
        {
          // if it's somewhere in the middle
          temp-> prev->next = temp->next ; // make the previous one next point to the next one 
          temp-> next->prev = temp->prev ; // make the next one previous point to the previous
        }
        free(temp); // free the one to be deleted
      }
      return;
    }
    temp = temp->next;
  }
  printf (" Element %d not found in the supplied list \n", num );
}

/*
  Deletes an element at a specified index
  Parameters:
  struct DoublyLinked ** list - a pointer to the list to be amended 
  int location - location of the element to be deleted 
*/
void deleteAtDoublyLinked(struct DoublyLinked **list, int location)
{
  struct DoublyLinked *temp, *r, *p;
  r = NULL;
  temp=*list;
  int i;

  for (i=0; i<location; i++) // go to one location before the element that needs deleting
  {
    r=temp; // save that location (one before)
    temp=temp->next; // save the element to be deleted
  }
  if (temp->next != NULL) { // if the element to be deleted is the last element in the list
    r->next = temp->next; // make the previous element point to the next element
    p=temp->next;
    p->prev=r; // make the next element point to the previous one
  }
  else { // if the element to be deleted is the last element in the list
    r->next = NULL;
  }

  free(temp); // release the unwanted element in the middle
}

/*
  For the purposes of ConnectFour game. Finds the last move (data) of the specified token (X or O)
  Parameters:
  struct DoublyLinked *list - a pointer to the list to be searched
  char token - token to find the last move for
  Returns:
  int - the stored data (last move)
*/
int findLastMove(struct DoublyLinked *list, char token)
{
   struct DoublyLinked *temp = list;

    // if the list is empty:
    if (list==NULL){
      return -1;
    }

    // if there is only one element in the list:
    if(temp->prev==NULL && temp->next==NULL && temp->token==token){
      return temp->data;
    }
    else if(temp->prev==NULL && temp->next==NULL)
    {
      return -1;
    }

    // if there is more than one element in the list:
    while (temp->next!=NULL)
    {
      temp=temp->next; // get to the last element
    }

    int lastMove = -1;
    // starting from the last element, go back until you find the last move for the given token:
    while(temp!=NULL)
    {
      if(temp->token==token)
      {
        lastMove=temp->data;
        break;
      }
      
      temp=temp->prev;
    }

    return lastMove;
}
