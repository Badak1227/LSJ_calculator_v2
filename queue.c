#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct queueNode {
	char data;
	struct queueNode* next;
}qNode;

typedef struct queue {
	qNode* front;
	qNode* rear;
}queue;

queue newQueue() {
	queue* tmp = (queue*)malloc(sizeof(queue));
	tmp->front = NULL;
	tmp->rear = NULL;

	queue res = *tmp;
	free(tmp);
	return res;
}

qNode* get_qNode() {
	qNode* tmp = (qNode*)malloc(sizeof(qNode));
	tmp->data = 0;
	tmp->next = NULL;

	return tmp;
}

void enQueue(char data, queue* head) {
	qNode* tmp = get_qNode();
	tmp->data = data;
	
	if (head->front == NULL) {
		head->front = tmp;
		head->rear = tmp;
		return;
	}

	head->rear->next = tmp;

	head->rear = tmp;
}

char deQueue(queue* head) {
	qNode* tmp = head->front;

	char res = tmp->data;

	if (head->front == head->rear) head->rear = NULL;

	head->front = head->front->next;

	free(tmp);

	return res;
}

char peek(queue* head) {
	return head->front->data;
}

int isEmptyQueue(queue* head) {
	return head->front == NULL ? 1 : 0;
}

void freeQueue(queue* head) {
	while (!isEmptyQueue(head)) {
		deQueue(head);
	}
}