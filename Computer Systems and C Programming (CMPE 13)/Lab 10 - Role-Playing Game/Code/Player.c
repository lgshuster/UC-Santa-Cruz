#include "Player.h"

static int inventory[INVENTORY_SIZE];
static int numItems = 0; // Current index

int AddToInventory(uint8_t item)
{
    if (numItems == INVENTORY_SIZE) { // Checks if at max items
        return STANDARD_ERROR;
    } else {
        inventory[numItems] = item; // Adds item at current index
        numItems++; // Moves onto next index
        return SUCCESS;
    }
}

int FindInInventory(uint8_t item)
{
    int position;

    for (position = 0; position < INVENTORY_SIZE; position++) { // Checks if item is located in the array
        if (inventory[position] == item) {
            return SUCCESS;
        }
    }
    return STANDARD_ERROR;
}