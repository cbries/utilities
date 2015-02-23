/*
 * License: MIT 
 * Author: Christian Benjamin Ries
 * Source: https://github.com/cbries/utilities
 */

/* 
** DESCRIPTION: Rahmen für MPI-Programme
** COMPILE:     mpicc -o hw5 hw5.c
** RUN:         mpirun -np 4 -machinefile machines hw5 arg1 arg2 ...
*/
/*
! Homework description:
!      Assume each processor has a real array A(1:n).  On
!      processor 0 form B(i) = SUM A(i), where the SUM is
!      taken over all p processors.  Write your program
!      for efficient execution using mpi_send and mpi_recv.
!      Your program should work correctly for all n and p > 1.
!      Turn in results for n = 3 and p = 4.
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
	double *B = NULL;
	double *C = NULL;
	
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
	
	for(i=0; i<DIMENSION; i++) {
		printf("On processor of rank %d A=%.2f\n", myrank, A[i]);
		fflush(stdout);
	}

	if(myrank==0) {
		B = (double*) malloc(DIMENSION * sizeof(double));
		if(B == NULL) {
			perror("malloc for B failed");
		}

		C = (double*) malloc(DIMENSION * sizeof(double));
		if(C == NULL) {
			perror("malloc for C failed");
		}

		for(i=0; i<DIMENSION; i++) {
			B[i] = 0.0;
			C[i] = 0.0;
		}
	
		for(i=0; i<DIMENSION; i++) {
			B[i] += A[i];
		}

		for(i=1; i<nprocs; i++) {
			MPI_Recv(C, DIMENSION, MPI_DOUBLE, i, TAG, MPI_COMM_WORLD, &status);

			for(j=0; j<DIMENSION; j++) {
				B[j] += C[j];
			}

		}

		printf("B=");
		for(i=0; i<DIMENSION; i++) {
			printf("%.2f   ", B[i]);
		}
		printf("\n");

		free(B); B = NULL;
		free(C); C = NULL;
	} else {
		MPI_Send(A, DIMENSION, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD);
	}

	/* MPI geordnet beenden */
	MPI_Finalize();

	return 0;
}
/* Ergebnis (p=3)
On processor of rank 0 A=0.49
On processor of rank 0 A=0.62
On processor of rank 0 A=0.14
B=4.47   4.86   3.42   
On processor of rank 1 A=1.49
On processor of rank 1 A=1.62
On processor of rank 1 A=1.14
On processor of rank 2 A=2.49
On processor of rank 2 A=2.62
On processor of rank 2 A=2.14
* 
* Ergebnis (p=4)
On processor of rank 0 A=0.47
On processor of rank 0 A=0.05
On processor of rank 0 A=0.16
On processor of rank 3 A=3.47
On processor of rank 3 A=3.05
On processor of rank 3 A=3.16
On processor of rank 1 A=1.47
On processor of rank 1 A=1.05
On processor of rank 1 A=1.16
B=7.88   6.20   6.64   
On processor of rank 2 A=2.47
On processor of rank 2 A=2.05
On processor of rank 2 A=2.16
 */
