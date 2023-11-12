#include "matrix_functions.h"

void createMatrix(struct matrix *m, char filename[30]){
    
    //MAKE SURE THE TXT FILE DOESN'T CONTAIN SPACES
    FILE *fileptr = fopen(filename, "r");

    char line[128];
    fgets(line, 128, fileptr);
    int nrow, ncol;
    if(sscanf(line, "%d %d", &nrow, &ncol) < 2) {
        ncol = 1;
    }
    m->row = nrow;
    m->col = ncol;


    float *data = (float *)malloc(m->row*m->col*sizeof(float));
    m->matrixData = (float **)malloc(m->row*sizeof(float*));
    for (int i=0; i<m->row; i++){
        m->matrixData[i] = &(data[m->col*i]);
    }

    char * matrixEntry;
    int fileRow = 0, fileCol = 0;
    while(fgets(line,128,fileptr) != NULL) {
        fileCol = 0;
        matrixEntry = strtok(line, " ");
        while(matrixEntry != NULL) {
            m->matrixData[fileRow][fileCol] = atof(matrixEntry);
            fileCol = fileCol + 1;
            matrixEntry = strtok(NULL, " ");
        }
        fileRow = fileRow + 1;
    }
    fclose(fileptr);

    
}

void createZeroMatrix(struct matrix *m){
    float *data = (float *)malloc(m->row*m->col*sizeof(float));
    m->matrixData = (float **)malloc(m->row*sizeof(float*));
    for (int i=0; i<m->row; i++){
        m->matrixData[i] = &(data[m->col*i]);
    }

    for(int i = 0; i < m->row; i++) {
        for(int j = 0; j< m->col; j++){
            m->matrixData[i][j] = 0;
        }
    }

    
}

void freeMatrix(struct matrix *m) {
    free(m->matrixData[0]);
    free(m->matrixData);
}

void addVector(struct matrix * mat, struct matrix * vector){
    for(int i = 0; i < mat->row; i ++){
        for(int j = 0; j < mat->col; j++){
            mat->matrixData[i][j] += vector->matrixData[i][0];
        }
    }
}

void printMatrix(struct matrix * mat){
    for(int i = 0; i < mat->row; i++) {
        for(int j = 0; j < mat->col; j++) {
            printf(" %.4f ", mat->matrixData[i][j]);
        }
        printf("\n");
    }
}

void sigmoidMatrix(struct matrix * mat){
    for(int i = 0; i < mat->row; i++) {
        for(int j = 0; j < mat->col; j++) {
            mat->matrixData[i][j] = 1/(1 + exp(-(mat->matrixData[i][j])));
        }
        
    }
}

void matrixMultiply(struct matrix * weight, struct matrix * input, struct matrix * output){

    output->row = weight->row;
    output->col = input->col;
    createZeroMatrix(output);
    for(int i = 0; i < weight->row; i++){
        for(int j = 0; j < input->col; j++) {
            for(int k = 0; k < weight->col; k++){
                output->matrixData[i][j] += weight->matrixData[i][k] * input->matrixData[k][j];
            }

        }
    }
    
}
void writeMatrix(struct matrix * mat, char filename[30]){
    FILE * fileptr = fopen(filename, "w");
    fprintf(fileptr, "%d %d\n", mat->row, mat->col);
    for(int i = 0; i < mat->row; i++){
        for(int j = 0; j < mat->col; j++) {
            fprintf(fileptr, "%f ", mat->matrixData[i][j]);
        }
        fprintf(fileptr, "\n");
    } 
    fclose(fileptr);
}