// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "MatrixMath.h"

// **** Set macros and preprocessor directives ****

// **** Define global, module-level, or external variables here ****

// **** Declare function prototypes ****

int main()
{
    BOARD_Init();

    /******************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     *****************************************************************************/
    // Various matrices for testing
    float mat1[3][3] = { {-2, -71, -43}, {-9, -36, -21}, {-42, -10, -56} };
    float mat2[3][3] = { {-2, -71, -43}, {-9, -36, -21}, {-42, -10, -56} };
    float mat3[3][3] = { {12, 25, -13}, {50, -2, 15}, {-65, 20, 32} };
    float mat4[3][3] = { {21, 52, -31}, {5, -20, 51}, {-56, 2, 23} };
    float mat5[3][3] = { {7, 2, 1}, {0, 3, -1}, {-3, 4, -2} };
    float mat6[3][3] = { {0, -3, -2}, {1, -4, -2}, {-3, 4, 1} };
    
    // Matrices where functions store results
    float outputMatrix1[3][3];
    float outputMatrix2[3][3];
    
   // Used for scalar matrix operations
    float scalar1 = -5;
    float scalar2 = 4;
    
   // Expected matrices of various functions
    float expectedMultMatrix1[3][3] = { {2449, 3128, 3985}, {1224, 2145, 2319}, {2526, 3902, 5152} };
    float expectedMultMatrix2[3][3] = { {1105, 98, 604}, {200, 2670, -1307}, {-3057, -3716, 3771} };
    float expectedScalMultMatrix1[3][3] = { {10, 355, 215}, {45, 180, 105}, {210, 50, 280} };
    float expectedScalMultMatrix2[3][3] = { {48, 100, -52}, {200, -8, 60}, {-260, 80, 128} };
    float expectedAddMatrix1[3][3] = { {-4, -142, -86}, {-18, -72, -42}, {-84, -20, -112} };
    float expectedAddMatrix2[3][3] = { {33, 77, -44}, {55, -22, 66}, {-121, 22, 55} };
    float expectedScalAddMatrix1[3][3] = { {-7, -76, -48}, {-14, -41, -26}, {-47, -15, -61} };
    float expectedScalAddMatrix2[3][3] = { {16, 29, -9}, {54, 2, 19}, {-61, 24, 36} };
    float expectedInverseMatrix1[3][3] = { {-2, 8, -5}, {3, -11, 7}, {9, -34, 21} };
    float expectedInverseMatrix2[3][3] = { {4, -5, -2}, {5, -6, -2}, {-8, 9, 3} };
    float expectedTransposeMatrix1[3][3] = { {-2, -9, -42}, {-71, -36, -10}, {-43, -21, -56} };
    float expectedTransposeMatrix2[3][3] = { {12, 50, -65}, {25, -2, 20}, {-13, 15, 32} };
   
  // Expected values of various functions
    float expectedValue1 = 30696; 
    float expectedValue2 = -80053;
    float expectedValue3 = - 94;
    float expectedValue4 = 42;
   
  //Counter for number of individual passes and total passes
    int numPass;
    int totalPasses = 0;
    
    
  // Test for Matrix Equals
    if (MatrixEquals(mat1, mat2) == 1 && MatrixEquals(mat3, mat4) == 1) {
        numPass = 2;
        totalPasses += 1;
    }
    else if (MatrixEquals(mat1, mat2) == 1 || MatrixEquals(mat3, mat4) == 1) {
        numPass =1;
    }
    else {
        numPass = 0;
    }
    printf("PASSED (%d/2): MatrixEquals()\n", numPass);
    
   
  // Test for Matrix Multiply
    MatrixMultiply(mat1, mat2, outputMatrix1);
    MatrixMultiply(mat3, mat4, outputMatrix2);    
    
    if (MatrixEquals(outputMatrix1, expectedMultMatrix1) == 1 && MatrixEquals(outputMatrix2, 
            expectedMultMatrix2) == 1) {
        numPass = 2;
        totalPasses += 1;
    }
    else if (MatrixEquals(outputMatrix1, expectedMultMatrix1) == 1 || MatrixEquals(outputMatrix2, 
            expectedMultMatrix2) == 1) {
        numPass = 1;
    }
    else {
        numPass = 0;
    }
    printf("PASSED (%d/2): MatrixMultiply()\n", numPass);
    
 
  // Test for Matrix Scalar Multiply
    MatrixScalarMultiply(scalar1, mat1, outputMatrix1);
    MatrixScalarMultiply(scalar2, mat3, outputMatrix2);
    
    if (MatrixEquals(outputMatrix1, expectedScalMultMatrix1) == 1 && MatrixEquals(outputMatrix2, 
            expectedScalMultMatrix2) == 1) {
        numPass = 2;
        totalPasses += 1;
    }
    else if (MatrixEquals(outputMatrix1, expectedScalMultMatrix1) == 1 || MatrixEquals(outputMatrix2, 
            expectedScalMultMatrix2) == 1) {
        numPass =1;
    }
    else {
        numPass = 0;
    }
    printf("PASSED (%d/2): MatrixScalarMultiply()\n", numPass);
    
 
  // Test for Matrix Determinant
    if (MatrixDeterminant(mat1) == expectedValue1 && MatrixDeterminant(mat3) == expectedValue2) {
        numPass = 2;
        totalPasses += 1;
    }
    else if (MatrixDeterminant(mat1) == expectedValue1 || MatrixDeterminant(mat3) == expectedValue2) {
        numPass =1;
    }
    else {
        numPass = 0;
    }
    printf("PASSED (%d/2): MatrixDeterminant()\n", numPass);
    
      
  // Test for Matrix Add
    MatrixAdd(mat1, mat2, outputMatrix1);
    MatrixAdd(mat3, mat4, outputMatrix2);
    
    if (MatrixEquals(outputMatrix1, expectedAddMatrix1) == 1 && MatrixEquals(outputMatrix2, 
            expectedAddMatrix2) == 1) {
        numPass = 2;
        totalPasses += 1;
    }
    else if (MatrixEquals(outputMatrix1, expectedAddMatrix1) == 1 || MatrixEquals(outputMatrix2, 
            expectedAddMatrix2) == 1) {
        numPass =1;
    }
    else {
        numPass = 0;
    }
    printf("PASSED (%d/2): MatrixAdd()\n", numPass);
    

  // Test for Matrix Scalar Add
    MatrixScalarAdd(scalar1, mat1, outputMatrix1);
    MatrixScalarAdd(scalar2, mat3, outputMatrix2);
    
    if (MatrixEquals(outputMatrix1, expectedScalAddMatrix1) == 1 && MatrixEquals(outputMatrix2, 
            expectedScalAddMatrix2) == 1) {
        numPass = 2;
        totalPasses += 1;
    }
    else if (MatrixEquals(outputMatrix1, expectedScalAddMatrix1) == 1 || MatrixEquals(outputMatrix2, 
            expectedScalAddMatrix2) == 1) {
        numPass = 1;
    }
    else {
        numPass = 0;
    }
    printf("PASSED (%d/2): MatrixScalarAdd()\n", numPass);
    
       
  // Test for Matrix Inverse
    MatrixInverse(mat5, outputMatrix1);
    MatrixInverse(mat6, outputMatrix2);
    
    if (MatrixEquals(outputMatrix1, expectedInverseMatrix1) == 1 && MatrixEquals(outputMatrix2, 
            expectedInverseMatrix2) == 1) {
        numPass = 2;
        totalPasses += 1;
    }
    else if (MatrixEquals(outputMatrix1, expectedInverseMatrix1) == 1 || MatrixEquals(outputMatrix2, 
            expectedInverseMatrix2) == 1) {
        numPass = 1;
    }
    else {
        numPass = 0;
    }
    printf("PASSED (%d/2): MatrixInverse()\n", numPass);
    

  // Test for Matrix Transpose
    MatrixTranspose(mat1, outputMatrix1);
    MatrixTranspose(mat3, outputMatrix2);

    
    if (MatrixEquals(outputMatrix1, expectedTransposeMatrix1) == 1 && MatrixEquals(outputMatrix2,
            expectedTransposeMatrix2) == 1) {
        numPass = 2;
        totalPasses += 1;
    }
    else if (MatrixEquals(outputMatrix1, expectedTransposeMatrix1) == 1 || MatrixEquals(outputMatrix2,
            expectedTransposeMatrix2) == 1) {
        numPass = 1;
    }
    else {
        numPass = 0;
    }
    printf("PASSED (%d/2): MatrixTranspose()\n", numPass);
    
    
  // Test for Matrix Trace 
    if (MatrixTrace(mat1) == expectedValue3 && MatrixTrace(mat3) == expectedValue4) {
        numPass = 2;
        totalPasses += 1;
    }
    else if (MatrixTrace(mat1) == expectedValue3 || MatrixTrace(mat3) == expectedValue4) {
        numPass = 1;
    }
    else {
        numPass = 0;
    }
    printf("PASSED (%d/2): MatrixTrace()\n", numPass);

  // Prints total and percentage
    float percentage = (totalPasses / 9.0) * 100;
    printf("%d out of 9 functions passed (%.1f%%).\n ", totalPasses, (double) percentage);    
    
    
  // Test for Matrix Print
    printf("\nOutput of MatrixPrint(): \n");
    MatrixPrint(mat1);
    
    printf("\nExpected output of MatrixPrint(): \n");
    printf("-2.00 -71.00 -43.00 \n-9.00 -36.00 -21.00 \n-42.00 -10.00 -56.00"); 
    /******************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     *****************************************************************************/

    // Returning from main() is bad form in embedded environments. So we sit and spin.
    while (1);
}

