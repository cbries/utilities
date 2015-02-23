/*
 * License: MIT 
 * Author: Christian Benjamin Ries
 * Source: https://github.com/cbries/utilities
 */

/* 
** DESCRIPTION: Rahmen für MPI-Programme
** COMPILE:     mpicc -o hw11b hw11b.c
** RUN:         mpirun -np 4 -machinefile machines hw11b datalen
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
	char *data = NULL;
	char *localData = NULL;

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
	
	if(datalen%nprocs > 0) {
		if(myrank==0) {
			printf("Count of processes should divide datalen.\n");
		}
		MPI_Finalize();
		return 1;
	}

	m = datalen/nprocs;

	data = (char*) malloc(datalen*sizeof(char));
	localData = (char*) malloc(m*sizeof(char));
	for(i=0; i<m; i++) {
		localData[i] = rand()%('Z'-'A')+'A';
	}

	MPI_Allgather(localData, m, MPI_BYTE, data, m, MPI_BYTE, MPI_COMM_WORLD);

	if(myrank==0) {
		printf("data:");
		for(i=0; i<datalen; i++) {
			printf("%c", data[i]);
		}
		printf("\n");
	}

	/* MPI geordnet beenden */
	MPI_Finalize();

	return 0;
}
/* Result:
 */
