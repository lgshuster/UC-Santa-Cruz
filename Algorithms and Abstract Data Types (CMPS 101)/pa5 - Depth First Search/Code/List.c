// Name: Leonid Shuster, ID #1616143
// CruzID: lshuster
// Assignment name: pa5
// Role: Creates list with nodes that store data, also has various methods to help sort list

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "List.h"

// Struct for node, contains data and next and previous nodes
typedef struct NodeObj {
    int data;
    struct NodeObj* next;
    struct NodeObj* prev;
} NodeObj;

typedef NodeObj* Node;

// Struct for list, contains front, back and cursor of the list
typedef struct ListObj {
    Node front;
    Node back;
    Node cursor;
} ListObj;

// Allocates memory for node and stores data
Node newNode(int data) {
    Node node = malloc(sizeof(NodeObj));
    assert(node != NULL);
    node->data = data;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

// Frees node
void freeNode(Node* N) {
    if (N != NULL && *N != NULL) { // Checks that node and pointer to node are not null
        free(*N);
        *N = NULL;
    }
}

// Allocates memory for list
List newList(void) {
    List list = malloc(sizeof(ListObj));
    assert(list != NULL);
    list->front = NULL;
    list->back = NULL;
    list->cursor = NULL;
    return list;
}

// Frees list and every node
void freeList(List* pL) {
    clear(*pL);
    free(*pL);
    *pL = NULL;
}

// Returns length of the list
int length(List L) {
    if (L == NULL) { // Checks if list is not null
        fprintf(stderr, "List error: calling length() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    Node current = L->front;

    int listLength = 0;
    while (current != NULL) { // Loops until the end of the list
        listLength++;
        current = current->next; // Moves on to next node
    }
    return listLength;
}

// Returns index of the cursor
int index(List L) {
    if (L == NULL) {
        fprintf(stderr, "List error: calling index() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    int cursorIndex = 0;
    if (L->cursor != NULL) { // If cursor is defined
        Node current = L->front;

        while (current != L->cursor) { // Loops through nodes until cursor is found
            cursorIndex++;
            current = current->next;
        }
    }
    else { // If cursor is undefined
        cursorIndex = -1;
    }
    return cursorIndex;
}

// Returns front node's data
int front(List L) {
    if (L == NULL) {
        fprintf(stderr, "List error: calling front() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (length(L) > 0) { // List length has to be greater than 0
        return L->front->data;
    }
    else {
        return -1;
    }
}

// Returns back node's data
int back(List L) {
    if (L == NULL) {
        fprintf(stderr, "List error: calling back() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (length(L) > 0) {
        return L->back->data;
    }
    else {
        return -1;
    }
}

// Returns node's data at the cursor's position
int get(List L) {
    if (L == NULL) {
        fprintf(stderr, "List error: calling get() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (length(L) > 0 && index(L) >= 0) {
        return L->cursor->data;
    }
    else {
        return -1;
    }
}

// Returns if two lists are equal
int equals(List A, List B) {
    if (A == NULL || B == NULL) {
        fprintf(stderr, "List error: calling equals() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    Node currentList = A->front;
    Node otherList = B->front;

    int isEqualLists = 1;

    if (length(A) != length(B)) { // If the two lists aren't the same length
        isEqualLists = 0;
    }
    else {
        while (currentList != NULL || otherList != NULL) { // Loop until either list ends
            if (currentList->data != otherList->data) { // If one element's data doesn't match the other one
                isEqualLists = 0;
                break;
            }
            currentList = currentList->next;
            otherList = otherList->next;
        }
    }
    return isEqualLists;
}

// Clears list
void clear(List L) {
    if (L == NULL) {
        fprintf(stderr, "List error: calling clear() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    Node current = L->front; // Front of the list
    Node next; // Temp node

    while (current != NULL) { // Loops until end of the list storing next node and freeing current node
        next = current->next;
        freeNode(&current);
        current = next; // Moves onto next node
    }
    L->front = NULL;
    L->back = NULL;
    L->cursor = NULL;
}

// Moves cursor to front of the list
void moveFront(List L) {
    if (L == NULL) {
        fprintf(stderr, "List error: calling moveFront() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (length(L) != 0) {
        L->cursor = L->front;
    }
}

// Moves cursor to back of the list
void moveBack(List L) {
    if (L == NULL) {
        fprintf(stderr, "List error: calling moveBack() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (length(L) != 0) {
        L->cursor = L->back;
    }
}

// Moves cursor one node back
void movePrev(List L) {
    if (L == NULL) {
        fprintf(stderr, "List error: calling movePrev() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (L->cursor != NULL && L->cursor != L->front) { // If cursor is defined and not at the front, move previous
        L->cursor = L->cursor->prev;
    }
    else if (L->cursor != NULL && L->cursor == L->front) { // If cursor is defined and at the front, cursor becomes undefined
        L->cursor = NULL;
    }
}

// Moves cursor one node forward
void moveNext(List L) {
    if (L == NULL) {
        fprintf(stderr, "List error: calling moveNext() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (L->cursor != NULL && L->cursor != L->back) { // If cursor is defined and not at the back, move next
        L->cursor = L->cursor->next;
    }
    else if (L->cursor != NULL && L->cursor == L->back) { // If cursor is defined and at the back, cursor becomes undefined
        L->cursor = NULL;
    }
}

// Adds node with new data to the front of the list
void prepend(List L, int data) {
    if (L == NULL) {
        fprintf(stderr, "List error: calling prepend() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    Node node = newNode(data);

    if (L->front == NULL) { // If list is empty, make the new node the front and back and update cursor
        L->front = node;
        L->back = node;
        L->cursor = L->front;
    }
    else { // If list is not empty, make the new node the front of the list
        node->next = L->front;
        L->front->prev = node;
        L->front = node;
    }
}

// Adds node with new data to the end of the list
void append(List L, int data) {
    if (L == NULL) {
        fprintf(stderr, "List error: calling append() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    Node node = newNode(data);

    if (L->front == NULL) { // If list is empty, make the new node the front and back and update cursor
        L->front = node;
        L->back = node;
        L->cursor = L->front;
    }
    else { // If list is not empty, make the new node the back of the list
        node->prev = L->back;
        L->back->next = node;
        L->back = node;
    }
}

// Adds node with new data before node at cursor's position
void insertBefore(List L, int data) {
    if (L == NULL) {
        fprintf(stderr, "List error: calling insertBefore() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (length(L) > 0 && index(L) >= 0) { // If list length is greater than 0 and cursor is defined
        if (L->cursor->prev == NULL) { // If trying to insert at the beginning of the list, prepend
            prepend(L, data);
        }
        else { // If not at the end of the list
            Node node = newNode(data);
            Node beforeCursor = L->cursor->prev;

            node->prev = beforeCursor;
            node->next = L->cursor;
            beforeCursor->next = node;
            L->cursor->prev = node;
        }
    }
}

// Adds node with new data after node at cursor's position
void insertAfter(List L, int data) {
    if (L == NULL) {
        fprintf(stderr, "List error: calling insertAfter() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (length(L) > 0 && index(L) >= 0) { // If list length is greater than 0 and cursor is defined
        if (L->cursor->next == NULL) { // If trying to insert at the end of the list, append
            append(L, data);
        }
        else { // If not at the end of the list
            Node afterCursor = L->cursor->next;
            Node node = newNode(data);


            node->prev = L->cursor;
            node->next = afterCursor;
            L->cursor->next = node;
            afterCursor->prev = node;
        }
    }
}

// Deletes node at the front of the list
void deleteFront(List L) {
    if (L == NULL) {
        fprintf(stderr, "List error: calling deleteFront() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (length(L) > 0) {
        if (L->cursor == L->front) { // If cursor is at the front, cursor becomes undefined
            L->cursor = NULL;
        }
        Node node = L->front;
        L->front = L->front->next;
        if (L->front == NULL) { // If list becomes empty, clear list
            L->back = NULL;
        }
        else { // If list does not become empty, make front's previous node null
            L->front->prev = NULL;
        }
        freeNode(&node);
    }
}

// Deletes node at the back of the list
void deleteBack(List L) {
    if (L == NULL) {
        fprintf(stderr, "List error: calling deleteBack() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (length(L) > 0) {
        if (L->cursor == L->back) { // If cursor is at the back, cursor becomes undefined
            L->cursor = NULL;
        }
        Node node = L->back;
        L->back = L->back->prev;
        if (L->back == NULL) { // If list becomes empty, clear list
            L->front = NULL;
        }
        else { // If list does not become empty, make back's next node null
            L->back->next = NULL;
        }
        freeNode(&node);
    }
}

// Deletes node at the cursor's position
void delete(List L) {
    if (L == NULL) {
        fprintf(stderr, "List error: calling delete() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (length(L) > 0 && index(L) >= 0) { // If list length is greater than 0 and cursor is defined
        if (L->cursor == L->front) { // If cursor is at the front, delete front node
            deleteFront(L);
        }
        else if (L->cursor == L->back) { // If cursor is at the back, delete back node
            deleteBack(L);
        }
        else { // If cursor is somewhere in between
            Node beforeCursor = L->cursor->prev;
            Node afterCursor = L->cursor->next;

            beforeCursor->next = afterCursor;
            afterCursor->prev = beforeCursor;

            freeNode(&(L->cursor));
        }
    }
}

// Prints list in single line
void printList(FILE *out, List L) {
    if (L == NULL) {
        fprintf(stderr, "List error: calling printList() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    Node current = L->front;

    while (current != NULL) { // Loops until end of the list
        fprintf(out, "%d ", current->data);
        current = current->next;
    }
}

// Returns copied list
List copyList(List L) {
    if (L == NULL) {
        fprintf(stderr, "List error: calling copyList() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    Node current = L->front;

    List list = newList();

    while (current != NULL) { // Loops until end of the list appending data into the new list
        append(list, current->data);
        current = current->next;
    }
    return list;
}

// Returns concatenated list
List concatList(List A, List B) {
    if (A == NULL || B == NULL) {
        fprintf(stderr, "List error: calling concatList() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    Node currentList = A->front;
    Node otherList = B->front;

    List concatList = newList();

    while (currentList != NULL) { // Loops until end of the list appending data into the new list
        append(concatList, currentList->data);
        currentList = currentList->next;
    }

    while (otherList != NULL) { // Loops until end of the list appending data into the same new list
        append(concatList, otherList->data);
        otherList = otherList->next;
    }
    return concatList;
}




