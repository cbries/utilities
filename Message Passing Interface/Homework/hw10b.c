/*
 * License: MIT 
 * Author: Christian Benjamin Ries
 * Source: https://github.com/cbries/utilities
 */

/* 
** DESCRIPTION: Rahmen für MPI-Programme
** COMPILE:     mpicc -o hw10b hw10b.c
** RUN:         mpirun -np 4 -machinefile machines hw10b datalen
*/

/*
 * Homework description:
MPI_Broadcast ersetzen durch MPI_Send von root und einer Schleife über
alle anderen Prozesse mit MPI_Recv. Vergleich beider Versionen mit
unterschiedlichen Nachrichtengrößen und unterschiedlicher Anzahl von
Prozessoren via Zeitmessung!
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
	double messungTime = 0.0;
	double messungTimeSumme = 0.0;
	int datalen = 0;
	char *data = NULL;

	MPI_Status status;
	
	/* MPI initialisieren und Argumente setzen */ 
	MPI_Init(&argc, &argv);

	/* Anzahl der Prozesse erfragen */
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

	/* Eigenen Rang erfragen */
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	/* Rechnernamen erfragen */
	MPI_Get_processor_name(name, &namelen);

	if(argc<2) {
		if(myrank==0) {
			printf("Usage: hw10b bytes\n");	
		}
		MPI_Finalize();
		return 1;
	}

	srand(time(NULL));

	// Pruefung auf Datentyp fehlt(!)
	datalen = atoi(argv[1]);
	data = (char*) malloc(datalen*sizeof(char));
	if(myrank==0) {
		for(i=0; i<datalen; i++) {
			data[i] = (char) rand()%'A'+'Z';
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);
	messungTime = MPI_Wtime();

	if(myrank==0) {
		for(i=1; i<nprocs; i++) {
			MPI_Send(data, datalen, MPI_BYTE, i, TAG, MPI_COMM_WORLD);
		}
	} else {
		MPI_Recv(data, datalen, MPI_BYTE, 0, TAG, MPI_COMM_WORLD, &status);
	}

	messungTime = MPI_Wtime() - messungTime;
	MPI_Barrier(MPI_COMM_WORLD);

	MPI_Reduce(&messungTime, &messungTimeSumme, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if(myrank==0) {
		printf("Dauer mit MPI_Send(...):  %.10lfsec\n", messungTimeSumme);
	}

	/* MPI geordnet beenden */
	MPI_Finalize();

	return 0;
}
/* Result:
 */
