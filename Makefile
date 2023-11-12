all: serial parallel

matrix_functions.o:
	gcc matrix_functions.c -c -o matrix_functions.o 


parallel: parallel.c matrix_functions.o
	mpicc parallel.c matrix_functions.o -o parallel -lm

serial: serial.c matrix_functions.o
	gcc serial.c matrix_functions.o -o serial -lm

clean:
	rm -f parallel serial matrix_functions.o parallel_results.txt serial_results.txt