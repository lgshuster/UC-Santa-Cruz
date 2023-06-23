// Student Libraries
#include "Morse.h"
#include "BinaryTree.h"
#include "Buttons.h"

// Constants
#define TREE_LEVELS 6

// **** Define any module-level, global, or external variables here ****
// Binary tree and root
static Node *BinaryTree;
static Node *root;

// Decoded character and starting level
static char decodedCharacter;
static int levels = TREE_LEVELS;

// Data for Binary Tree
static const char* morseTree = "\0" "EISH54V" "\0" "3UF" "\0" "\0" "\0" "\0" "2ARL" "\0"
        "\0" "\0" "\0" "\0" "WP" "\0" "\0" "J" "\0" "1" "TNDB6" "\0"
        "X" "\0" "\0" "KC" "\0" "\0" "Y" "\0" "\0" "MGZ7" "\0" "Q"
        "\0" "\0" "O" "\0" "8" "\0" "\0" "90";

// Checking buttons presses and times
static uint8_t buttonTimer = 0; // Default state
static MorseEvent morseState = MORSE_EVENT_NONE; // Default state

int MorseInit(void)
{
    ButtonsInit();
    
    BinaryTree = TreeCreate(6, morseTree); // Creates morse tree with 6 levels
    root = BinaryTree; // Sets beginning of tree as root

    if (BinaryTree != NULL) { // If Binary Tree is succesfully generated
        return SUCCESS;
    }
    else {
        return STANDARD_ERROR;
    }
}

char MorseDecode(MorseChar in) {
    if (BinaryTree != NULL) { // Checks if Binary Tree is empty
        switch (in) {
        case MORSE_CHAR_DOT: // If input is dot, goes left side
            BinaryTree = BinaryTree->leftChild;
            levels--; // Subtracts a level
            break;
        case MORSE_CHAR_DASH: // If input is dash, goes right side
            BinaryTree = BinaryTree->rightChild;
            levels--; // Subtracts a level
            break;
        case MORSE_CHAR_END_OF_CHAR: // If no more inputs
            if (BinaryTree->data == '\0') { // Checks if current data is empty
                decodedCharacter = '#'; // Fill empty space with something in order to see it
            }
            else {
                decodedCharacter = BinaryTree->data; // Completes decoded character
            }
            BinaryTree = root; // Resets to original
            levels = TREE_LEVELS; // Resets to original
            return decodedCharacter; // Returns decoded character
            break;
        case MORSE_CHAR_DECODE_RESET: // Resets to original
            BinaryTree = root;
            levels = TREE_LEVELS;
            return SUCCESS;
            break;
        default:
            break;
        }
    
        if (BinaryTree == NULL || levels == 0) { // Error if tree is empty or if levels reaches 0
            return STANDARD_ERROR;
            BinaryTree = root;
            levels = TREE_LEVELS;
        }
        else {
            return SUCCESS;
        }
    }
    
    else { // Resets if Binary Tree is empty
        BinaryTree = root;
        levels = TREE_LEVELS;
        return STANDARD_ERROR;
    }
}

MorseEvent MorseCheckEvents(void) {
    buttonTimer++; // Timer increments every call
    uint8_t buttonEvent = ButtonsCheckEvents(); // Creates new events every call
    
    switch (morseState) {
    case MORSE_EVENT_NONE: 
        if ((buttonEvent & BUTTON_EVENT_4DOWN) != 0) { // Starts with dot
            buttonTimer = 0;
            morseState = MORSE_EVENT_DOT;
        }
        break;
    case MORSE_EVENT_DOT:
        if (buttonTimer >= MORSE_EVENT_LENGTH_DOWN_DOT) { // Becomes a dash
            morseState = MORSE_EVENT_DASH;
        } else if ((buttonEvent & BUTTON_EVENT_4UP) != 0) { // Else become interletter and return dot
            buttonTimer = 0;
            morseState = MORSE_EVENT_INTER_LETTER;
            return MORSE_EVENT_DOT;
        }
        break;
    case MORSE_EVENT_DASH:
        if ((buttonEvent & BUTTON_EVENT_4UP) != 0) { // Become an interletter, return dash
            buttonTimer = 0;
            morseState = MORSE_EVENT_INTER_LETTER;
            return MORSE_EVENT_DASH;
        }
        break;
    case MORSE_EVENT_INTER_LETTER:
        if ((buttonEvent & BUTTON_EVENT_4DOWN) != 0) { // If button 4 is down
            if (buttonTimer >= MORSE_EVENT_LENGTH_UP_INTER_LETTER) { // Become a dot, returns interletter
                buttonTimer = 0;
                morseState = MORSE_EVENT_DOT;
                return MORSE_EVENT_INTER_LETTER;
            } else { // Else goes back to dot
                buttonTimer = 0;
                morseState = MORSE_EVENT_DOT;
            }
        } else if (buttonTimer >= MORSE_EVENT_LENGTH_UP_INTER_LETTER_TIMEOUT) { // Else become and return interword
            buttonTimer = 0;
            morseState = MORSE_EVENT_INTER_WORD;
            return MORSE_EVENT_INTER_WORD;
        }
        break;
    case MORSE_EVENT_INTER_WORD:
        morseState = MORSE_EVENT_NONE; // Default to beginning
        break;
    }
    return MORSE_EVENT_NONE;
}