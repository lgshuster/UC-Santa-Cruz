#include "Game.h"
#include "Player.h"
#include <stdio.h>
#include <string.h>

#define FALSE 0
#define TRUE 1

typedef struct {
    char title[GAME_MAX_ROOM_TITLE_LENGTH + 1];
    char description[GAME_MAX_ROOM_DESC_LENGTH + 1];
    uint8_t exits[4]; // North, East, South, West
} Room;

static Room RoomData;

static void FileRead(FILE *filePointer, uint8_t room);

int GameGoNorth(void)
{
    if (RoomData.exits[0] == 0) { // Checks for north exit
        return STANDARD_ERROR;
    } else {
        char way[21];
        FILE *filePointer;

        sprintf(way, "RoomFiles/room%i.txt", RoomData.exits[0]);
        filePointer = fopen(way, "rb"); // Opens file with current room

        if (filePointer == NULL) {
            FATAL_ERROR();
        }

        FileRead(filePointer, RoomData.exits[0]);
        fclose(filePointer);

        return SUCCESS;
    }
}

int GameGoEast(void) // Same as GameGoNorth
{
    if (RoomData.exits[1] == 0) {
        return STANDARD_ERROR;
    } else {
        char way[21];
        FILE *filePointer;

        sprintf(way, "RoomFiles/room%i.txt", RoomData.exits[1]);
        filePointer = fopen(way, "rb");

        if (filePointer == NULL) {
            FATAL_ERROR();
        }

        FileRead(filePointer, RoomData.exits[1]);
        fclose(filePointer);

        return SUCCESS;
    }
}

int GameGoSouth(void) // Same as GameGoNorth
{
    if (RoomData.exits[2] == 0) {
        return STANDARD_ERROR;
    } else {
        char way[21];
        FILE *filePointer;

        sprintf(way, "RoomFiles/room%i.txt", RoomData.exits[2]);
        filePointer = fopen(way, "rb");

        if (filePointer == NULL) {
            FATAL_ERROR();
        }

        FileRead(filePointer, RoomData.exits[2]);
        fclose(filePointer);

        return SUCCESS;
    }
}

int GameGoWest(void) // Same as GameGoNorth
{
    if (RoomData.exits[3] == 0) {
        return STANDARD_ERROR;
    } else {
        char way[21];
        FILE *filePointer;

        sprintf(way, "RoomFiles/room%i.txt", RoomData.exits[3]);
        filePointer = fopen(way, "rb");

        if (filePointer == NULL) {
            FATAL_ERROR();
        }

        FileRead(filePointer, RoomData.exits[3]);
        fclose(filePointer);

        return SUCCESS;
    }
}

int GameInit(void)
{
    char way[21];
    FILE *filePointer;
    sprintf(way, "RoomFiles/room%i.txt", STARTING_ROOM);
    filePointer = fopen(way, "rb");

    if (filePointer == NULL) {
        FATAL_ERROR();
        return STANDARD_ERROR;
    }


    FileRead(filePointer, STARTING_ROOM);

    fclose(filePointer);
    return SUCCESS;
}

int GameGetCurrentRoomTitle(char *title)
{
    return strlen(RoomData.title);
}

int GameGetCurrentRoomDescription(char *desc)
{
    return strlen(RoomData.description);
}

uint8_t GameGetCurrentRoomExits(void)
{
    uint8_t exits = 0;

    int i;
    for (i = 0; i < 4; i++) {
        if (RoomData.exits[i] != 0) {
            exits |= (1 << (3 - i));
        }
    }

    return exits;
}

static void FileRead(FILE *filePointer, uint8_t room)
{
    uint8_t decryptKey = room + DECRYPTION_BASE_KEY;
    uint8_t sizeOfTitle;

    fread(&sizeOfTitle, 1, 1, filePointer); // Gets size of title
    sizeOfTitle ^= decryptKey; // Decrypts size of title

    fread(&RoomData.title, 1, sizeOfTitle, filePointer); // Gets title

    int i;
    for (i = 0; i < sizeOfTitle; i++) { // Decrypts every element
        RoomData.title[i] ^= (DECRYPTION_BASE_KEY + room);
    }
    RoomData.title[sizeOfTitle] = '\0'; // Append with null terminator

    uint8_t validRoom = FALSE; // Initializes boolean condition
    while (validRoom == FALSE) { // Loops until room becomes false
        uint8_t numNeededItems; // Keeps track of number of items
        fread(&numNeededItems, 1, 1, filePointer); // Stores number of needed items
        numNeededItems ^= decryptKey;

        if (numNeededItems != 0) { // Room requires items
            uint8_t item;
            uint8_t checkForItem;

            uint8_t i;
            for (i = 0; i < numNeededItems; i++) { // Checks if we have item needed
                fread(&item, 1, 1, filePointer);
                item ^= decryptKey;
                if (FindInInventory(item) == STANDARD_ERROR && checkForItem == TRUE) { // If item was not found
                    checkForItem = FALSE;
                }
            }
            if (checkForItem == TRUE) {
                validRoom = TRUE;
            }
        } else { // Room does not require any items
            validRoom = 1;
        }
        uint8_t sizeOfDescription;
        fread(&sizeOfDescription, 1, 1, filePointer); // Gets size of descriptions
        sizeOfDescription ^= decryptKey; // Decrypts size of description

        if (validRoom == FALSE) {
            fseek(filePointer, sizeOfDescription, SEEK_CUR); // If room is not found
        } else {
            fread(&RoomData.description, 1, sizeOfDescription, filePointer);
            int i;
            for (i = 0; i < sizeOfDescription; i++) {
                RoomData.description[i] ^= (DECRYPTION_BASE_KEY + room); // Decrypts every element
            }
            RoomData.description[sizeOfDescription] = '\0'; // Append with null terminator
        }

        uint8_t itemsInRoom;
        fread(&itemsInRoom, 1, 1, filePointer); // Reads items in room
        itemsInRoom ^= decryptKey; // Decrypts items in room

        if (validRoom == FALSE) {
            fseek(filePointer, itemsInRoom, SEEK_CUR); // If room is not found
        } else {
            if (itemsInRoom != TRUE) { // If there are items in the room
                uint8_t item;
                int i;
                for (i = 0; i < itemsInRoom; i++) { // Read the item, decrypt it, and add to inventory
                    fread(&item, 1, 1, filePointer);
                    item ^= decryptKey;
                    AddToInventory(item);
                }
            }
        }
        if (validRoom == FALSE) {
            fseek(filePointer, 4, SEEK_CUR); // If room is not found
        } else {
            int i;
            uint8_t exits;
            for (i = 0; i < 4; i++) { // Puts 0 or number whether there is an exit
                fread(&exits, 1, 1, filePointer);
                exits ^= decryptKey;
                RoomData.exits[i] = exits;
            }
        }
    }
}