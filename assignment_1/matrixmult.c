#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N 256

int main (int argc, char *argv)
{
	omp_set_num_threads(16);
	
	int i, j, k, x;
	double sum;
	double start, end;
	double A[N][N], B[N][N], C[N][N], D[N][N];
	int count = 1;
	double average_seq, average_para;
	
	for(i = 0; i < N; ++i)
	{
		for (j = 0; j < N; ++j)
		{
			A[i][j] = j * 1;
			B[i][j] = i * j + 2;
		}
	}
	
		start = omp_get_wtime();
		for (i = 0; i < N; ++i)
		{
			for (j = 0; j < N; ++j)
			{
				sum = 0;
				for (k = 0; k < N; ++k)
				{
					sum += A[i][k] * B[k][j];
				}
				C[i][j] = sum;
			}
		}
		end = omp_get_wtime();
		average_seq += end - start;
	
		//parallelized multiplication
		start = omp_get_wtime();
		#pragma omp parallel for private(j)
		for (i = 0; i < N; ++i)
		{
			#pragma omp parallel for private(sum, k)
			for (j = 0; j < N; ++j)
			{
				sum = 0;
				for (k = 0; k < N; ++k)
				{
					sum += A[i][k] * B[k][j];
				}
				D[i][j] = sum;
			}
		}
		end = omp_get_wtime();
		average_para += end - start;
	
		int error = 0;
		for (i = 0; i < N; ++i)
		{
			for (j = 0; j < N; ++j)
			{
				if ((C[i][j] - D[i][j] > 0.001) || (D[i][j] - C[i][j] > 0.001))
					error = -1;
			}
		}
		if(error == -1) {printf("ERROR, sequential and parallel versions give different answers\n"); }
	
	printf("average sequential runtime: %f seconds\n", average_seq / count);
	printf("average parallel runtime: %f seconds\n", average_para / count);
	
	return 0;
}
