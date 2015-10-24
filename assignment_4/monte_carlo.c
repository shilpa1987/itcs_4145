#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include "mpi.h"

#define PI 3.14159265358979

int main (int argc, char **argv){
	int total = 0, N, S;
	double my_pi;
	double x, y;
	double diff, elapsed_time;
	double start, end;
	int seed, from, tag = 99, finished = 0, seeds_left;
	int response[2];
	
	MPI_Status status;
	int rank, size;
	
	MPI_Init (&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	MPI_Comm_size (MPI_COMM_WORLD, &size);
	int i, count = 0;
	
	if (rank == 0){
		printf ("how many iterations?\n");
		scanf ("%d", &N);
	
		printf ("how many seeds?\n");
		scanf ("%d", &S);
		
		seeds_left = S;
	}
	
	MPI_Bcast (&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	if (rank == 0) start = MPI_Wtime();
	
	if (rank == 0){
		for (i = 1; i < size; ++i){
			MPI_Send (&seeds_left, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
			seeds_left--;
		}
		
		while (finished < (size - 1)){
			MPI_Recv(response, 2, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
			
			if (response[1] == -1){
				finished++; 
			} else {
				total += response[1];
			}
			
			if (seeds_left == 0){
				MPI_Send (&seeds_left, 1, MPI_INT, response[0], tag, MPI_COMM_WORLD);
			} else {
				seeds_left--;
				MPI_Send (&seeds_left, 1, MPI_INT, response[0], tag, MPI_COMM_WORLD);
			}
		}
	} else {
		if (rank == 1) printf ("slave 1: N = %d\n", N);
		MPI_Recv (&seed, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
		while (seed != 0) {
			srand (seed);
			count = 0;
			for (i = 0; i < N; ++i){
				x = (float) rand () / RAND_MAX;
				y = (float) rand () / RAND_MAX;
		
				if (((x * x) + (y * y)) <= 1){
					count++;
				}
			}
			
			response[0] = rank;
			response[1] = count;
			
			MPI_Send (response, 2, MPI_INT, 0, tag, MPI_COMM_WORLD);
			
			MPI_Recv (&seed, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
		}
		
		response[1] = -1;
		
		MPI_Send (response, 2, MPI_INT, 0, tag, MPI_COMM_WORLD);
	}
	
	MPI_Barrier (MPI_COMM_WORLD);
	
	if (rank == 0){
		printf ("master: total = %d\n", total);
		end = MPI_Wtime();
		my_pi = 4 * ((float) total / (N * S));
		diff = PI - my_pi;
	
		elapsed_time = end - start;
	
		printf ("calculated pi: %lf\n", my_pi);
		printf ("difference: %lf\n", diff);
		printf ("total time = %lf\n", elapsed_time);
	}
	
	MPI_Finalize();
	
	return 0;
}
