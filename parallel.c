#include "matrix_functions.h"
#include <mpi.h>


int main(int argc, char **argv ) {
    MPI_Init(&argc, &argv);
    double start, end;
    int myid, numprocs, workload, offset;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    struct matrix weight;
    struct matrix input;
    struct matrix bias;
    struct matrix output;

    if(myid == 0){
        start = MPI_Wtime();
        
        createMatrix(&weight, "weight.txt");
        createMatrix(&input, "input.txt");
        createMatrix(&bias, "bias.txt");


        workload = weight.row /numprocs;

        int senddata[] = {workload, weight.col, input.col};
        
        for(int i = 1; i < numprocs; i++){
            MPI_Send(&senddata[0], 3, MPI_INT, i, 0, MPI_COMM_WORLD);

            MPI_Send(&(weight.matrixData[i * workload][0]), workload * weight.col, MPI_INT, i, 1, MPI_COMM_WORLD);

            MPI_Send(&(bias.matrixData[i * workload][0]), workload , MPI_INT, i, 2, MPI_COMM_WORLD);          
            MPI_Send(&(input.matrixData[0][0]), input.row * input.col, MPI_INT, i, 3, MPI_COMM_WORLD);

            
        }

        matrixMultiply(&weight, &input, &output, workload);
        addVector(&output, &bias, workload);
        sigmoidMatrix(&output, workload);
        
        
        for(int i = 1; i < numprocs; i++){
            offset = workload * i;
            MPI_Recv(&(output.matrixData[offset][0]), workload * output.col, MPI_INT, i, 4, MPI_COMM_WORLD, &status);

        }


        writeMatrix(&output, "parallel_results.txt");
        


    }
    else{   
 
        int  senddata[] = {0,0,0};
        MPI_Recv(&senddata[0], 3, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        workload = senddata[0];
        offset = myid * workload;

        weight.col = senddata[1];
        weight.row = workload;

        input.col = senddata[2];
        input.row = weight.col;


        bias.col = 1;
        bias.row = workload;

        createZeroMatrix(&weight);
        createZeroMatrix(&bias);
        createZeroMatrix(&input);
        
        MPI_Recv(&(weight.matrixData[0][0]), workload * weight.col , MPI_INT, 0, 1, MPI_COMM_WORLD, &status);


        MPI_Recv(&(bias.matrixData[0][0]), workload , MPI_INT, 0, 2, MPI_COMM_WORLD, &status);

        MPI_Recv(&(input.matrixData[0][0]), input.row * input.col, MPI_INT, 0, 3, MPI_COMM_WORLD, &status);

        matrixMultiply(&weight, &input, &output, weight.row);
        addVector(&output, &bias,output.row);
        sigmoidMatrix(&output, output.row);

        MPI_Send(&(output.matrixData[0][0]), workload * output.col, MPI_INT, 0, 4, MPI_COMM_WORLD);


    }
    freeMatrix(&weight);
    freeMatrix(&input);
    freeMatrix(&output);
    freeMatrix(&bias);
    if(myid == 0) {
        end = MPI_Wtime();
        printf("Parallel implementation with %d processes lasted %f seconds.\n", numprocs, end - start);
    }
    MPI_Finalize();

    
    return 0;
}
