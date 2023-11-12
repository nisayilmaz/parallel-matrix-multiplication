#include "matrix_functions.h"
#include <mpi.h>


void main(int argc, char **argv ) {
    int myid, numprocs, workload, offset;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    struct matrix weight;
    struct matrix input;
    struct matrix bias;
    struct matrix output;

    if(myid == 0){
        printf("master process\n");
        struct matrix bias;

        createMatrix(&weight, "weight.txt");
        createMatrix(&input, "input.txt");
        createMatrix(&bias, "bias.txt");
        
        output.row = weight.row;
        output.col = input.col;
        createZeroMatrix(&output);
        

        workload = weight.row /numprocs;
        offset = workload;

        for(int i = 1; i < numprocs; i++){
            MPI_Send(&workload, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&weight.col, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&offset, 1, MPI_INT, i, 2, MPI_COMM_WORLD);
            MPI_Send(&(weight.matrixData[offset][0]), workload * weight.col, MPI_INT, i, 3, MPI_COMM_WORLD);

            MPI_Send(&input.row, 1, MPI_INT, i, 4, MPI_COMM_WORLD);
            MPI_Send(&input.col, 1, MPI_INT, i, 5, MPI_COMM_WORLD);
            MPI_Send(&(input.matrixData[0][0]), input.row * input.col, MPI_INT, i, 6, MPI_COMM_WORLD);

            MPI_Send(&(bias.matrixData[offset][0]), workload , MPI_INT, i, 7, MPI_COMM_WORLD);          
            
            offset += workload;
        }

        for(int i = 0; i < workload; i++){
            for(int j = 0; j < input.col; j++) {
                for(int k = 0; k < weight.col; k++){
                    output.matrixData[i][j] += weight.matrixData[i][k] * input.matrixData[k][j];
                }
                output.matrixData[i][j]  = 1/(1 + exp(-(output.matrixData[i][j] + bias.matrixData[i][0])));
            }
        }
        
        for(int i = 1; i < numprocs; i++){
            MPI_Recv(&offset, 1, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(&(output.matrixData[offset][0]), workload * output.col, MPI_INT, i, 3, MPI_COMM_WORLD, &status);

           
        }
        writeMatrix(&output, "parallel_results.txt");


    }
    else{    

        MPI_Recv(&workload, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&weight.col, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
        weight.row = workload;
        bias.col = 1;
        bias.row = workload;

        createZeroMatrix(&weight);
        createZeroMatrix(&bias);

        MPI_Recv(&(weight.matrixData[0][0]), workload * weight.col , MPI_INT, 0, 3, MPI_COMM_WORLD, &status);


        MPI_Recv(&(bias.matrixData[0][0]), workload , MPI_INT, 0, 7, MPI_COMM_WORLD, &status);

        MPI_Recv(&input.row, 1, MPI_INT, 0, 4, MPI_COMM_WORLD, &status);
        MPI_Recv(&input.col, 1, MPI_INT, 0, 5, MPI_COMM_WORLD, &status);
        createZeroMatrix(&input);
        MPI_Recv(&(input.matrixData[0][0]), input.row * input.col, MPI_INT, 0, 6, MPI_COMM_WORLD, &status);

        matrixMultiply(&weight, &input, &output);
        addVector(&output, &bias);
        sigmoidMatrix(&output);
        
        MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&(output.matrixData[0][0]), workload * output.col, MPI_INT, 0, 3, MPI_COMM_WORLD);
        

    }
    freeMatrix(&weight);
    freeMatrix(&input);
    freeMatrix(&output);
    freeMatrix(&bias);

    MPI_Finalize();
}
