/*
*  Name: Leonid Shuster, ID #1616143
*  Role: Reads in file and creates a queue using a linked list. Can enqueue a number by adding to the end of the queue and
      allocating space for a node, dequeue a number by removing the first number of the queue and freeing the space used by the node,
      or print a queue. After finishing, everything left on the queue is freed
*  File name: queueClient.c
*  Instructions: Compile with make and then command ./queueClient "some input" "some output"
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

#define MAX 257

int main(int argc, char* argv[]){
   FILE* in;  /* file handle for input */  
   FILE* out; /* file handle for output */
   char input[MAX];

   /* check command line for correct number of arguments */
   if(argc != 3) {
      printf("Usage: %s <input file> <output file>\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   /* open input file for reading */
   in = fopen(argv[1], "r");
   if(in == NULL) {
      printf("Unable to read from file %s\n", argv[1]);
      exit(EXIT_FAILURE);
   }

   /* open output file for writing */
   out = fopen(argv[2], "w");
   if(out == NULL) {
      printf("Unable to write to file %s\n", argv[2]);
      exit(EXIT_FAILURE);
   }

   struct QueueLinkedList* queue = newQueue(); // Create new linked list
   while (fgets(input, MAX, in) != NULL) { // Keeps reading file until error from reading
      char* ch = strtok(input, " "); // Get letter instruction

      // Find out which instruction
      if (strcmp(ch, "e") == 0) { // If instruction is to enqueue
         ch = strtok(NULL, " "); // Since enqueued instruction also has a number, move on to next token
         enqueue(out, atoi(ch), queue);
      }
      else if (strcmp(ch, "d\n") == 0) { // If instruction is to dequeue
         dequeue(out, queue);
      }
      else if (strcmp(ch, "p\n") == 0) { // If instruction is to print
         printQueue(out, queue);
      }
      memset(input, '\0', MAX); // Reset input array
   }
   freeQueueLinkedList(queue); // Free nodes left in linked list and linked list itself

   
   /* close input and output files */
   fclose(in);
   fclose(out);

   return(EXIT_SUCCESS);
}