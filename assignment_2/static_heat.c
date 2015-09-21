#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 10
#define T 5000

int main () {
	double h[N][N], g[N][N];
	int i, j, iteration;
	
	for (i = 0; i < N - 1; ++i) {
		for (j = 0; j < N - 1; ++j) {
			if (i == 0 && (j > 2 && j < 6)) {
				h[i][j] = 100;
			} else {
				h[i][j] = 20;
			}
		}
	}

	printf ("initial values: \n");	
	for (i = 0; i < N - 1; ++i) {
		for (j = 0; j < N - 1; ++j) {
				printf ("%.2f\t", h[i][j]);
		}
			printf ("\n");
	}
	
	for (iteration = 0; iteration < T; iteration++) {
 		for (i = 1; i < N-1; i++)
 			for (j = 1; j < N-1; j++)
 				g[i][j] = 0.25 * (h[i-1][j] + h[i+1][j] + h[i][j-1] + h[i][j+1]);

 	for (i = 1; i < N-1; i++) // update points
 		for (j = 1; j < N-1; j++)
 			h[i][j] = g[i][j];
 	}
	
	printf ("\nFinal values: \n");
	for (i = 0; i < N - 1; ++i) {
		for (j = 0; j < N - 1; ++j) {
				printf ("%.2f\t", h[i][j]);
		}
			printf ("\n");
	}
	
	return 0;		
}
