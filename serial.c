#include "matrix_functions.h"
#include <sys/time.h>

int main(int argc, char  **argv) {

    struct matrix input;
    struct matrix weight;
    struct matrix out;
    struct matrix bias;
    double start, end;
    struct timeval start_time, end_time;
    double elapsed_time;

    gettimeofday(&start_time, NULL);

    createMatrix(&weight, "weight.txt");
    createMatrix(&input, "input.txt");
    createMatrix(&bias, "bias.txt");


    matrixMultiply(&weight, &input, &out, weight.row);

    addVector(&out, &bias, out.row);
    sigmoidMatrix(&out, out.row);
   
    writeMatrix(&out, "serial_results.txt");
    freeMatrix(&weight);
    freeMatrix(&input);
    freeMatrix(&bias);
    freeMatrix(&out);
    gettimeofday(&end_time, NULL);
    elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
                   (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
    printf("Serial implementation lasted %f seconds.\n", elapsed_time);


    return 0;
}



