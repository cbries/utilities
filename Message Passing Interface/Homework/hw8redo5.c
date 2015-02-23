/*
 * License: MIT 
 * Author: Christian Benjamin Ries
 * Source: https://github.com/cbries/utilities
 */

/* 
** DESCRIPTION: Rahmen für MPI-Programme
** COMPILE:     mpicc -o hw8redo5 hw8redo5.c
** RUN:         mpirun -np 4 -machinefile machines hw8redo5 arg1 arg2 ...
*/
/*
! Homework description:
!      Assume each processor has a real array A(1:n).  On
!      processor 0 form B(i) = SUM A(i), where the SUM is
!      taken over all p processors.  Write your program
!      for efficient execution using mpi_reduce.
!      Your program should work correctly for all n and p > 1.
!      Turn in results for n = 3 and p = 4.
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <mpi.h>

#define DIMENSION 4
#define TAG 123

int main (int argc, char **argv)
{
	int i, j;
	int myrank, nprocs;
	int namelen;
	char name[MPI_MAX_PROCESSOR_NAME];

	double *A = NULL;
	double *B = NULL;
	
	MPI_Status status;
	
	/* MPI initialisieren und Argumente setzen */ 
	MPI_Init(&argc, &argv);

	/* Anzahl der Prozesse erfragen */
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

	/* Eigenen Rang erfragen */
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	/* Rechnernamen erfragen */
	MPI_Get_processor_name(name, &namelen);

	A = (double*) malloc(DIMENSION * sizeof(double));
	if(A == NULL) {
		perror("malloc for A failed");
	}
	
	srand(time(NULL));
	for(i=0; i<DIMENSION; i++) {
		A[i] = rand()%100/100.0f + myrank;
	}

	printf("On processor of rank %d A=", myrank);
	for(i=0; i<DIMENSION; i++) {
		printf("%.2f   ", A[i]);
	}
	printf("\n");

	if(myrank==0) {
		B = (double*) malloc(DIMENSION*sizeof(double));
		if(B==NULL) {
			printf("malloc for B failed");
		}
	}

	// receive A from all other processors and put their sum into B

	MPI_Reduce(A, B, DIMENSION, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if(myrank==0) {
		printf("B=");
		for(i=0; i<DIMENSION; i++) {
			printf("%.2f   ", B[i]);
		}
		printf("\n");		
	}

	free(A); A = NULL;
	free(B); B = NULL;

	/* MPI geordnet beenden */
	MPI_Finalize();

	return 0;
}
/* Ergebnis (p=3)
On processor of rank 0 A=0.18   0.85   0.48   0.89   
On processor of rank 2 A=2.18   2.85   2.48   2.89   
On processor of rank 1 A=1.18   1.85   1.48   1.89   
B=3.54   5.55   4.44   5.67 
 *
 * Ergebnis (p=4)
On processor of rank 0 A=0.00   0.36   0.69   0.21   
On processor of rank 3 A=3.00   3.36   3.69   3.21   
On processor of rank 1 A=1.00   1.36   1.69   1.21   
On processor of rank 2 A=2.00   2.36   2.69   2.21   
B=6.00   7.44   8.76   6.84
 */ 
