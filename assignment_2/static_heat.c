#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define X_RESN 1000
#define Y_RESN 1000

int main () {
	int i, j, iteration, T, N;
	double h[2][256][256], g[2][256][256];
	int current = 0;
	int next = 1;
	double seq_start, seq_end, par_start, par_end;
	
	printf("How big is the room?\n");
	scanf("%d", &N);
	
	printf("How many iterations?\n");
	scanf("%d", &T);
	
	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			if (i == 0 && (j > N * 0.30 && j < N * 0.70)) {
				h[0][i][j] = 100;
				h[1][i][j] = 100;
				g[0][i][j] = 100;
				g[1][i][j] = 100;
			} else {
				h[0][i][j] = 20;
				h[1][i][j] = 20;
				g[0][i][j] = 20;
				g[1][i][j] = 20;
			}
		}
	}
	
	printf("Sequential Initial Values:\n");
	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			if(j % 10 == 0 && i % 10 == 0)
				h[0][i][j];
		}
		if(i % 10 == 0)
			printf ("\n");
	}
	
	seq_start = omp_get_wtime();
	for (iteration = 0; iteration < T; iteration++) {
 		for (i = 1; i < N - 1; i++)
 			for (j = 1; j < N - 1; j++) {
				h[next][i][j] = 0.25 * (h[current][i + 1][j] + h[current][i - 1][j] + h[current][i][j - 1]  + h[current][i][j + 1]);
			}
 		current = next;
 		next = 1 - current;
 	}
 	seq_end = omp_get_wtime();
 	
	printf("\nSequential Final Values\n");
	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			if(j % 10 == 0 && i % 10 == 0)
				printf ("%.2f\t", h[current][i][j]);
		}
		if(i % 10 == 0)
			printf ("\n");
	}

	printf("\nParallel Initial Values\n");
	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			if(j % 10 == 0 && i % 10 == 0)
				printf ("%.2f\t", g[current][i][j]);
		}
		if(i % 10 == 0)
			printf ("\n");
	}
	
	par_start = omp_get_wtime();
	for (iteration = 0; iteration < T; iteration++) {
		#pragma omp parallel for private (i, j)
 		for (i = 1; i < N - 1; i++)
 			#pragma omp parallel for private (j)
 			for (j = 1; j < N - 1; j++) {
				g[next][i][j] = 0.25 * (g[current][i + 1][j] + g[current][i - 1][j] + g[current][i][j - 1]  + g[current][i][j + 1]);
			}
 		current = next;
 		next = 1 - current;
 	}
 	par_end = omp_get_wtime();
 	
	printf("\nSequential Final Values\n");
	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			if(j % 10 == 0 && i % 10 == 0)
				printf ("%.2f\t", g[current][i][j]);
		}
		if(i % 10 == 0)
			printf ("\n");
	}
	
	int error = 0;
	for (i = 0; i < N; ++i)
	{
		for (j = 0; j < N; ++j)
		{
			if ((h[current][i][j] - g[current][i][j] > 0.001) || (h[current][i][j] - g[current][i][j] > 0.001))
				error = -1;
		}
	}
	if(error == -1) {printf("ERROR, sequential and parallel versions give different answers\n"); }
	
	printf("\nsequential time: %f\n", seq_end - seq_start);
	printf("parallel time: %f\n", par_end - par_start);
	
	return 0;		
}
