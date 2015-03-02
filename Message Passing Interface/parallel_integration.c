/*
 * Copyright (C) 2010 Christian Benjamin Ries
 * Website: https://github.com/cbries/utilities
 * License: MIT 
 */
#include <mpi.h>
#include <math.h>
#include <stdio.h>

#define UNTERGRENZE			0
#define OBERGRENZE			2*M_PI
#define UNTERTEILUNGEN	10

inline double fct(double x) {
	return cos(x);
}

inline double exact(double a, double b) {
	double sa = sin(a);
	double sb = sin(b);
	return sb - sa;
}

/// Funktionsprototyp, siehe Ende der Datei!
double integral(double a, int n, double h);

/**
 * 1) MPI_Init
 * 2) MPI_Comm_rank
 * 3) MPI_Comm_size
 * 4) MPI_Recv
 * 5) MPI_Send
 * 6) MPI_Finalize
 */
int main(int argc, char**argv) {            
	/// Untergrenze der Integration
	double a = UNTERGRENZE;
	/// Obergrenze der Integration
	double b = OBERGRENZE;
	/// Unterteilungen in den jeweiligen Berechnungen
	int n = UNTERTEILUNGEN;

	/// Berechnetes Ergebnis
	double result = 0.0f;
	/// Exaktes Ergebnis fuer die Endpruefung
	double exact_result = exact(a,b);

	/// Identifikationsnummer fuer den Empfaenger
	/// aller Berechnungsergebnisse
	int dest = 0;
	/// Identifikationsnummer fuer die Kommunikation zwischen
	/// den Prozessen
	int tag = 20100325;

	/// Variablen fuer MPI
	int myID = 0;
	int prozessAnzahl = 0;

	MPI_Status status;      
	
	/// MPI starten
	MPI_Init(&argc,&argv);
	/// Identifikationsnummer des aktuellen Prozesses
	MPI_Comm_rank(MPI_COMM_WORLD, &myID);
	/// Anzahl der Prozesse ermitteln
	MPI_Comm_size(MPI_COMM_WORLD, &prozessAnzahl);

	/// Laenge eines Schrittes
	double schrittlaenge = (b-a) / n;
	/// Anzahl der Intervalle jedes Prozesses
	int num = n / prozessAnzahl;
      
	double my_range = (b-a) / prozessAnzahl;
	double my_a = a + myID * my_range;
	double my_result = integral( my_a, num, schrittlaenge);

	if(myID == 0) {
		double my_result = 0.0f;
		
		for (int i=1; i < prozessAnzahl; i++) {
			MPI_Recv(&my_result, 1, MPI_REAL, i, tag, MPI_COMM_WORLD, &status);
			result += my_result;
		}

		/// Ausgabe der Ergebnisse!
		printf("Ergebnisse (float)\n");
		printf("------------------\n");
		printf("Ergebnis der MPI Berechnung = %.18lf\n", result);
		printf("Ergebnis der direkten Berechnung = %.18lf\n", exact_result);
		printf("Fehlerrate (float): %.2f%%\n", ((float)exact_result-(float)result)/(float)exact_result*100.0f);

		printf("Ergebnisse (double)\n");
		printf("------------------\n");
		printf("Ergebnis der MPI Berechnung = %.18f\n", result);
		printf("Ergebnis der direkten Berechnung = %.18f\n", exact_result);
		printf("Fehlerrate (double): %.2lf%%\n", (exact_result-result)/exact_result*100.0f);
	} else {

		printf("Prozess %d - Ich habe die Teilloesung = %lf\n", myID, my_result);
		
		MPI_Send(&my_result, 1, MPI_REAL, dest, tag, MPI_COMM_WORLD);
	}
	MPI_Finalize();
}

/**
 * @param a
 * @param n
 * @param h
 * @return The result of the function in the range(a, a+(n*h))
 */
double integral(double a, int n, double h) {
      int j;
      double h2, a_ij, integ;

      integ = 0.0;

      h2 = h/2.;

      for (j=0;j<n;j++) {
        a_ij = a + j*h;
        integ += fct(a_ij + h2)*h;
      }

      return (integ);
}
