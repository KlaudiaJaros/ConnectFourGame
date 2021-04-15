#pragma once
#define MAX 42

/*
    Header file for Stack struct and Stack methods' declarations
*/

struct Stack{
  int moves[MAX];
  char tokens[MAX];
  int top;
};

void initStack(struct Stack *s);

void push(struct Stack *s, int item, char token);

int pop(struct Stack *s);

void display(struct Stack* s);

char whichToken(struct Stack *s);

int popOneBefore(struct Stack* s);