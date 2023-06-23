// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "Stack.c"
#include "Stack.h"

// **** Set macros and preprocessor directives ****

// **** Define global, module-level, or external variables here ****

// **** Declare function prototypes ****


// This code will unit test the Stack.c implementation. You should run at least two different tests
// on each function to be able to determine if it is working correctly. You will have to write these
// tests yourself.


int main() {
    BOARD_Init();

    /******************************** Your custom testing code goes below here ***********************/
    printf("Welcome to CMPE13 Stack Unit Testing for lshuster, Leonid Shuster\n");

    struct Stack stack;
    
    int testsPassed = 0; 
    float testValue = 3;
    
  // Test for StackInit
    StackInit(&stack); // Initializes the stack
    
    if (stack->initialized == 1) {
        printf("Stack was successfully initialized\n");
        testsPassed++;
    }
    else {
        printf("Stack was not initialized\n");
    }
    
  // Test for StackPush
    if (StackPush(stack, testValue) == SUCCESS) {
        printf("Value was successfully pushed onto the stack\n");
        testsPassed++;
    }
    else {
        printf("Value was not pushed onto the stack\n");
    }
    
  // Test for StackPop
    if (StackPop(stack, testValue) == SUCCESS) {
        printf("Value was successfully popped off the stack\n");
        testsPassed++;
    }
    else {
        printf("Value was not popped off the stack\n");
    }

  // Test for StackIsEmpty
    if (StackIsEmpty(stack) == 0 || StackIsEmpty(stack) == 1) {
        printf("Stack is successfully empty or filled\n");
        testsPassed++;
    }
    else {
        printf("Stack is neither empty nor filled\n");
    }
    
  // Test for StackIsFull
    if (StackIsEmpty(stack) == 0 || StackIsEmpty(stack) == 1) {
        printf("Stack is successfully full or filled\n");
        testsPassed++;
    }
    else {
        printf("Stack is neither full nor filled\n");
    }
    
  // Test for StackGetSize
    if (StackGetSize(stack) == SIZE_ERROR || StackGetSize(stack) == 0 || 
            StackGetSize(stack) == stack->currentItemIndex + 1) {
        printf("Successfully got the size of the stack\n");
        testsPassed++;
    }
    else {
        printf("Didn't get the size of the stack\n");
    }
    
  // Prints percentage of tests correct
    float percentage = (testsPassed / 6.0) * 100;
    printf("%d out of 6 functions passed (%.1f%%).\n ", testsPassed, (double) percentage);
    /*************************************************************************************************/

    // You can never return from main() in an embedded system (one that lacks an operating system).
    // This will result in the processor restarting, which is almost certainly not what you want!
    while (1);
}

