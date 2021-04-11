#pragma once

/*
	Header file for LinkedList struct and LinkedList methods' declarations
*/

struct LinkedList {
    int data;
    struct LinkedList* link;
};


void displayLinkedList(struct LinkedList* list);

int count(struct LinkedList* list);

void appendLinkedList(struct LinkedList** list, int num);

void deleteAt(struct LinkedList** list, int location);

void insert_after(struct LinkedList *list, int location, int num);

void insert_before(struct LinkedList *list, int location, int num);



