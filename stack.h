#ifndef __STACK_H__ 
#define __STACK_H__

typedef struct stackNode {
	double data;
	struct stackNode* next;
}sNode;

typedef struct stack {
	sNode* end;
}stack;

stack newStack();

sNode* get_sNode();

void push(double data, stack* head);

double pop(stack* head);

double top(stack* head);

int isEmptyStack(stack* head);

void freeStack(stack* head);
#endif