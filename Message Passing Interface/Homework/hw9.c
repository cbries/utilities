/*
 * License: MIT 
 * Author: Christian Benjamin Ries
 * Source: https://github.com/cbries/utilities
 */

/*
 * @date 2010_04_22
 * @description Rahmen für MPI-Programme
 *
 * COMPILE:     mpicc -o hw9 hw9.c
 * RUN:         mpirun -np 4 -machinefile machines hw9 arg1 arg2 ...
*/
/*
!     DESCRIPTION OF THIS PROBLEM:
!     Calculate y = y + Ax where x & y are 1-dimensional arrays of
!     length n and A is an 2-dimensional array of dimension (n,n).
!     Let p = the number of processors used for the execution of this
!     program.  THIS PROGRAM ASSUMES p DIVIDES n.
!     All floating point calculations are in real*8.
!     The calculation proceeds as follows:

!        *  A, x, & y are initialized by the master node,
!           i.e. the processor of rank 0

!        *  master node calculates z = y + Ax to check answers

!        *  y and A are partitioned into p row blocks so the block
!           size is m = n/p

!        *  start the timer and broadcast x.

!        *  the y and A row blocks are sent to the processors of rank
!           1, 2, ..., p-1.  Row blocks of y & A are placed in the temporary
!           array "temp" for sending. Use mpi_send for sending row blocks.

!        *  the above row blocks are received by these processors that then
!           calculate their portion of y = y + Ax.  Use mpi_recv & sgemv.

!        *  the Host Program receives each y(i) using mpi_recv, calls the timer,
!           calculates z = z - y & Mflop/s, print maxval(abs(z)) and Mflop/s.

!    Take p = 4 and n = 500 to test your program.  Debug with n = 4 or 8.
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <mpi.h>

#define MESSUNGEN 10
#define DIMENSION 8
#define TAG 123

#define TMPSIZE (m * (DIMENSION+1))
#define RANKPOS (myrank*m*(DIMENSION+1))

/*
 * Multipliziert eine Matrix mit einem Vektor. Spalten und Zeilen
 * muessen uebereinstimmen. Das Ergebnis wird in res abgespeichert.
 * @param mat
 * @param vec
 * @param veclen Dimension der Matrix und des Vektors
 * @param res Vector
 */
void matMulVec( double *mat, double *vec, int veclen, double *res ) {
	int i, j;
	for(j=0; j<veclen; j++ ) {
		res[j] = 0.0;
		for(i=0; i<veclen; i++) {
			res[j] += mat[i+j*veclen] * vec[i];
		}
	}
}

/*
 * Addiert zwei Vektoren miteinander.
 * @param vec1
 * @param vec2
 * @param veclen
 * @param vecres
 */
void vecAdd( double *vec1, double *vec2, int veclen, double *vecres ) {
	int i=0;
	for(i=0; i<veclen; i++) {
		vecres[i] = vec1[i] + vec2[i];
	}
}

/*
 * Zeigt die Elemente eines Vektors an.
 * @param v Vektor
 * @param len Laenge des Vektors
 * @param name Ein Name der vor der Ausgabe hinzugefuegt wird
 */
void display(double *v, int len, char *name) {
	int i;
	for(i=0; i<len; i++) {
		printf("%s[%d]=%.12lf\n", name, i, v[i]);
	}
}

/*
 * @param argc
 * @param argv
 */
