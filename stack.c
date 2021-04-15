#include <stdio.h>
#include "stack.h"

/*
	Library file for Stack methods
*/


/*
	Sets the stack top pointer to -1 to say that the stack is empty.
	Parameters:
	struct Stack *s - a pointer to the stack
*/
void initStack(struct Stack *s){
	s->top=-1;
}

/*
	Adds a new element at the top of the stack
	Parameters:
	struct Stack *s - a pointer to the stack to be modified
	int item - item to be added
	char token - token to be added
*/
void push(struct Stack *s, int item, char token){
	
	// if the stack is empty, return:
	if(s->top == MAX-1){
		printf("Stack is full. Couldn't push %d onto stack\n", item );
		return;
	}
	// increment the top pointer and save the data:
	s->top++;
	s->moves[s->top] = item;
	s->tokens[s->top] = token;
}

/*
	Pops the last element on the stack
	Parameters:
	struct Stack *s - a pointer to the stack to be modified
	Returns:
	int data - the value that was held in the poped element
*/
int pop(struct Stack *s){
	int data=-1;
	// if the stack is empty, return -1:
	if(s->top==-1){
		printf("Stack is empty.\n");
		return data;
	}
	// get the stored data and change the top pointer to one down:
	data=s->moves[s->top];
	s->top--;
	return data;
}

/*
	Pops an element before the last and moves the last element to its place
	Parameters:
	struct Stack *s - a pointer to the stack to be modified
	Returns:
	int data - the value that was held in the poped element
*/
int popOneBefore(struct Stack* s) {
	int data=-1;
	// if the stack is empty, return -1:
	if (s->top == -1) {
		printf("Stack is empty.\n");
		return data;
	}
	// save the data from the last element:
	int tempData = s->moves[s->top]; // get the data from one before
	char tempToken = s->tokens[s->top]; // get the token from one before
	data = s->moves[(s->top)-1]; 
	s->top--; // point to the element before last
	// save last element data and token:
	s->moves[s->top] = tempData; 
	s->tokens[s->top] = tempToken;
	return data;
}

/*
	Gets the token (char) stored in the last element of the stack
	Parameters:
	struct Stack *s - a pointer to the stack
	Returns:
	char token - the char token that is in the last element
*/
char whichToken(struct Stack *s){
	// if the stack is empty, return '-' :
	if(s->top==-1){
		return '-';
	}
	else {
		return s->tokens[s->top];
	}
}

/*
	Prints the entire stack
	Parameters:
	struct Stack *s - a pointer to the stack
*/
void display(struct Stack* s){
	int idx;
	for(idx=0; idx<=s->top; idx++){
		printf("%d %c \n", s->moves[idx], s->tokens[idx]);
	}
	printf("\n");
}
