#include <stdio.h>
#include "BOARD.h"
#include "MatrixMath.h"
#include <math.h>

void MatrixPrint(float mat[3][3]) {
    int i, j; // Array index counters
    
    for (i = 0; i < 3; i++) { // First loops counts through rows
        for (j = 0; j < 3; j++) { // Next loop counts through cols
            printf("%.2f ", (double) mat[i][j]);
        }
        printf("\n");
    }
}

int MatrixEquals(float mat1[3][3], float mat2[3][3]) {
    int i, j; // Array index counters
    
    for (i = 0; i < 3; i++) { // First loops counts through rows
        for (j = 0; j < 3; j++) { // Next loop counts through cols
            if (fabs(mat1[i][j] - mat2[i][j]) > FP_DELTA) {
                return 0;
                break;
            }
        }
    }
    return 1;
}

void MatrixAdd(float mat1[3][3], float mat2[3][3], float result[3][3]) {
    int i, j; // Array index counters
    
    for (i = 0; i < 3; i++) { // First loops counts through rows
        for (j = 0; j < 3; j++) { // Next loop counts through cols
            result[i][j] = mat1[i][j] + mat2[i][j];
        }
    }
}

void MatrixScalarAdd(float x, float mat[3][3], float result[3][3]) {
    int i, j; // Array index counters
    
    for (i = 0; i < 3; i++) { // First loops counts through rows
        for (j = 0; j < 3; j++) { // Next loop counts through cols
            result[i][j] = mat[i][j] + x;
        }
    }
    
}

void MatrixScalarMultiply(float x, float mat[3][3], float result[3][3]) {
    int i, j; // Array index counters
    
    for (i = 0; i < 3; i++) { // First loops counts through rows
        for (j = 0; j < 3; j++) { // Next loop counts through cols
            result[i][j] = mat[i][j] * x;
        }
    }
}

void MatrixMultiply(float mat1[3][3], float mat2[3][3], float result[3][3]) {
    int i, j, k; // Array index counters
    
    for (i = 0; i < 3; i++) { // First loops counts through rows
        for (j = 0; j < 3; j++) { // Next loop counts through cols
            for (k = 0; k < 3; k++) {
                result[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
}

float MatrixTrace(float mat[3][3]) {
    int i; // Array index counter
    float sum = 0;
    
    for (i = 0; i < 3; i++) { // Loop counts through rows
        sum += mat[i][i];
    }
    return sum;
}

float MatrixDeterminant(float mat[3][3]) {
    float sum = 0;
    
    sum = (mat[0][0] * ((mat[1][1] * mat[2][2]) - (mat[1][2] * mat[2][1]))) - 
            (mat[0][1] * ((mat[1][0] * mat[2][2]) - (mat[2][0] * mat[1][2]))) + 
            (mat[0][2] * ((mat[1][0] * mat[2][1]) - (mat[1][1] * mat[2][0])));
    
    return sum;
}

void MatrixTranspose(float mat[3][3], float result[3][3]) {
    int i, j;
    
    for (i = 0; i < 3; i++) { // First loops counts through rows
        for (j = 0; j < 3; j++) { // Next loop counts through cols
            result[i][j] = mat[j][i];
        }
    }
}

void MatrixInverse(float mat[3][3], float result[3][3]) { 
    float matrixOfCofactors[3][3];
    float invDeterminant = 1/(MatrixDeterminant(mat));
    
    matrixOfCofactors[0][0] = ((mat[1][1] * mat[2][2]) - (mat[1][2] * mat[2][1]));
    matrixOfCofactors[0][1] = ((mat[1][2] * mat[2][0]) - (mat[1][0] * mat[2][2]));
    matrixOfCofactors[0][2] = ((mat[1][0] * mat[2][1]) - (mat[1][1] * mat[2][0]));
    matrixOfCofactors[1][0] = ((mat[0][2] * mat[2][1]) - (mat[0][1] * mat[2][2]));
    matrixOfCofactors[1][1] = ((mat[0][0] * mat[2][2]) - (mat[0][2] * mat[2][0]));
    matrixOfCofactors[1][2] = ((mat[0][1] * mat[2][0]) - (mat[0][0] * mat[2][1]));
    matrixOfCofactors[2][0] = ((mat[0][1] * mat[1][2]) - (mat[0][2] * mat[1][1]));
    matrixOfCofactors[2][1] = ((mat[0][2] * mat[1][0]) - (mat[0][0] * mat[1][2]));    
    matrixOfCofactors[2][2] = ((mat[0][0] * mat[1][1]) - (mat[0][1] * mat[1][0]));
    
    MatrixTranspose(matrixOfCofactors, result);
        
    MatrixScalarMultiply(invDeterminant, result, result);
}