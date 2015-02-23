/*
 * License: MIT 
 * Author: Christian Benjamin Ries
 * Source: https://github.com/cbries/utilities
 */

/* 
** DESCRIPTION: Rahmen für MPI-Programme
** COMPILE:     mpicc -o hw4 hw4.c
** RUN:         mpirun -np 4 -machinefile machines hw4 arg1 arg2 ...
*/
/*
! Homework description:
!      Assume there are p processors and each processor has a
!      real array B(1:n).  Gather the B arrays into a single
!      real array A on processor 0 so that A(1:n,j) = B(1:n) on
!      processor j.  Your program should be correct for all
!      values of n > 0 and p > 1.  WRITE YOUR PROGRAM FOR
!      EFFICIENT EXECUTION. Run your program with p = 4
!      and present the results at the end of the program.
!      Use only mpi_send and mpi_recv.
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

	for(i=0; i<DIMENSION; i++) {
		B[i] = (rand()%100) / 100.0f + myrank;
	}

	for(i=0; i<DIMENSION; i++) {
		printf("On processor of rank %d B=%.2f\n", myrank, B[i]);
		fflush(stdout);
	}
	
	if(myrank==0) {
		A = (double*) malloc(DIMENSION * nprocs * sizeof(double));
		if(A==NULL) {
			perror("malloc for A failed");
		}

		for(i=0; i<DIMENSION; i++) {
			A[i] = B[i];
		}
		
		for(i=1; i<nprocs; i++) {
			MPI_Recv(A+DIMENSION+((i-1)*DIMENSION), DIMENSION, MPI_DOUBLE, i, TAG, MPI_COMM_WORLD, &status);
		}
		
		for(i=0; i<DIMENSION; i++) {
			printf("column(%d): ", i);
			for(j=0; j<nprocs; j++) {
				printf("%.2f   ", A[j*DIMENSION+i]);
			}
			printf("\n");
		}
		
		free(A); A = NULL;
	} else {
		MPI_Send(B, DIMENSION, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD);
	}

	/* MPI geordnet beenden */
	MPI_Finalize();

	return 0;
}
/* Ergebnis:
On processor of rank 0 B=0.83
On processor of rank 0 B=0.86
On processor of rank 0 B=0.77
On processor of rank 0 B=0.15
On processor of rank 2 B=2.83
On processor of rank 2 B=2.86
On processor of rank 2 B=2.77
On processor of rank 2 B=2.15
On processor of rank 1 B=1.83
On processor of rank 1 B=1.86
On processor of rank 1 B=1.77
On processor of rank 1 B=1.15
On processor of rank 3 B=3.83
On processor of rank 3 B=3.86
On processor of rank 3 B=3.77
On processor of rank 3 B=3.15
column(0): 0.83   1.83   2.83   3.83   
column(1): 0.86   1.86   2.86   3.86   
column(2): 0.77   1.77   2.77   3.77   
column(3): 0.15   1.15   2.15   3.15
 */
