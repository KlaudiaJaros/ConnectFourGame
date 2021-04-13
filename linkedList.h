#pragma once

/*
	Header file for LinkedList struct and LinkedList methods' declarations
*/

/*
    Struct to store LinkedList element containg data and a pointer to the next element
*/
struct LinkedList {
    int data;
    struct LinkedList* link;
};


void displayLinkedList(struct LinkedList* list);

int countLinkedList(struct LinkedList* list);

void appendLinkedList(struct LinkedList** list, int num);

void deleteAtLinkedList(struct LinkedList** list, int location);

void insertAfterLinkedList(struct LinkedList *list, int location, int num);

void insertBeforeLinkedList(struct LinkedList *list, int location, int num);



