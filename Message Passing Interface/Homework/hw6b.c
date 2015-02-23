/*
 * License: MIT 
 * Author: Christian Benjamin Ries
 * Source: https://github.com/cbries/utilities
 */

/* 
** DESCRIPTION: Rahmen für MPI-Programme
** COMPILE:     mpicc -o hw6b hw6b.c
** RUN:         mpirun -np 4 -machinefile machines hw6b arg1 arg2 ...
*/
/*
! Homework description:
! Assume each processor has a real array A(1:n). 
! Perform a right circular shift on p processors. Your program
! should run correctly for any n and p, but only
! turn in your homework verifying that your program 
! works correctly for n=3 and p=4.
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <mpi.h>

#define DIMENSION 3
#define TAG 123

int main (int argc, char **argv)
{
	int i, j;
	int myrank, nprocs;
	int namelen;
	char name[MPI_MAX_PROCESSOR_NAME];

	double A[DIMENSION] = {0};
	double B[DIMENSION] = {0};
	
	MPI_Status status;
	
	/* MPI initialisieren und Argumente setzen */ 
	MPI_Init(&argc, &argv);

	/* Anzahl der Prozesse erfragen */
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

	/* Eigenen Rang erfragen */
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	/* Rechnernamen erfragen */
	MPI_Get_processor_name(name, &namelen);

	srand(time(NULL));
	for(i=0; i<DIMENSION; i++) {
		A[i] = (rand()%100) / 100.0f + myrank;
	}

	printf("Before the shift on processor %d A=", myrank);
	for(i=0; i<DIMENSION; i++) {
		printf("%.2f   ", A[i]);
	}
	printf("\n");
	
	i = myrank - 1; // source is 'left' processor
	j = myrank + 1; // destination is 'right' processor

	if(myrank == nprocs-1) {
		j = 0;
		i = myrank - 1;
	}
	
	if(myrank == 0) {
		j = myrank + 1;
		i = nprocs - 1;
	}

	MPI_Sendrecv_replace(A, DIMENSION, MPI_DOUBLE, j, TAG, i, TAG, MPI_COMM_WORLD, &status);

	printf("After the shift on processor %d B=", myrank);
	for(i=0; i<DIMENSION; i++) {
		printf("%.2f    ", A[i]);
	}
	printf("\n");

	/* MPI geordnet beenden */
	MPI_Finalize();

	return 0;
}
/* Ergebnis (p=3)
Before the shift on processor 0 A=0.63   0.37   0.72   
After the shift on processor 0 B=2.63    2.37    2.72    
Before the shift on processor 2 A=2.63   2.37   2.72   
Before the shift on processor 1 A=1.63   1.37   1.72   
After the shift on processor 1 B=0.63    0.37    0.72    
After the shift on processor 2 B=1.63    1.37    1.72

 * Ergebnis (p=4)
Before the shift on processor 2 A=2.61   2.15   2.57   
After the shift on processor 2 B=1.61    1.15    1.57    
Before the shift on processor 0 A=0.61   0.15   0.57   
After the shift on processor 0 B=3.61    3.15    3.57    
Before the shift on processor 1 A=1.61   1.15   1.57   
Before the shift on processor 3 A=3.61   3.15   3.57   
After the shift on processor 3 B=2.61    2.15    2.57    
After the shift on processor 1 B=0.61    0.15    0.57

 */
