#include <stdio.h>
#include <stdlib.h>
#include "LinkedList.h"

/*
	LinkedList methods source file
*/


/*
  Prints the elements of LinkedList
  Parameters: struct LinkedList *list -  a pointer to the list to be displayed
*/
void displayLinkedList(struct LinkedList* list)
{
    // while the pointer to the next element is not empty:
    while (list != NULL)
    {
        // print the stored data:
        printf("%d\n", list->data);
        // point to the next element:
        list = list->link;
    }
    printf("\n");
}

/*
  Counts the elements of a Linked List
  Parameters:
  struct LinkedList *list - a pointer to a LinkedList to be counted
  Returns: int - number of elements
*/
int countLinkedList(struct LinkedList* list)
{
    int count = 0;

    // while the pointer to the next node is not empty:
    while (list != NULL)
    {
        // list pointer becomes the next link
        list = list->link;
        // increment the count to count nodes/elements:
        count++;
    }
    return count;
}


/*
  Adds a new element to the end of the LinkedList
  Parameters:
  struct LinkedList **list - a pointer to the list  
  int num - data to be saved 
*/
void appendLinkedList(struct LinkedList** list, int num)
{
    struct LinkedList* temp, * r;
    if (*list == NULL) // if there are no elements
    {
        // create a new element at the beginning of the list
        temp = (struct LinkedList*)malloc(sizeof(struct LinkedList));
        temp->data = num;
        temp->link = NULL;
        *list = temp;
    }
    else
    {
        temp = *list;
        while (temp->link != NULL)
        {
            temp = temp->link; // find the last element
        }
        // add a new element at the end:
        r = (struct LinkedList*)malloc(sizeof(struct LinkedList));
        r->data = num;
        r->link = NULL;
        temp->link = r;
    }
}

/*
  Deletes an element at a specified index
  Parameters:
  struct LinkedList ** list - a pointer to the list to be amended 
  int location - location of the element to be deleted 
*/
void deleteAtLinkedList(struct LinkedList** list, int location)
{
    struct LinkedList* temp, *r = NULL;
    temp = *list;
    int i;

    for (i = 0; i < location; i++) // go to one location before the element that needs deleting
    {
        r = temp; // save that location (one before)
        temp = temp->link; // save the element to be deleted
    }
    if (temp->link != NULL) {
        r->link = temp->link; // make the previous element point to the next element
    }
    else {
        r->link = NULL;
    }
    free(temp); // release the unwanted element in the middle
}

/*
  Adds a new element after the specified location
  Parameters:
  struct LinkedList **list - a pointer to the list
  int location - location where the element should be added  
  int num - data to be saved 
*/
void insertAfterLinkedList(struct LinkedList *list, int location, int num)
{
  struct LinkedList *temp, *r;
  int i;
  temp=list;

  // get the link at the location and also check if the location is valid:
  for(i=0; i<location; i++)
  {
    temp=temp->link;
    if(temp==NULL)
    {
      printf("Length of the list is %d but supplied location is %d\n", i, location );
      return;
    }
  }
  // make r point to a memory location the size of a struct:
  r=(struct LinkedList *) malloc (sizeof(struct LinkedList));
  r->data=num; // save the new value
  r->link=temp->link; // make r point to the next element
  temp->link=r; // make temp point to r (instead of the next element)
}

/*
  Adds a new element before the specified location
  Parameters:
  struct LinkedList **list - a pointer to the list
  int location - location where the element should be added  
  int num - data to be saved 
*/
void insertBeforeLinkedList(struct LinkedList *list, int location, int num)
{
  struct LinkedList *temp, *r;
  int i;
  temp=list;

  // check if the location is valid:
  for (i=0; i<location-1; i++)
  {
    temp=temp->link;
    if (temp==NULL)
    {
      printf("Length of the list is %d but supplied location is %d\n", i+1, location );
      return;
    }
  }

    r=(struct LinkedList *) malloc(sizeof(struct LinkedList));
    r->data=num;
    r->link=temp->link;
    temp->link=r;

}
