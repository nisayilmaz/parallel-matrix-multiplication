#ifndef MATRIX_FUNCTIONS_H
#define MATRIX_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct matrix
{
    float ** matrixData;
    int row;
    int col;
};

void createMatrix(struct matrix *m, char filename[30]);
void createZeroMatrix(struct matrix *m);
void freeMatrix(struct matrix *m);
void addVector(struct matrix * mat, struct matrix * vector);
void printMatrix(struct matrix * mat);
void sigmoidMatrix(struct matrix * mat);
void matrixMultiply(struct matrix * weight, struct matrix * input, struct matrix * output);
void writeMatrix(struct matrix * mat, char filename[30]);


#endif 