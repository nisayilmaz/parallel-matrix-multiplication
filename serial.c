#include "matrix_functions.h"

int main(int argc, char const *argv[]) {
    struct matrix input;
    struct matrix weight;
    struct matrix out;
    struct matrix bias;
    
    

    createMatrix(&weight, "weight.txt");
    createMatrix(&input, "input.txt");
    createMatrix(&bias, "bias.txt");

    matrixMultiply(&weight, &input, &out);

    addVector(&out, &bias);
    sigmoidMatrix(&out);
    writeMatrix(&out, "serial_results.txt");

    freeMatrix(&weight);
    freeMatrix(&input);
    freeMatrix(&bias);
    freeMatrix(&out);
    
    return 0;
}



