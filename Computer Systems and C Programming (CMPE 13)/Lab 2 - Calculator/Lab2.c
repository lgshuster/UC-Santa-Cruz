// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <math.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries


// **** Set macros and preprocessor directives ****

// **** Define global, module-level, or external variables here ****

// **** Declare function prototypes ****
/******************************************************************************
 * Your function prototypes go in between this comment and the following one with asterisks.
 *****************************************************************************/
double Average(double firstOperand, double secondOperand);
double absoluteValue(double firstOperand);
double fahrToCelc(double firstOperand);
double celcToFahr(double firstOperand);
double Tan(double firstOperand);
double Round(double operand);




/******************************************************************************
 * Your function prototypes go in between this comment and the preceding one with asterisks
 *****************************************************************************/


// If this code is used for testing, rename main to something we can call from our testing code.
#ifndef LAB2_TESTING

int main(void)
{
    BOARD_Init();
#else

int their_main(void)
{
#endif // LAB2_TESTING

    /******************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     *****************************************************************************/
    double firstOperand;
    double secondOperand;
    int numOperands;
    char operation;
    double result;

    printf("Welcome to Leon's calculator program!\n");

    /******************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     *****************************************************************************/
    while (1) {

        printf("Enter a mathematical operation to perform (*,/,+,-,v,a,c,f,t,r): ");
        scanf("%c", &operation);

        // Sets operation to 0 if operation is invalid
        if (operation != '*' && operation != '/' && operation != '+' && operation != '-' && operation != 'v' && operation != 'a'
                && operation != 'c' && operation != 'f' && operation != 't' && operation != 'r') {
            operation = '0';
        }

        // Checks if input is valid
        if (operation != '0') {
            printf("\nEnter first operand: "); 
            scanf("%f%*c", &firstOperand); 
            numOperands = 1; // Counts first operand

            // Checks if operation needs one or two operands
            if (operation != 'a' && operation != 'c' && operation != 'f' && operation != 't' && operation != 'r') {
                printf("Enter second operand: ");
                scanf("%f%*c", &secondOperand); 
                numOperands = 2; // Counts second operand
            } 
            // Starts checking for type of operation
            if (operation == '+') {
                result = firstOperand + secondOperand;
            } else if (operation == '-') {
                result = firstOperand - secondOperand;
            } else if (operation == '*') {
                result = firstOperand * secondOperand;
            } else if (operation == '/') {
                result = firstOperand / secondOperand;
            } else if (operation == 'v') {
                result = Average(firstOperand, secondOperand);
            } else if (operation == 'a') {
                result = absoluteValue(firstOperand);
            } else if (operation == 'c') {
                result = fahrToCelc(firstOperand);
            } else if (operation == 'f') {
                result = celcToFahr(firstOperand);
            } else if (operation == 't') {
                result = Tan(firstOperand);
            } else if (operation == 'r') {
                result = Round(firstOperand);
            }
            // Checks for number of operands
            if (numOperands == 2) {
                printf("Result of (%.6f %c %.6f): %.6f\n", firstOperand, operation, secondOperand, result);
            } else if (numOperands == 1) { // Prints different messages depending on operation
                if (operation == 'a') {
                    printf("Result of |%.6f|: %.6f\n", firstOperand, result);
                } else if (operation == 'f' || operation == 'c') {
                    printf("Result of %.6f deg->%c: %.6f\n", firstOperand, operation, result);
                } else if (operation == 't') {
                    printf("Result of tan(%.6f): %.6f\n", firstOperand, result);
                } else if (operation == 'r') {
                    printf("Result of round(%.6f): %.6f\n", firstOperand, result);
                }
            }
        } else {
            printf("\nInput is invalid!\n"); // Prints message for invalid input
        }
    }
}

/********************************************************************************
 * Define the Absolute Value function here.
 ********************************************************************************/
double absoluteValue(double firstOperand)
{
    if (firstOperand < 0) {
        firstOperand *= -1;
    }
    return firstOperand;
}

/*********************************************************************************
 * Define the Fahrenheit to Celsius function here.
 ********************************************************************************/
double fahrToCelc(double firstOperand)
{
    double celc = (firstOperand - 32) * (5.0 / 9.0);
    return celc;
}

/*********************************************************************************
 * Define the Celsius to Fahrenheit function here.
 ********************************************************************************/
double celcToFahr(double firstOperand)
{
    double fahr = (firstOperand * (9.0 / 5.0)) + 32;
    return fahr;
}

/********************************************************************************
 * Define the Average function here.
 *******************************************************************************/
double Average(double firstOperand, double secondOperand)
{
    double average = (firstOperand + secondOperand) / 2;
    return average;
}

/*********************************************************************************
 * Define the Tangent function that takes input in degrees (Note: Your tangent 
 * function relies on a call from the tangent function of math.h which uses 
 * radians).
 ********************************************************************************/
double Tan(double firstOperand)
{
    double radians = firstOperand * (M_PI / 180);
    double tan = tan(radians);
    return tan;
}

/*********************************************************************************
 * Define the Round function here.
 * In order to receive the extra credit your calculator MUST ALSO CORRECTLY utilize
 * this function.
 ********************************************************************************/
double Round(double operand)
{
    if (operand > 0) {
        operand = (int) (operand + 0.5);
    } else {
        operand = (int) (operand - 0.5);
    }
    return operand;
}