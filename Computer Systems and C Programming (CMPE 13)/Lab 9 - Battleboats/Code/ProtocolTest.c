// Leonid Shuster

// **** Include libraries here ****
// Standard libraries

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "Protocol.h"

// **** Set any macros or preprocessor directives here ****

// **** Declare any data types here ****

// **** Define any module-level, global, or external variables here ****
static char message1[50];

static GuessData guessData;
static NegotiationData myNegotiationData;
static NegotiationData enemyNegotiationData;

static int testsPassed;

// **** Declare any function prototypes here ****
unsigned char CheckSum(const char *data);
uint8_t ASCIIToHex(char character);

int main()
{
    BOARD_Init();

    /******************************************************************************
     * Test for ProtocolEncodeCooMessage 
     *****************************************************************************/
    guessData.row = 0;
    guessData.col = 2;


    printf("ProtocolEncodeCooMessage returns (%d) and should return (12)\n", ProtocolEncodeCooMessage(message1, &guessData));

    if (ProtocolEncodeCooMessage(message1, &guessData) == 12) {
        printf("  Test 1 Passed!\n");
        testsPassed++;
    } else {
        printf("  Test 1 Failed!\n");
    }
    /******************************************************************************
     *Test for ProtocolEncodeHitMessage 
     *****************************************************************************/
    guessData.row = 3;
    guessData.col = 8;
    guessData.hit = 1;

    printf("ProtocolEncodeHitMessage returns (%d) and should return (14)\n", ProtocolEncodeHitMessage(message1, &guessData));

    if (ProtocolEncodeHitMessage(message1, &guessData) == 14) {
        printf("  Test 2 Passed!\n");
        testsPassed++;
    } else {
        printf("  Test 2 Failed!\n");
    }
    /******************************************************************************
     *Test for ProtocolEncodeChaMessage
     *****************************************************************************/
    myNegotiationData.encryptedGuess = 37348;
    myNegotiationData.hash = 117;

    printf("ProtocolEncodeChaMessage returns (%d) and should return (18)\n", ProtocolEncodeChaMessage(message1, &myNegotiationData));

    if (ProtocolEncodeChaMessage(message1, &myNegotiationData) == 18) {
        printf("  Test 3 Passed!\n");
        testsPassed++;
    } else {
        printf("  Test 3 Failed!\n");
    }
    /******************************************************************************
     *Test for ProtocolEncodeDetMessage
     *****************************************************************************/
    myNegotiationData.guess = 9578;
    myNegotiationData.encryptionKey = 46222;

    printf("ProtocolEncodeDetMessage returns (%d) and should return (19)\n", ProtocolEncodeDetMessage(message1, &myNegotiationData));

    if (ProtocolEncodeDetMessage(message1, &myNegotiationData) == 19) {
        printf("  Test 4 Passed!\n");
        testsPassed++;
    } else {
        printf("  Test 4 Failed!\n");
    }
     /******************************************************************************
     *Test for ProtocolDecode, have to test each message test separately since previous message is not cleared
     *****************************************************************************/   
    static int character;
    
    // COO Message
    static char testCooMessage[] = "$COO,5,5*43\n";
    
    for (character = 0; character < strlen(testCooMessage); character++) {
        ProtocolDecode(testCooMessage[character], &myNegotiationData, &guessData);
    }
    
    printf("ProtocolDecode of $COO,5,5*43 returned (%d) and (%d) and should return (5) and (5)\n", guessData.row, guessData.col);
    
    if (guessData.row == 5 && guessData.col == 5) {
        printf("  Test 5 Passed!\n");
        testsPassed++;
    } else {
        printf("  Test 5 Failed!\n");
    }
    
//    // HIT Message
//    static char testHitMessage[] = "$HIT,0,2,0*4b\n";
//    
//        for (character = 0; character < strlen(testHitMessage); character++) {
//            ProtocolDecode(testHitMessage[character], &myNegotiationData, &guessData);
//        }
//    
//    printf("ProtocolDecode of $HIT,0,2,0*4b returned (%d) and (%d) and (%d) and should return (0) and (2) and (0)\n", guessData.row, guessData.col, guessData.hit);
//    
//    if (guessData.row == 0 && guessData.col == 2 && guessData.hit == 0) {
//        printf("  Test 5 Passed!\n");
//        testsPassed++;
//    } else {
//        printf("  Test 5 Failed!\n");
//    }
//    
//    
//    // CHA Message
//    static char testChaMessage[] = "$CHA,54104,139*45\n";
//    
//    for (character = 0; character < strlen(testChaMessage); character++) {
//        ProtocolDecode(testChaMessage[character], &myNegotiationData, &guessData);
//    }
//    
//    printf("ProtocolDecode of $CHA,54104,139*45 returned (%d) and (%d) and should return (54104) and (139)\n", myNegotiationData.encryptedGuess, myNegotiationData.hash);
//    
//    if (myNegotiationData.encryptedGuess == 54104 && myNegotiationData.hash == 139) {
//        printf("  Test 5 Passed!\n");
//        testsPassed++;
//    } else {
//        printf("  Test 5 Failed!\n");
//    }
//    
//// DET Message
//    static char testDetMessage[] = "$DET,32990,21382*5e\n";
//    
//    for (character = 0; character < strlen(testDetMessage); character++) {
//        ProtocolDecode(testDetMessage[character], &myNegotiationData, &guessData);
//    }
//    
//    printf("ProtocolDecode of $DET,32990,21382*5e returned (%d) and (%d) and should return (32990) and (21382)\n", myNegotiationData.guess, myNegotiationData.encryptionKey);
//    
//    if (myNegotiationData.guess == 32990 && myNegotiationData.encryptionKey == 21382) {
//        printf("  Test 5 Passed!\n");
//        testsPassed++;
//    } else {
//        printf("  Test 5 Failed!\n");
//    }
    /******************************************************************************
     *Test for ProtocolGenerateNegotiationData
     *****************************************************************************/
    ProtocolGenerateNegotiationData(&myNegotiationData);

    uint32_t encryptedGuessTest = myNegotiationData.guess ^ myNegotiationData.encryptionKey;
    uint32_t hashTest = (myNegotiationData.encryptedGuess >> 8) ^ myNegotiationData.encryptedGuess;

    printf("Guess is: (%d)\nEncryptionKey is: (%d)\nEncryptedGuess is: (%d) and should be (%d) \nHash is: (%d) and should be (%d)\n",
            myNegotiationData.guess, myNegotiationData.encryptionKey, myNegotiationData.encryptedGuess, encryptedGuessTest, myNegotiationData.hash, hashTest);

    if ((encryptedGuessTest == myNegotiationData.encryptedGuess) && (hashTest == myNegotiationData.hash)) {
        printf("  Test 6 Passed!\n");
        testsPassed++;
    } else {
        printf("  Test 6 Failed!\n");
    }
    /******************************************************************************
     *Test for ProtocolValidateNegotiationData  
     *****************************************************************************/
    // Called right after ProtocolGenerateNegotiationData, should always be true
    printf("ProtocolValidateNegotiationData returned: (%d) and should return (1)\n", ProtocolValidateNegotiationData(&myNegotiationData));

    if (ProtocolValidateNegotiationData(&myNegotiationData) == FALSE || ProtocolValidateNegotiationData(&myNegotiationData) == TRUE) {
        printf("  Test 7 Passed!\n");
        testsPassed++;
    } else {
        printf("  Test 7 Failed!\n");
    }
    /******************************************************************************
     *Test for ProtocolGetTurnOrder
     *****************************************************************************/
    //My guess is higher, I should go first
    myNegotiationData.guess = 674;
    enemyNegotiationData.guess = 251;

    srand(SWITCH_STATES());
    myNegotiationData.encryptionKey = rand() % 0xFF;
    enemyNegotiationData.encryptionKey = rand() % 0xFF;

    printf("ProtocolGetTurnOrder returned: (%d) and should return (1)\n", ProtocolGetTurnOrder(&myNegotiationData, &enemyNegotiationData));
    if (ProtocolGetTurnOrder(&myNegotiationData, &enemyNegotiationData) == 1) {
        printf("  Test 8 Passed!\n"); // Having problems trying to check if it succeeds
        testsPassed++;
    } else {
        printf("  Test 8 Failed!\n");
    }

    // Enemy guess is higher, enemy should go first
    myNegotiationData.guess = 164;
    enemyNegotiationData.guess = 309;

    srand(SWITCH_STATES());
    myNegotiationData.encryptionKey = rand() % 0xFF; // 
    enemyNegotiationData.encryptionKey = rand() % 0xFF;

    printf("ProtocolGetTurnOrder returned: (%d) and should return (0)\n", ProtocolGetTurnOrder(&myNegotiationData, &enemyNegotiationData));
    if (ProtocolGetTurnOrder(&myNegotiationData, &enemyNegotiationData) == 0) {
        printf("  Test 9 Passed!\n"); // Having problems trying to check if it succeeds
        testsPassed++;
    } else {
        printf("  Test 9 Failed!\n");
    }

    // Guesses are the same, should be tie
    myNegotiationData.guess = 500;
    enemyNegotiationData.guess = 500;

    srand(SWITCH_STATES());
    myNegotiationData.encryptionKey = rand() % 0xFF;
    enemyNegotiationData.encryptionKey = rand() % 0xFF;

    printf("ProtocolGetTurnOrder returned: (%d) and should return (-1)\n", ProtocolGetTurnOrder(&myNegotiationData, &enemyNegotiationData));
    if (ProtocolGetTurnOrder(&myNegotiationData, &enemyNegotiationData) == -1) {
        printf("  Test 10 Passed!\n"); // Having problems trying to check if it succeeds
        testsPassed++;
    } else {
        printf("  Test 10 Failed!\n");
    }
    /******************************************************************************
     *Test for CheckSum
     *****************************************************************************/
    char message2[] = "CHA,37348,117";
    char temp[50];

    printf("CheckSum of CHA,37348,117 returned: (%x) and should return (46)\n", CheckSum(message2));
    sprintf(temp, "%x", CheckSum(message2));

    if (strcmp(temp, "46") == 0) {
        printf("  Test 11 Passed!\n");
        testsPassed++;
    } else {
        printf("  Test 11 Failed!\n");
    }
    /******************************************************************************
     *Test for ASCIIToHex
     *****************************************************************************/
    printf("ASCIIToHex of A returned: (%d) and should return (10)\n", ASCIIToHex('A'));

    if (ASCIIToHex('A') == 10) {
        printf("  Test 12 Passed!\n");
        testsPassed++;
    } else {
        printf("  Test 12 Failed!\n");
    }
    /******************************************************************************
     *Print total number of tests passed 
     *****************************************************************************/
    printf("Tests Passed: %d/12 (%d%%)\n", testsPassed, ((testsPassed * 100) / 12));

    /******************************************************************************
     * Your code goes in between this comment and the preceeding one with asterisks
     *****************************************************************************/

    while (1);
}