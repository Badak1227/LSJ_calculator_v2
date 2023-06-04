#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct stackNode {
	double data;
	struct stackNode* next;
}sNode;

typedef struct stack {
	sNode* end;
}stack;

stack newStack() {
	stack* tmp = (stack*)malloc(sizeof(stack));
	tmp->end = NULL;

	stack res = *tmp;
	free(tmp);
	return res;
}

sNode* get_sNode() {
	sNode* tmp = (sNode*)malloc(sizeof(sNode));
	tmp->data = 0;
	tmp->next = NULL;
	return tmp;
}

void push(double data, stack* head) {
	sNode* tmp = get_sNode();
	tmp->data = data;
	tmp->next = head->end;
	head->end = tmp;
}

double pop(stack* head) {
	sNode* tmp = head->end;
	double res = tmp->data;
	head->end = head->end->next;
	free(tmp);

	return res;
}

double top(stack* head) {
	return head->end->data;
}

int isEmptyStack(stack* head) {
	return head->end == NULL ? 1 : 0;
}

void freeStack(stack* head) {
	while (!isEmptyStack(head)) {
		pop(head);
	}
}