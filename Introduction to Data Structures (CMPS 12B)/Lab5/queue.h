/*
*  Name: Leonid Shuster, ID #1616143
*  Role: Contains prototypes for all the structs and functions located in queue.c
*  File name: queue.h
*  Instructions: Compile main file with make and then command ./queueClient "some input" "some output"
*/

#ifndef _QUEUE_H_INCLUDE_
#define _QUEUE_H_INCLUDE_

// Node
// Exported reference type 
struct QueueNode* node;

// LinkedList
// Exported reference type
struct QueueLinkedList* queue;

// Constructor for Node
// Takes in value, returns node
struct QueueNode* newNode(int x);
	
// Constructor for Queue represented by Linked List
// Takes in nothing, returns pointer to linked list
struct QueueLinkedList* newQueue(void);

// Destructor for the LinkedList type
// Takes in pointer to linked list
void freeQueueLinkedList(struct QueueLinkedList* queue);

// Prints queue from the front to the rear
// Takes in output file and pointer to linked list
void printQueue(FILE* out, struct QueueLinkedList* queue);

// Adds number to the back of the queue and prints "enqueued <int>"
// Takes in output file, number to enqueue, and pointer to linked list
void enqueue(FILE* out, int number, struct QueueLinkedList* queue);

// Delete number from front of the queue and prints "<int>"
// Takes in output file and pointer to linked list
void dequeue(FILE* out, struct QueueLinkedList* queue);

#endif