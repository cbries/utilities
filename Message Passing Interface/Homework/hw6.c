/*
 * License: MIT 
 * Author: Christian Benjamin Ries
 * Source: https://github.com/cbries/utilities
 */

/* 
** DESCRIPTION: Rahmen für MPI-Programme
** COMPILE:     mpicc -o hw6 hw6.c
** RUN:         mpirun -np 4 -machinefile machines hw6 arg1 arg2 ...
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

	if(myrank==0) {
		MPI_Sendrecv(A, DIMENSION, MPI_DOUBLE, myrank+1, TAG,
								 B, DIMENSION, MPI_DOUBLE, nprocs-1, TAG,
								 MPI_COMM_WORLD, &status);
	} else if(myrank==nprocs-1) {
		MPI_Sendrecv(A, DIMENSION, MPI_DOUBLE, 0, TAG,
								 B, DIMENSION, MPI_DOUBLE, myrank-1, TAG,
								 MPI_COMM_WORLD, &status);
	} else {
		MPI_Sendrecv(A, DIMENSION, MPI_DOUBLE, myrank+1, TAG,
								 B, DIMENSION, MPI_DOUBLE, myrank-1, TAG,
								 MPI_COMM_WORLD, &status);
	} 

	printf("After the shift on processor %d B=", myrank);
	for(i=0; i<DIMENSION; i++) {
		printf("%.2f    ", B[i]);
	}
	printf("\n");

	/* MPI geordnet beenden */
	MPI_Finalize();

	return 0;
}
/* Ergebnis (p=3)
Before the shift on processor 1 A=1.57   1.38   1.04   
After the shift on processor 1 B=0.57    0.38    0.04    
Before the shift on processor 2 A=2.57   2.38   2.04   
After the shift on processor 2 B=1.57    1.38    1.04    
Before the shift on processor 0 A=0.57   0.38   0.04   
After the shift on processor 0 B=2.57    2.38    2.04
* 
* Ergebnis (p=4)
Before the shift on processor 0 A=0.21   0.75   0.41   
After the shift on processor 0 B=3.21    3.75    3.41    
Before the shift on processor 3 A=3.21   3.75   3.41   
Before the shift on processor 2 A=2.21   2.75   2.41   
Before the shift on processor 1 A=1.21   1.75   1.41   
After the shift on processor 1 B=0.21    0.75    0.41    
After the shift on processor 2 B=1.21    1.75    1.41    
After the shift on processor 3 B=2.21    2.75    2.41
 */
