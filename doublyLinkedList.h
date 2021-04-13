#pragma once

/*
	Header file for Doubly LinkedList struct and its methods' declarations
*/

/*
    Struct to store DoublyLinkedList element containg data, a pointer to the next element and a pointer to the previous element
*/
struct DoublyLinked
{
  int data;
  char token;
  struct DoublyLinked *prev;
  struct DoublyLinked *next;
};

int countDoublyLinked(struct DoublyLinked *list);

void displayDoublyLinked(struct DoublyLinked *list);

void appendDoublyLinked(struct DoublyLinked **list, int num, char token);

void prependDoublyLinked(struct DoublyLinked **list, int num, char token);

void insertAfterDoublyLinked( struct DoublyLinked * list , int location , int num, char token );

void deleteDoublyLinked(struct DoublyLinked **list, int num);

void deleteAtDoublyLinked(struct DoublyLinked **list, int location);

int findLastMove(struct DoublyLinked *list, char token);