int main (int argc, char **argv) {
	int i, j, m, nprocs, myrank, i_row, i_column, index, i_temp;
	int namelen;
	char name[MPI_MAX_PROCESSOR_NAME];
	
	double A[DIMENSION*DIMENSION] = {0.0};
	double X[DIMENSION] = {0.0};
	double Y[DIMENSION] = {0.0};
	double Z[DIMENSION] = {0.0};
	double ZShared[DIMENSION] = {-1.0};
	double TMP[DIMENSION*DIMENSION] = {0.0};
	double *temp = NULL;
	double *results = NULL;

	int messung;
	double wtime[MESSUNGEN] = {0.0};
	double wtick[MESSUNGEN] = {0.0};
	double messungResultsWTIME[MESSUNGEN] = {0.0};
	double messungResultsWTICK[MESSUNGEN] = {0.0};

	MPI_Status status;
	
	/* MPI initialisieren und Argumente setzen */ 
	MPI_Init(&argc, &argv);

	/* Anzahl der Prozesse erfragen */
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	
	/* Eigenen Rang erfragen */
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	/* Rechnernamen erfragen */
	MPI_Get_processor_name(name, &namelen);

/*
! A, x, & y are initialized by the master node,
! i.e. the processor of rank 0
 */
	if(myrank == 0) {
		for(i=0; i<DIMENSION; i++) {
			X[i] = (double) i + 1.0;
			Y[i] = (double) i + 1.0;
		}
		for(i=0; i<DIMENSION*DIMENSION; i++) {
			A[i] = (double) i + 1.0;
		}
/*
! master node calculates z = y + Ax to check answers
 */
		matMulVec(A, X, DIMENSION, TMP);
		vecAdd(Y, TMP, DIMENSION, Z);
		
		display(Z, DIMENSION, "Z");	
	}

 // Messung der Durchlaufzeit und Prozesstakte
 for(messung=0; messung<MESSUNGEN; messung++) {

	m = DIMENSION / nprocs;

	if( DIMENSION%nprocs != 0) {
		printf("Die Dimension sollte durch die Prozessoranzahl teilbar sein.\n");
	} else {
		if(myrank==0) {
			printf("nprocs=%d, m=%d\n", nprocs, m);
		}
	}

	results = (double*) malloc( m * sizeof(double));
	temp = (double*) malloc( TMPSIZE * sizeof(double));
	/* results und temp auf NULL pruefen(!) */

	MPI_Barrier(MPI_COMM_WORLD);
	wtime[messung] = MPI_Wtime();
	wtick[messung] = MPI_Wtick();

	MPI_Bcast(X, DIMENSION, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	
	if(myrank == 0) {		

		/*
		 * Nachfolgende Schleife baut das Feld 'temp' zum Verschicken auf.
		 * Der Aufbau des Vektors (2x2 Dimension) ist:
		 * temp = { 
		 *   Y1, A11, A12,     --> 3 Elemente, 1. Element ist immer Y.i_row
		 *   Y2, A21, A22
		 * }
		 */
		for(j=1; j<nprocs; j++) {
			i_temp = 0;

			for(i_row=j*m; i_row<(j+1)*m; i_row++) {

				index = i_row*DIMENSION;

				temp[i_temp] = Y[i_row];
			
				i_temp++;
			
				// Werte der Matrix A in temp speichern
				for(i=0; i<DIMENSION; i++) {

					index = i_row*DIMENSION+i;

					temp[i_temp] = A[index];
					
					i_temp++;
				}
			}
			MPI_Send(temp, TMPSIZE, MPI_DOUBLE, j, TAG, MPI_COMM_WORLD);
		}

		for(i_row=0; i_row<m; i_row++) {
			ZShared[i_row] = Y[i_row];
			for(i=0; i<DIMENSION; i++) {
				ZShared[i_row] += (A[i_row*DIMENSION+i] * X[i]);
			}
		}

		for(i=1; i<nprocs; i++) {
			MPI_Recv(ZShared+(i*m), m, MPI_DOUBLE, i, TAG, MPI_COMM_WORLD, &status);
		}

		/* Ausgabe */
		//display(ZShared, DIMENSION, "ZShared");
	}
	
	if(myrank > 0) {
		MPI_Recv(temp, TMPSIZE, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD, &status);

		for(i_row=0; i_row<m; i_row++) {

			index = i_row*(DIMENSION+1);
			
			results[i_row] = temp[index];
			
			for(i_column=0; i_column<DIMENSION; i_column++) {
				results[i_row] += (temp[i_column+index+1] * X[i_column]);	
			}
		}
		
		MPI_Send(results, m, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD); 
	}
	
	/* Zeitmessung beenden */
	wtime[messung] = MPI_Wtime() - wtime[messung];
	wtick[messung] = MPI_Wtick() - wtick[messung];
	MPI_Barrier(MPI_COMM_WORLD);	

	//printf("wtime=%lf of process %d\n", wtime[messung], myrank);
	//printf("wtick=%lf of process %d\n", wtick[messung], myrank);

 } // for(messung : 0 < MESSUNGEN)

	MPI_Reduce(wtime, messungResultsWTIME, MESSUNGEN, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	MPI_Reduce(wtick, messungResultsWTICK, MESSUNGEN, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	
	free(temp); temp = NULL;
	free(results); results = NULL;
		
	if(myrank==0) {
		display(messungResultsWTIME, MESSUNGEN, "wtime");
		display(messungResultsWTICK, MESSUNGEN, "wtick");
	}
		
	/* MPI geordnet beenden */
	MPI_Finalize();

	return 0;
}
/* Ergebnis (p=8, DIMENSION=8)
cr@VisualGrid:~/programmingForCheckin/ain/trunk/schroeder_c$ mpiexec -np 8 ./hw9
Z[0]=205.000000000000
Z[1]=494.000000000000
Z[2]=783.000000000000
Z[3]=1072.000000000000
Z[4]=1361.000000000000
Z[5]=1650.000000000000
Z[6]=1939.000000000000
Z[7]=2228.000000000000
nprocs=8, m=1
nprocs=8, m=1
nprocs=8, m=1
nprocs=8, m=1
nprocs=8, m=1
nprocs=8, m=1
nprocs=8, m=1
nprocs=8, m=1
nprocs=8, m=1
nprocs=8, m=1
wtime[0]=0.000577926636
wtime[1]=0.000225067139
wtime[2]=0.001389980316
wtime[3]=0.000365018845
wtime[4]=0.000373840332
wtime[5]=0.000374078751
wtime[6]=0.000265836716
wtime[7]=0.000380992889
wtime[8]=0.000297069550
wtime[9]=0.000400781631
wtick[0]=0.000000000000
wtick[1]=0.000000000000
wtick[2]=0.000000000000
wtick[3]=0.000000000000
wtick[4]=0.000000000000
wtick[5]=0.000000000000
wtick[6]=0.000000000000
wtick[7]=0.000000000000
wtick[8]=0.000000000000
wtick[9]=0.000000000000
 */ 
