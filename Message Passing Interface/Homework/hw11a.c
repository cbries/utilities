/*
 * License: MIT 
 * Author: Christian Benjamin Ries
 * Source: https://github.com/cbries/utilities
 */

/* 
** DESCRIPTION: Rahmen für MPI-Programme
** COMPILE:     mpicc -o hw11a hw11a.c
** RUN:         mpirun -np 4 -machinefile machines hw11a datalen
 */

/*
 * Homework description:
MPI_Allgather ersetzen durch MPI_Gather und Broadcast. Vergleich beider
Versionen mit unterschiedlichen Nachrichtengrößen und unterschiedlicher
Anzahl von Prozessoren via Zeitmessung!
 **/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <mpi.h>

#define DIMENSION 4
#define TAG 123

int main (int argc, char **argv)
{
	int i, m;
	int myrank, nprocs;
	int namelen;
	char name[MPI_MAX_PROCESSOR_NAME];
	double messungTime = 0.0;
	double messungTimeSumme = 0.0;
	int datalen = 0;
	double *data = NULL;
	double *dataLocal = NULL;

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

	// Pruefung auf Datentyp fehlt(!)
  datalen = atoi(argv[1]);

	m = datalen / nprocs;

	if(datalen % nprocs > 0) {
		if(myrank==0) {
			printf("datalen should be divided by processors.\n");
		}
		MPI_Finalize();
		return 1;
	}

	srand(time(NULL));

	data = (double*) malloc(datalen*sizeof(double));
	if(myrank==0) {
		for(i=0; i<datalen; i++) {
			data[i] = 0.0;
		}
	}

	dataLocal = (double*) malloc(m*sizeof(double));
	for(i=0; i<m; i++) {
		dataLocal[i] = (double) i + (myrank/100.0);
	}

	MPI_Barrier(MPI_COMM_WORLD);
	messungTime = MPI_Wtime();

	MPI_Gather(dataLocal, m, MPI_DOUBLE, 
						 data, m, MPI_DOUBLE, 
						 0, MPI_COMM_WORLD);

	MPI_Bcast(data, datalen, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	messungTime = MPI_Wtime() - messungTime;
	MPI_Barrier(MPI_COMM_WORLD);

	MPI_Reduce(&messungTime, &messungTimeSumme, 1, MPI_DOUBLE, MPI_SUM,
						 0, MPI_COMM_WORLD);

	if(myrank==0) {
		printf("The operation took %.10lf seconds.\n", messungTimeSumme);
	}

	/* MPI geordnet beenden */
	MPI_Finalize();

	return 0;
}
/* Result:
 */
