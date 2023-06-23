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
#include "Stack.h"

// **** Set macros and preprocessor directives ****

// **** Define global, module-level, or external variables here ****
#define CHAR_LIMIT 60

// **** Declare function prototypes ****
void ProcessTokens(struct Stack *stack, char *userInput);
int ProcessOperator(struct Stack *stack, char *userInput);

/*
 * Function prototype for ProcessBackspaces() - This function should be defined
 * below main after the related comment.
 */
int ProcessBackspaces(char *rpn_sentence);

// If this code is used for testing, rename main to something we can call from our testing code. We
// also skip all processor configuration and initialization code.

#ifndef LAB4_TESTING

int main()
{
    BOARD_Init();
#else

int their_main(void)
{
#endif // LAB4_TESTING

    /******************************** Your custom code goes below here *******************************/
    printf("Welcome to Leon's RPN calculator.\n");
    printf("Enter floats and + - / * in RPN format:\n");

    struct Stack stack;

    char userInput[CHAR_LIMIT];

    while (1) {

        fgets(userInput, CHAR_LIMIT, stdin); // Receives input from the user, stores in userInput, no more than 60 characters

        userInput[strcspn(userInput, "\n\0")] = '\0'; // Adds null terminator to the end of string

        StackInit(&stack); // Initializes the stack

        ProcessTokens(&stack, userInput);
    }
    /*************************************************************************************************/

    // You can never return from main() in an embedded system (one that lacks an operating system).
    // This will result in the processor restarting, which is almost certainly not what you want!
    while (1);
}

/**
 * Extra Credit: Define ProcessBackspaces() here - This function should read through an array of
 * characters and when a backspace character is read it should replace the preceding character with
 * the following character. It should be able to handle multiple repeated backspaces and also
 * strings with more backspaces than characters. It should be able to handle strings that are at
 * least 256 characters in length.
 * @param rpn_sentence The string that will be processed for backspaces. This string is modified in
 *                     place.
 * @return Returns the size of the resultant string in `rpn_sentence`.
 */
int ProcessBackspaces(char *rpn_sentence)
{
    return 0;
}

void ProcessTokens(struct Stack *stack, char *userInput)
{
    char *token = strtok(userInput, " ");
    float value;
    int error = 0;

    do {
        if (strlen(token) == 1) { // If length is one, it's either a single digit or operator
            if (strpbrk(token, "+-*/")) { // Checks if token has any of the operators
                if (ProcessOperator(stack, token) == STANDARD_ERROR) { // If error came up when performing operation
                    printf("ERROR: Not enough operands before operator.\n");
                    error = 1;
                    break;
                }
            } else if (strpbrk(token, "0123456789")) { // Checks if token has any of the numbers
                value = atof(token);
                if (StackPush(stack, value) == STANDARD_ERROR) { // If error came up when pushing onto stack
                    printf("ERROR: No more room on stack.\n");
                    error = 1;
                    break;
                }
            } else { // If token isn't both a number or operator
                printf("ERROR: Invalid character in RPN string.\n");
                error = 1;
                break;
            }
        } else { // If length isn't one, must be a number
            value = atof(token);
            if (StackPush(stack, value) == STANDARD_ERROR) { // If error came up when pushing onto stack
                printf("ERROR: No more room on stack.\n");
                error = 1;
                break;
            }
        }
    } while ((token = strtok(NULL, " ")) != NULL);

    if (error == 0) { // If there weren't any errors
        if (StackGetSize(stack) != 1 || StackGetSize(stack) == SIZE_ERROR) // If there is more than one element on the stack
            printf("ERROR: Invalid RPN calculation: more or less than one item in the stack.\n");
        else
            printf("Result: %.2f\n", (double) stack->stackItems[stack->currentItemIndex]);
    }
}

int ProcessOperator(struct Stack *stack, char *operator)
{
    float num1, num2, result;

    if (StackPop(stack, &num2) == SUCCESS && StackPop(stack, &num1) == SUCCESS) { // Pops first and second numbers
        if (*operator == '+') {
            result = num1 + num2;
        } else if (*operator == '-') {
            result = num1 - num2;
        } else if (*operator == '*') {
            result = num1 * num2;
        } else if (*operator == '/') {
            result = num1 / num2;
        }

        StackPush(stack, result); // Pushes result back onto stack

        return SUCCESS;
    } else {
        return STANDARD_ERROR;
    }
}

