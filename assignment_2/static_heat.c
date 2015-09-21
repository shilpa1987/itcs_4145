#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 100
#define T 5000

int main () {
	int i, j, iteration;
	double h[2][N][N];
	int current = 0;
	int next = 1;
	double start, end, par_time, seq_time;
	
	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			if (i == 0 && (j > 39 && j < 60)) {
				h[0][i][j] = 100;
				h[1][i][j] = 100;
			} else {
				h[0][i][j] = 20;
				h[1][i][j] = 20;
			}
		}
	}
	
	printf("Initial Values:\n");
	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			if(j % 10 == 0 && i % 10 == 0)
				printf ("%.2f\t", h[current][i][j]);
		}
		if(i % 10 == 0)
			printf ("\n");
	}
	
	start = omp_get_wtime();
	for (iteration = 0; iteration < T; iteration++) {
 		for (i = 1; i < N - 1; i++)
 			for (j = 1; j < N - 1; j++) {
				h[next][i][j] = 0.25 * (h[current][i + 1][j] + h[current][i - 1][j] + h[current][i][j - 1]  + h[current][i][j + 1]);
			}
 		current = next;
 		next = 1 - current;
 	}
 	end = omp_get_wtime();
 	seq_time = end - start;
	
	printf ("\nSequential final values:\n");
	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			if(j % 10 == 0 && i % 10 == 0)
				printf ("%.2f\t", h[current][i][j]);
		}
		if(i % 10 == 0)
			printf ("\n");
	}
	
	//reset the values
	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			if (i == 0 && (j > 39 && j < 60)) {
				h[0][i][j] = 100;
				h[1][i][j] = 100;
			} else {
				h[0][i][j] = 20;
				h[1][i][j] = 20;
			}
		}
	}
	
	//TODO: parallelize this bitch
	start = omp_get_wtime();
	for (iteration = 0; iteration < T; iteration++) {
 		for (i = 1; i < N - 1; i++)
 			for (j = 1; j < N - 1; j++) {
				h[next][i][j] = 0.25 * (h[current][i + 1][j] + h[current][i - 1][j] + h[current][i][j - 1]  + h[current][i][j + 1]);
			}
 		current = next;
 		next = 1 - current;
 	}
	end = omp_get_wtime();
	par_time = end - start;
	
	printf ("\nSequential final values:\n");
	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			if(j % 10 == 0 && i % 10 == 0)
				printf ("%.2f\t", h[current][i][j]);
		}
		if(i % 10 == 0)
			printf ("\n");
	}
	
	printf ("Sequential time: %f\n", seq_time);
	printf ("Parallel Time: %f\n", par_time);
	
	return 0;		
}
