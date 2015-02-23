/*
 * License: MIT 
 * Author: Christian Benjamin Ries
 * Source: https://github.com/cbries/utilities
 */

/* 
** DESCRIPTION: Rahmen für MPI-Programme
** COMPILE:     mpicc -o hw8redo3 hw8redo3.c
** RUN:         mpirun -np 4 -machinefile machines hw8redo3 arg1 arg2 ...
*/
/*
! Homework description:
!      Scatter real array A from processor 0 to all other processors
!      using collective routines.  Let p
!      be the number of available processors and declare A as
!      real*8,dimension(:,:),allocatable ::  A
!      and then after determining the value of p from calling
!      mpi_comm_size, only on processor 0 allocate A as
!         allocate (A(n,0:p-1))
!      and give A values by using the F90 random number
!      generator by
!         call random_number(A)
!      print the values of A, send the jth column of
!      A to processor j, j = 1, ..., p-1, receive
!      into array B and print the values of B to
!      check your program's accuracy.  Test your
!      program with 4 processors.  Your program should work
!      correctly for all p > 1!
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

	A = (double*) malloc(DIMENSION * nprocs * sizeof(double));
	if(A == NULL) {
		perror("malloc failed for A");
	}
	
	if(myrank==0) {
		srand(time(NULL));
		for(i=0; i<DIMENSION*nprocs; i++) {
			A[i] = (rand()%100) / 100.0f + myrank;
		}
		
		printf("On processor %d A=", myrank);
		for(i=0; i<DIMENSION*nprocs; i++) {
			printf("%.2f    ", A[i]);
		}
		printf("\n");
	}
	
	MPI_Scatter(A, DIMENSION, MPI_DOUBLE, B, DIMENSION, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	
	printf("On processor %d B=", myrank);
	for(i=0; i<DIMENSION; i++) {
		printf("%.2f    ", B[i]);
	}
	printf("\n");
	
	free(A); A = NULL;

	/* MPI geordnet beenden */
	MPI_Finalize();

	return 0;
}
/* Ergebnis (p=3)
On processor 0 A=0.84    0.67    0.59    0.27    0.41    0.18    0.22    0.15    0.02    
On processor 0 B=0.84    0.67    0.59    
On processor 1 B=0.27    0.41    0.18    
On processor 2 B=0.22    0.15    0.02
 *
 * Ergebnis (p=4)
On processor 0 A=0.77    0.32    0.76    0.79    0.32    0.67    0.94    0.11    0.76    0.93    0.87    0.05    
On processor 0 B=0.77    0.32    0.76    
On processor 2 B=0.94    0.11    0.76    
On processor 1 B=0.79    0.32    0.67    
On processor 3 B=0.93    0.87    0.05
 */ 
