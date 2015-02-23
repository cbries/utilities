/*
 * License: MIT 
 * Author: Christian Benjamin Ries
 * Source: https://github.com/cbries/utilities
 */

/* 
** DESCRIPTION: Rahmen für MPI-Programme
** COMPILE:     mpicc -o hw8redo hw8redo.c
** RUN:         mpirun -np 4 -machinefile machines hw8redo2 arg1 arg2 ...
*/
/*
! Homework description:
! Broadcast A from processor 0 to all other processors
! using mpi collective routines. Print the value of A 
! on each processor along with the rank of the processor
! Write the program so it executes correctly for all p > 1
! but only include the result for p=3 and p=4.
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

	double A[DIMENSION] = {0};
	double B[DIMENSION] = {0};
	double C[DIMENSION] = {0};
	double D[DIMENSION] = {0};
	
	MPI_Status status;
	
	/* MPI initialisieren und Argumente setzen */ 
	MPI_Init(&argc, &argv);

	/* Anzahl der Prozesse erfragen */
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

	/* Eigenen Rang erfragen */
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	/* Rechnernamen erfragen */
	MPI_Get_processor_name(name, &namelen);

	if(myrank==0) {
		for(i=0; i<DIMENSION; i++) {
			A[i] = i;
		}
	}

	MPI_Bcast(A, DIMENSION, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	printf("On processor %d A=", myrank);
	for(i=0; i<DIMENSION; i++) {
		printf("%.2f    ", A[i]);
	}
	printf("\n");

	/* MPI geordnet beenden */
	MPI_Finalize();

	return 0;
}
/* Ergebnis (p=3)
On processor 0 A=0.00    1.00    2.00    3.00    
On processor 1 A=0.00    1.00    2.00    3.00    
On processor 2 A=0.00    1.00    2.00    3.00
 *
 * Ergebnis (p=4)
On processor 1 A=0.00    1.00    2.00    3.00    
On processor 0 A=0.00    1.00    2.00    3.00    
On processor 2 A=0.00    1.00    2.00    3.00    
On processor 3 A=0.00    1.00    2.00    3.00
 */ 
