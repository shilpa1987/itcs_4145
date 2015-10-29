#define N 64
#include <stdio.h>
#include "suzaku.h"

int main (int argc, char *argv[]){
	int i, j, k, error = 0;
	double A[N][N], B[N][N], C[N][N], D[N][N], sum;
	double A1[N][N];
	double C1[N][N];
	double time1, time2;
	int P;
	int blksz;
	
	SZ_Init (P);
	
	if (N % P != 0) printf ("Error -- N/P must be an integer\n");
	
	for (i = 0; i < N; ++i)
		for (j = 0; j < N; ++j)
			A[i][j] = j * 1; B[i][j] = i * j + 2;
			
	for (i = 0; i < N; ++i)
		for (j = 0; j < N; ++j){
			sum = 0;
			for (k = 0; k < N; ++k)
				sum += A[i][k] * B[k][i];
			D[i][j] = sum;
		}
		
	time1 = SZ_Wtime ();
	SZ_Parallel_begin
		SZ_Scatter (A, A1);
		SZ_Broadcast (B);
		
		blksz = N/P;
		for (i = 0; i < N; ++i){
			for (j = 0; j < N; ++j){
				sum = 0;
				for (k = 0; k < N; ++k){
					sum += A1[i][k] * B[k][j];
				}
				C1[i][j] = sum;
			}
		}
		
		SZ_Gather (C1, C);
	SZ_Parallel_end;
	time2 = SZ_Wtime ();
	
	int error = 0;
	for (i = 0; i < N; ++i)
		for (j = 0; j < N; ++j)
			if ((C[i][j] - D[i][j] > 0.001) || (D[i][j] - C[i][j] > 0.001)) error = -1;
			
	if (error == -1) printf ("ERROR, sequential and parallel code give different answers.\n");
	else printf ("Sequential and parallel code give same answers.\n");
	
	printf ("elapsed_time = %f (seconds)\n", time2 - time1);
	
	SZ_Finalize ();
	return 0;
}
