#ifndef __QUEUE_H__ 
#define __QUEUE_H__

typedef struct queueNode {
	char data;
	struct queueNode* next;
}qNode;

typedef struct queue {
	qNode* front;
	qNode* rear;
}queue;

queue newQueue();

qNode* get_qNode();

void enQueue(char data, queue* head);

char deQueue(queue* head);

char peek(queue* head);

int isEmptyQueue(queue* head);

void freeQueue(queue* head);

#endif