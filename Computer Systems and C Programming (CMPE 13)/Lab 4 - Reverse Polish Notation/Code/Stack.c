#include "Stack.h"

void StackInit(struct Stack *stack)
{
    stack->currentItemIndex = -1;
    stack->initialized = TRUE;
}

int StackPush(struct Stack *stack, float value)
{
    if (StackIsFull(stack) == 0 && stack->initialized == 1) { // If stack is not full and stack is initialized
        stack->currentItemIndex++; // Goes up the stack
        stack->stackItems[stack->currentItemIndex] = value; // Makes the current spot of the stack equal to the value
        return SUCCESS;
    } else { // If stack is full or not initialized, return error
        return STANDARD_ERROR;
    }
}

int StackPop(struct Stack *stack, float *value)
{
    if (StackIsEmpty(stack) == 0 && stack->initialized == 1) { // If stack is not empty and stack is initialized
        *value = stack->stackItems[stack->currentItemIndex]; // Makes value equal to the top of the stack
        stack->currentItemIndex--; // Goes down the stack
        return SUCCESS;
    } else { // If stack is empty or stack is not initialized
        return STANDARD_ERROR;
    }
}

int StackIsEmpty(const struct Stack *stack)
{
    if (stack->currentItemIndex == -1 && stack->initialized == 1) { // If stack is empty and initialized 
        return TRUE;
    } else { // If stack isn't empty or initialized
        return FALSE;
    }
}

int StackIsFull(const struct Stack *stack)
{
    if (stack->currentItemIndex == STACK_SIZE - 1 && stack->initialized == 1) { // If index is at stack size and stack is initialized 
        return TRUE;
    } else { // If stack is not full or initialized
        return FALSE;
    }
}

int StackGetSize(const struct Stack *stack)
{
    if (stack->initialized == 0) { // If stack is not initialized
        return SIZE_ERROR;
    } else if (StackIsEmpty(stack) == 1 && stack->initialized == 1) { // If stack is empty and initialized
        return 0;
    } else { // If stack isn't empty
        return stack->currentItemIndex + 1;
    }
}
