/*
 * Copyright (C) 2010 Christian Benjamin Ries
 * Website: https://github.com/cbries/utilities
 * License: MIT 
 */
#include <mpi.h>
#include <math.h>
#include <stdio.h>

#include <iostream>

using namespace std;

float fct(float x) {
      return cos(x);
}

/* Prototype */
float integral(float a, int n, float h);

/**
 * This is one of the MPI versions on the integration example
 * It demonstrates the use of :
 * 1) MPI_Init
 * 2) MPI_Comm_rank
 * 3) MPI_Comm_size
 * 4) MPI_Reduce
 * 5) MPI_Finalize
 */
int main( int argc, char **argv) {

      int n, p, num;
      float h, result, a, b, pi;
      float my_a, my_range;

      int myid, dest, tag;
      float my_result;
      float exact_result;

			/* = 3.14159... */
      pi = acos(-1.0);
      /* lower limit of integration */
      a = 0.0;
      /* upper limit of integration */
      b = 2.0*M_PI;
      /* number of increment within each process */
      n = 100000;

      exact_result = sin(b) - sin(a);

			/* define the process that computes the final result */
      dest = 0;
      /* set the tag to identify this particular job */
      tag = 123;

			/* starts MPI */
      MPI_Init(&argc,&argv);
      /* get current process id */
      MPI_Comm_rank(MPI_COMM_WORLD, &myid);
      /* get number of processes */
      MPI_Comm_size(MPI_COMM_WORLD, &p);

			/* length of increment */
      h = (b-a)/n;
      /* number of intervals calculated by each process*/
      num = n/p;
      my_range = (b-a)/p;
      my_a = a + myid*my_range;
      my_result = integral(my_a,num,h);

      printf("Process %d has the partial result of %f\n", myid, my_result);

			/* Use an MPI sum reduction to collect the results */		      
      MPI_Reduce(&my_result, &result,1,MPI_REAL,MPI_SUM,0,MPI_COMM_WORLD);

      if(myid == 0) {
        printf("The result =%f\n",result);
        printf("The exact value =%f\n",exact_result);
        printf("Error (compared to exact value) in percent =%f\n",(exact_result - result)/exact_result*100.);
			} else {
			}
			/* let MPI finish up ... */
      MPI_Finalize();
}

float integral(float a, int n, float h) {
      int j;
      float h2, aij, integ;

			/* initialize integral */
      integ = 0.0;
      h2 = h/2.;
      /* sum over all "j" integrals */
      for (j=0;j<n;j++) {
				/* lower limit of "j" integral */
        aij = a + j*h;
        integ += fct(aij+h2)*h;
      }
      return (integ);
}
