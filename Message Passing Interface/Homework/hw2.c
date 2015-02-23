/*
 * License: MIT 
 * Author: Christian Benjamin Ries
 * Source: https://github.com/cbries/utilities
 */

/* 
** DESCRIPTION: Rahmen für MPI-Programme
** COMPILE:     mpicc -o hw2 hw2.c
** RUN:         mpirun -np [3,4] -machinefile machines hw2 arg1 arg2 ...
*/

/*
 * Homework description:
 * Broadcast A from processor 0 to all other processors
 * using only mpi_send and mpi_recv. Print the value of A 
 * on each processor along with the rank of the processor
 * Write the program so it executes correctly for all p > 1
 * but only include the result for p=3 and p=4.
 */
#include <stdio.h>
#include <mpi.h>

#define DIMENSION 4
#define TAG 123

int main (int argc, char **argv)
{
	int i;
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
			A[i] = (double) i+1;
		}
		for(i=1; i<nprocs; i++) {
			MPI_Send(A, DIMENSION, MPI_DOUBLE, i, TAG, MPI_COMM_WORLD);
		}
	} else {
		MPI_Recv(B, DIMENSION, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD, &status);
		for(i=0; i<DIMENSION; i++) {
			printf("On processor %d, B=%lf\n", myrank, B[i]);
		}
	}

	/* MPI geordnet beenden */
	MPI_Finalize();

	return 0;
}
/*
 * Ergebnis (p=3):
 * On processor 2, B=1.000000
 * On processor 2, B=2.000000
 * On processor 2, B=3.000000
 * On processor 2, B=4.000000
 * On processor 1, B=1.000000
 * On processor 1, B=2.000000
 * On processor 1, B=3.000000
 * On processor 1, B=4.000000
 */

/* Ergebnis (p=4):
 * On processor 1, B=1.000000
 * On processor 1, B=2.000000
 * On processor 1, B=3.000000
 * On processor 1, B=4.000000
 * On processor 2, B=1.000000
 * On processor 2, B=2.000000
 * On processor 2, B=3.000000
 * On processor 2, B=4.000000
 * On processor 3, B=1.000000
 * On processor 3, B=2.000000
 * On processor 3, B=3.000000
 * On processor 3, B=4.000000
 */

