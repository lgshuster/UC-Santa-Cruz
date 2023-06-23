//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

// Student libraries
#include "BinaryTree.h"


Node *TreeCreate(int level, const char *data)
{
    struct Node *newNode = (struct Node*) malloc(sizeof(struct Node)); // Allocates memory for new node

    if (newNode == NULL) { // If malloc returns an error
        return NULL;
    }

    newNode->data = data[0]; // Saves the current character into the node's data

    if (level > 1) { // Populates children until it reaches the last level; left children first
        newNode-> leftChild = TreeCreate(--level, ++data); // Left child is the character next to the current one
        newNode ->rightChild = TreeCreate(--level, data + (1 << (--level))); // Right child is located 2^(level-1) away

        if (newNode ->leftChild == NULL || newNode ->rightChild == NULL) { // Checks if children are null
            return NULL;
        }
    } else { // If last level is reached, children have nothing
        newNode ->leftChild = NULL;
        newNode ->rightChild = NULL;
    }
    return newNode;
}

Node *GetLeftChild(Node *root)
{
    if (root != NULL && root ->leftChild != NULL) { // If root is not null and it has a left child
        return root ->leftChild;
    } else {
        return NULL;
    }
}

Node *GetRightChild(Node *root)
{
    if (root != NULL && root ->rightChild != NULL) { // If root is not null and it has a right child
        return root ->rightChild;
    } else {
        return NULL;
    }
}

void PrintTree(Node *node, int space)
{
    if (node != NULL) {
        space += LEVEL_SPACES;

        PrintTree(node ->rightChild, space); // First prints right child

        printf("\n");

        int i;
        for (i = LEVEL_SPACES; i < space; i++) {
            printf(" ");
        }

        printf("%c\n", node ->data);

        PrintTree(node -> leftChild, space); // Then left child
    }
    else {
        printf(" ");
    }
}

