/*
 * License: MIT 
 * Author: Christian Benjamin Ries
 * Source: https://github.com/cbries/utilities
 */

/* 
** DESCRIPTION: Rahmen für MPI-Programme
** COMPILE:     mpicc -o hw3 hw3.c
** RUN:         mpirun -np 4 -machinefile machines hw3 arg1 arg2 ...
*/
/*
! Homework description:
!      Scatter real array A from processor 0 to all other processors
!      using only mpi_send and mpi_recv as follows.  Let p
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

	if(myrank == 0) {
		A = (double*) malloc( DIMENSION * nprocs * sizeof(double));
		for(i=0; i<DIMENSION*nprocs; i++) {
			A[i] = rand()/1000.0f;
		}

		printf("A=\n");
		for(i=0; i<DIMENSION*nprocs; i++) {
			printf("%.2lf    ", A[i]);
		}
		printf("\n");

		/* Ohne Prüfung ob Speicher alloziert werden konnte(!) */
		/* Programm stuerzt hoffentlich ab... :D */

		for(i=1; i<nprocs; i++) {
			MPI_Send(A+i*DIMENSION, DIMENSION, MPI_DOUBLE, i, TAG, MPI_COMM_WORLD);
		}

		free(A); A = NULL;		
	} else {
		MPI_Recv(B, DIMENSION, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD, &status);
		for(i=0; i<DIMENSION; i++) {
			printf("B=%.2f on processor of rank=%d\n", B[i], myrank);
		}
	}

	/* MPI geordnet beenden */
	MPI_Finalize();

	return 0;
}
/* Ergebnis:
A=
1804289.38    846930.89    1681692.78    
1714636.92    1957747.79    424238.34    
719885.39    1649760.49    596516.65    
1189641.42    1025202.36    1350490.03    
B=1714636.92 on processor of rank=1
B=1957747.79 on processor of rank=1
B=424238.34 on processor of rank=1
B=1189641.42 on processor of rank=3
B=1025202.36 on processor of rank=3
B=1350490.03 on processor of rank=3
B=719885.39 on processor of rank=2
B=1649760.49 on processor of rank=2
B=596516.65 on processor of rank=2
 */
