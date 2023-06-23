// **** Include libraries here ****
// Standard libraries
#include <string.h>
#include <stdio.h>
#include <math.h>

//CMPE13 Support Library
#include "UNIXBOARD.h"



// User libraries
#include "Game.h"


// **** Set any macros or preprocessor directives here ****

// **** Declare any data types here ****

// **** Define any global or external variables here ****

// **** Declare any function prototypes here ****
void UpdateTerminal(void);

int main()
{



    /******************************** Your custom code goes below here ********************************/
    if (GameInit() == STANDARD_ERROR) {
        FATAL_ERROR();
    } else {
        UpdateTerminal();

        while (1) {
            char input = getchar();

            if (input == 'n') { // Checks for north input
                GameGoNorth();
                UpdateTerminal();
            } else if (input == 'e') { // Checks for east input
                GameGoEast();
                UpdateTerminal();
            } else if (input == 's') { // Checks for south input
                GameGoSouth();
                UpdateTerminal();
            } else if (input == 'w') { // Checks for west input
                GameGoWest();
                UpdateTerminal();
            } else if (input == 'q') { // Checks for quit
                exit(EXIT_SUCCESS);
            }
        }
    }
    /**************************************************************************************************/
}

void UpdateTerminal(void)
{
    printf("\033[2J");

    // Get and print title
    char title[GAME_MAX_ROOM_TITLE_LENGTH + 1];
    GameGetCurrentRoomTitle(title);
    printf("\033[41m\033[34m%s\033[0m\n", title);

    // Get and print description
    char desc[GAME_MAX_ROOM_DESC_LENGTH + 1];
    GameGetCurrentRoomDescription(desc);
    printf("%s", desc);

    // Moves cursor to print out directions
    printf("\033[H");
    printf("\033[45B");
    printf("\033[3A\033[8C");

    if (GameGetCurrentRoomExits() & GAME_ROOM_EXIT_NORTH_EXISTS) { // Print north
        printf("\033[32mNORTH\033[0m\n");
    } else {
        printf("\033[31mNORTH\033[0m\n");
    }

    if (GameGetCurrentRoomExits() & GAME_ROOM_EXIT_EAST_EXISTS) { // Print east
        printf("\033[32mEAST\033[0m\n");
    } else {
        printf("\033[31mEAST\033[0m\n");
    }
    printf("\033[8C");
    
    if (GameGetCurrentRoomExits() & GAME_ROOM_EXIT_SOUTH_EXISTS) { // Print south
        printf("\033[32mSOUTH\033[0m\n");
    } else {
        printf("\033[31mSOUTH\033[0m\n");
    }
    
    if (GameGetCurrentRoomExits() & GAME_ROOM_EXIT_WEST_EXISTS) { // Print west
        printf("\033[32mWEST\033[0m");
    } else {
        printf("\033[31mWEST\033[0m");
    }
    printf("\033[12C");

    printf("Enter Direction to Travel (n,e,s,w) or q to quit followed by enter:");
}