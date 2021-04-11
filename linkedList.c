#include <stdio.h>
#include <stdlib.h>
#include "LinkedList.h"

/*
	Library file for LinkedList methods
*/


// display all elements from a LinkedList:
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

// Count elements in a LinkedList
// parameter: a pointer to the first element of the LinkedList
int count(struct LinkedList* list)
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


// adds a new element to the end of a LinkedList
void appendLinkedList(struct LinkedList** list, int num)
{
    struct LinkedList* temp, * r;
    if (*list == NULL)
    {
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
            temp = temp->link;
        }

        r = (struct LinkedList*)malloc(sizeof(struct LinkedList));
        r->data = num;
        r->link = NULL;
        temp->link = r;
    }
}

// deletes an element at a specified location
void deleteAt(struct LinkedList** list, int location)
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

//adds a new element at after a specified location:
void insert_after(struct LinkedList *list, int location, int num)
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

//adds a new element befroe a specified location
void insert_before(struct LinkedList *list, int location, int num)
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
