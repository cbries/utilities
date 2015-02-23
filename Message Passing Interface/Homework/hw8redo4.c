/*
 * License: MIT 
 * Author: Christian Benjamin Ries
 * Source: https://github.com/cbries/utilities
 */

/* 
** DESCRIPTION: Rahmen für MPI-Programme
** COMPILE:     mpicc -o hw8redo4 hw8redo4.c
** RUN:         mpirun -np 4 -machinefile machines hw8redo4 arg1 arg2 ...
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

	srand(time(NULL));
	for(i=0; i<DIMENSION; i++) {
		B[i] = rand()%100/100.0f + myrank;
	}

	printf("On processor of rank %d B=", myrank);
	for(i=0; i<DIMENSION; i++) {
		printf("%.2f   ", B[i]);
	}
	printf("\n");

	if(myrank==0) {
		A = (double*) malloc(DIMENSION*nprocs*sizeof(double));
		if(A==NULL) {
			perror("malloc for A failed");
		}		
	}

	MPI_Gather(B, DIMENSION, MPI_DOUBLE, A, DIMENSION, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	if(myrank==0) {
		for(i=0; i<nprocs; i++) {
			printf("Column %d of A=", i);
			for(j=0; j<DIMENSION; j++) {
				printf("%.2f   ", A[i*DIMENSION+j]);
			}
			printf("\n");
		}
		
		if(A!=NULL) {
			free(A); A = NULL;
		}
	}

	/* MPI geordnet beenden */
	MPI_Finalize();

	return 0;
}
/* Ergebnis (p=3)
On processor of rank 0 B=0.83   0.83   0.33   0.84   
On processor of rank 2 B=2.83   2.83   2.33   2.84   
On processor of rank 1 B=1.83   1.83   1.33   1.84   
Column 0 of A=0.83   0.83   0.33   0.84   
Column 1 of A=1.83   1.83   1.33   1.84   
Column 2 of A=2.83   2.83   2.33   2.84
 *
 * Ergebnis (p=4)
On processor of rank 0 B=0.57   0.23   0.52   0.33   
On processor of rank 1 B=1.57   1.23   1.52   1.33   
On processor of rank 3 B=3.57   3.23   3.52   3.33   
On processor of rank 2 B=2.57   2.23   2.52   2.33   
Column 0 of A=0.57   0.23   0.52   0.33   
Column 1 of A=1.57   1.23   1.52   1.33   
Column 2 of A=2.57   2.23   2.52   2.33   
Column 3 of A=3.57   3.23   3.52   3.33
 */ 
