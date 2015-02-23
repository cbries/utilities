/*
 * License: MIT 
 * Author: Christian Benjamin Ries
 * Source: https://github.com/cbries/utilities
 */

/* 
** DESCRIPTION: Rahmen für MPI-Programme
** COMPILE:     mpicc -o hw1 hw1.c
** RUN:         mpirun -np 4 -machinefile machines hw1 arg1 arg2 ...
*/

/*
 * Homework description:
 * Initialize real array A on processor of rank 0,
 * A(i) = float(i) for i = 1,n where n = 4.
 * Send A to processor of rank 1 and add 1 to 
 * each element receiving A in array B.
 * Then send B from processor of rank 1 to 
 * processor of rank 2 into array C.
 * Add 1 to each element of C and then send C
 * to processor of rank 0 receiving C into
 * array D. Print the values of D on the 
 * processor of rank 0.
 * Use only mpi_send and mpi_recv and use p = 4.
 **/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
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

	printf("nprocs=%d\n", nprocs);
	printf("my rank=%d\n", myrank);

	if(myrank == 0) {

		for(i=0; i<DIMENSION; i++) {
			A[i] = (double) i + 1;
		}

		MPI_Send(A, DIMENSION, MPI_DOUBLE, 1, TAG, MPI_COMM_WORLD);
		MPI_Recv(D, DIMENSION, MPI_DOUBLE, 2, TAG, MPI_COMM_WORLD, &status);
		for(i=0; i<DIMENSION; i++) {
			printf("D[%d]=%lf on processor of rank %d\n", i, D[i], myrank);
			fflush(stdout);
		}
	} else if(myrank == 1) {
		MPI_Recv(B, DIMENSION, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD, &status);
		for(i=0; i<DIMENSION; i++) {
			B[i] = B[i] + 1.0;
		}
		MPI_Send(B, DIMENSION, MPI_DOUBLE, 2, TAG, MPI_COMM_WORLD);
	} else if(myrank == 2) {
		MPI_Recv(C, DIMENSION, MPI_DOUBLE, 1, TAG, MPI_COMM_WORLD, &status);
		for(i=0; i<DIMENSION; i++) {
			C[i] = C[i] + 1.0;
		}
		MPI_Send(C, DIMENSION, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD);
	} else {
		printf("rank=%d - Nothing to do...\n", myrank);
		fflush(stdout);
	}

	/* MPI geordnet beenden */
	MPI_Finalize();

	return 0;
}
/* Result:
 * rank=3 - Nothing to do...
 * D[0]=3.000000 on processor of rank 0
 * D[1]=4.000000 on processor of rank 0
 * D[2]=5.000000 on processor of rank 0
 * D[3]=6.000000 on processor of rank 0
 */
