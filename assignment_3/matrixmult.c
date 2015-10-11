#define N 256
#include <stdio.h>
#include <sys/time.h>
#include "mpi.h"

void print_results (char *prompt, double a[N][N]){
	int i, j;
	printf ("\n\n%s\n", prompt);
	for (i = 0; i < N; ++i){
		for (j = 0; j < N; ++j){
			printf(" %.2lf", a[i][j]);
		}
		printf ("\n");
	}
	printf ("\n\n");
}

int main (int argc, char *argv[]){
	double *x;
	int i, j, k, error = 0;
	double a[N][N], b[N][N], c[N][N], d[N][N];
	double start, end;
	
	MPI_Status status;
	int rank, P, blksz;
	
	MPI_Init (&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	MPI_Comm_size (MPI_COMM_WORLD, &P);
	
	if ((rank == 0) && (N % P != 0)) {
		error = -1;
		printf ("Error -- N/P must be an integer\n");
	} else blksz = N/P;
	
	if (rank == 0){
		for (i = 1; i < P; ++i){
			MPI_Send(&error, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
	} else {
		MPI_Recv (&error, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
	}
	
	if (error != 0) {
		printf ("This process %d... An error occurred...I am shutting down....\n", rank);
		MPI_Finalize();
		
		return 0;
	}
	
	if (rank == 0){
		for (i = 0; i < N; ++i)
			for (j = 0; j < N; ++j){
				a[i][j] = (j * 1) + 1;
				b[i][j] = (j * i) + 2;
			}
		
		start = MPI_Wtime();
		for (i = 0; i < N; ++i){
			for (j = 0; j < N; ++j){
				c[i][j] = 0;
				for (k = 0; k < N; ++k){
					c[i][j] += a[i][k] * b[k][j];
				}
			}
		}
		end = MPI_Wtime();
		
		printf ("sequentl= \t%lf (seconds)\n", end - start);
		
		x = malloc(blksz*N);
	}
	
	MPI_Barrier (MPI_COMM_WORLD);
	
	if (rank == 0) start = MPI_Wtime();
	
	MPI_Scatter (a, blksz*N, MPI_DOUBLE, x, blksz*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast (b, N*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	
	for (i = 0; i < blksz; ++i){
		for (j = 0; j < N; ++j){
			d[i][j] = 0;
			for (k = 0; k < N; ++k){
				d[i][j] += a[i][k] * b[k][j];
			}
		}
	}
	
	MPI_Gather (d, blksz*N, MPI_DOUBLE, x, blksz*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	
	if (rank == 0) {
		end = MPI_Wtime();
		printf ("parallel=\t%lf (seconds)\n", end - start);
		
		error = 0;
		for (i = 0; i < N; ++i)
		{
			for (j = 0; j < N; ++j)
			{
				if ((c[i][j] - d[i][j] > 0.001) || (d[i][j] - c[i][j] > 0.001))
					error = -1;
			}
		}
		if(error == -1) {printf("ERROR, sequential and parallel versions give different answers\n"); }
	}
	MPI_Finalize();
	return 0;
}		
