/*
*  Name: Leonid Shuster, ID #1616143
*  Role: Creates structs for nodes and linked list. Also has functions for creating a new node and new linked list,
	freeing a linked list, printing the queue, enqueueing and dequeueing
*  File name: queue.c
*  Instructions: Compile main file with make and then command ./queueClient "some input" "some output"
*/

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

typedef struct QueueNode { // Singly linked list, stores value and next node
	int val;
	struct QueueNode* next;
} QueueNode;

typedef struct QueueLinkedList {
	struct QueueNode* front; // Pointer for node in the front
	struct QueueNode* rear; // Pointer for node in the rear
} QueueLinkedList;

QueueNode* newNode(int x) {
	struct QueueNode* node = (QueueNode*) malloc(sizeof(struct QueueNode)); // Allocates space for new node
	node->val = x; // Stores number in node
	node->next = NULL; // New node becomes the end of the linked list, nothing after it
	return node;
}
	
QueueLinkedList* newQueue(void) {
	struct QueueLinkedList* queue = (QueueLinkedList*) malloc(sizeof(struct QueueLinkedList)); // Allocates space for linked list
	queue->front = NULL; // Initialize front to NULL
	queue->rear = NULL; // Initialize rear to NULL
	return queue;
}

void freeQueueLinkedList(struct QueueLinkedList* queue) {
	struct QueueNode* current = queue->front; // Set current to the front of the list
	struct QueueNode* next; // Pointer to temporarily hold next

	while (current != NULL) { // Keep iterating until no more nodes
		next = current->next; // Save next node
		free(current); // Free current node
		current = next; // Move on to next node
	}
	free(queue); // Free linked list
	queue = NULL;
}

void printQueue(FILE* out, struct QueueLinkedList* queue) {
	struct QueueNode* current = queue->front; // Set current to the front of the list

	while (current != NULL) { // Keep iterating until no more nodes
		fprintf(out, "%d ", current->val); // Print value at each node
		current = current->next; // Move on to next node
	}
	fprintf(out, "\n");
}

void enqueue(FILE* out, int number, struct QueueLinkedList* queue) {
	struct QueueNode* node = newNode(number); // Set node to new node created

	fprintf(out, "enqueued %d\n", node->val); // Print confirmation of new enqueued node
	if (queue->rear == NULL) { // If queue is empty, new node is both front and rear
		queue->front = node;
		queue->rear = node;
		return;
	}
	// If queue is not empty, new node is at the end of the queue and rear is updated
	queue->rear->next = node;
	queue->rear = node;
}

void dequeue(FILE* out, struct QueueLinkedList* queue) {
	if (queue->front == NULL) { // If queue is empty, print empty
		fprintf(out, "empty\n");
		return;
	}
	// If queue is not empty, store previous and update front
	struct QueueNode* node = queue->front;
	queue->front = queue->front->next;

	if (queue->front == NULL) { // If updating front makes the front become NULL, then rear should also be NULL
		queue->rear = NULL;
	}

	fprintf(out, "%d\n", node->val);
	free(node); // Free dequeued node
	node = NULL;
}
