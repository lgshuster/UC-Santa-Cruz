#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "BOARD.h"
#include "LinkedList.h"

ListItem *LinkedListNew(char *data)
{
    ListItem *newItem = malloc(sizeof (ListItem));

    if (newItem != NULL) {
        newItem->data = data;
        newItem->nextItem = NULL;
        newItem->previousItem = NULL;

        return newItem;
    } else {
        return NULL;
    }
}

char *LinkedListRemove(ListItem *item)
{
    if (item != NULL) {
        ListItem *previous = item->previousItem;
        ListItem *next = item->nextItem;

        // If there are previous items
        if (previous != NULL) {
            previous->nextItem = next;
        }

        // If there are terms in front
        if (next != NULL) {
            next->previousItem = previous;
        }

        //Save item
        char *data = item->data;

        // Then free up space on the heap
        free(item);

        return data;
    }
    else {
        return NULL;
    }
}

ListItem *LinkedListGetFirst(ListItem *list)
{
    if (list != NULL) {
        ListItem *currentItem = list;

        // Keep going back until item has no previous item
        while (currentItem->previousItem != NULL) {
            currentItem = currentItem->previousItem;
        }
        return currentItem;
    }
    else {
        return NULL;
    }
}

int LinkedListSize(ListItem *list)
{
    if (list != NULL) {
        ListItem *counter = LinkedListGetFirst(list);

        int items = 1;

        // Keep iterating until nextItem isn't NULL
        while (counter->nextItem != NULL) {
            counter = counter->nextItem;
            items++;
        }

        return items;
    }
    else {
        return NULL;
    }
}

ListItem *LinkedListCreateAfter(ListItem *item, char *data)
{
    ListItem *newItem = malloc(sizeof (ListItem));

    if (newItem != NULL) {
        newItem->data = data;

        if (item != NULL) {
            ListItem *afterNewItem = item->nextItem; // Saves the current item's next item

            newItem->previousItem = item; // Makes the new previous term the old item
            newItem->nextItem = afterNewItem; // Makes the new next term the old current item
            item->nextItem = newItem;

            // If current item has items on either side
            if (afterNewItem != NULL) {
                afterNewItem->previousItem = newItem;
            }
        }
        else {
            // Creates new list if new item is NULL
            newItem->nextItem = NULL;
            newItem->previousItem = NULL;
        }
        return newItem;
    }
    else {
        return NULL;
    }
}

int LinkedListSwapData(ListItem *firstItem, ListItem *secondItem)
{
    if (firstItem != NULL && secondItem != NULL) {

        char *temp = firstItem->data; // Temporarily keeps firstItem 
        firstItem->data = secondItem->data; // firstItem becomes secondItem
        secondItem->data = temp; // SecondItem becomes firstItem

        return SUCCESS;
    }
    else {
        return STANDARD_ERROR;
    }
}

int LinkedListSort(ListItem *list)
{
    if (list != 0) {
        int listSize = LinkedListSize(list);

        ListItem *firstItem = LinkedListGetFirst(list); // Gets firstItem
        ListItem *secondItem; // Gets secondItem

        char *secondData, *firstData;
        int i, j;

        for (i = 0; i < listSize - 1; i++) {
            secondItem = firstItem;

            for (j = i + 1; j < listSize; j++) {
                secondItem = secondItem->nextItem; // Makes secondItem the next item
                // Stores secondItem's data in secondData if not null

                secondData = (secondItem->data ? secondItem->data : "\0");
                firstData = (firstItem->data ? firstItem->data : "\0");

                // If length of secondData is less than firstData's
                if (strlen(secondData) < strlen(firstData)) {
                    LinkedListSwapData(secondItem, firstItem);
                }
                    // If secondData is less than firstData and their lengths are the same
                else if (strcmp(secondData, firstData) < 0 && strlen(secondData) == strlen(firstData)) {
                    LinkedListSwapData(secondItem, firstItem);
                }
            }
            firstItem = firstItem->nextItem; // Moves on to the next item for comparison
        }

        return SUCCESS;
    }
    else {
        return STANDARD_ERROR;
    }
}

int LinkedListPrint(ListItem *list)
{
    if (list != NULL) {
        ListItem *counter;

        if (list->previousItem != NULL) { // Checks if item is first item, if not get first item
            counter = LinkedListGetFirst(list);
        } else {
            counter = list;
        }

        printf("[%s", counter->data);

        while (counter->nextItem != NULL) { // Keeps printing until there are no more nextItems
            counter = counter->nextItem;
            printf(", %s", counter->data);
        }
        printf("]\n");

        return SUCCESS;
    }
    else {
        return STANDARD_ERROR;
    }
}