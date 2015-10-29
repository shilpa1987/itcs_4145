// Suzaku Routines -- B. Wilkinson June 17, 2015

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>	// Needed for INT_MAX
#include <stdlib.h>  	// needed for malloc in workpool
#include "mpi.h" 

#define __sz_TERM INT_MAX // used in workpool

#define N 1		// Size of data being sent
#define P 4		// Number of processes

int graph[P][P];	// used in generalized pattern routines

void SZ_PrintGraph() {  // print out connection graph, for checking
		int i,j;
		int rank;
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		if (rank == 0) {		
			printf("Connection graph\n    ");
			for (j = 0; j < P; j++) printf("%2d ",j);
			for (i = 0; i < P; i++) {
				printf("\n%3d ",i);
				for (j = 0; j < P; j++)
					printf("%2d ",graph[i][j]);
			}
			printf("\n");
		} 
}

void SZ_Pattern(const char* pattern) {

	int i, j;	
	for (i = 0; i < P; i++)  			// initialize pattern table in all processes
		for (j = 0; j < P; j++)  
			graph[i][j] = -1;  		// initialize to no connection

	// set up pattern. All processes do the same to have identical copies
	if (strcmp(pattern, "all-to-all") == 0) {   	// all-to-all pattern
		for (i = 0; i < P; i++)  		// source process
			for (j = 0; j < P; j++)  	// destination process
				if (i != j) graph[i][j] = i;
	}

	if ((strcmp(pattern, "pipeline") == 0) || (strcmp(pattern, "ring") == 0)){  // pipeline pattern
		for (i = 0; i < P-1; i++)  
			graph[i][i+1] = 0;
		graph[P-1][0] = 0; // ring, back to master
	}

	if (strcmp(pattern, "stencil") == 0) {  // square stencil pattern
		int x,y,proc,sqrtP;
		sqrtP = (int) round(sqrt(P)); // must have an integer squareroot
		if (sqrtP * sqrtP != P) printf("ERROR number of processes does not have integer squareroot\n");
		for (y = 0; y < sqrtP; y++) // down -- natural ordering
		for (x = 0; x < sqrtP; x++) { // across
			proc = y * sqrtP + x;
			if (x - 1 >= 0)     graph[proc][proc-1] = 0;  		// left
			if (x + 1 < sqrtP)  graph[proc][proc+1] = 1; 		// right
			if (y - 1 >= 0)     graph[proc][proc - sqrtP] = 2;  	// up
			if (y + 1 < sqrtP)  graph[proc][proc + sqrtP] = 3; 	// down
		}
	}

	SZ_PrintGraph();		// for checking

	MPI_Barrier(MPI_COMM_WORLD); // make sure all processes have updated gragh

} // end of SZ_Pattern


void SZ_Send(double a[N], double b[P][N]) {

	int i,j,temp; 
	int tag = 2;
	int rank;
	MPI_Status status;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	for (i = 0; i < P; i++) 	// this code could potentially deadlock?
	for (j = 0; j < P; j++)  {
		temp = graph[i][j];	
		if (temp >= 0) {
			if (rank == i) 
				MPI_Send(a, N, MPI_DOUBLE, j, tag, MPI_COMM_WORLD);
			if (rank == j) 
				MPI_Recv(b[temp], N, MPI_DOUBLE, i, tag, MPI_COMM_WORLD, &status);
		}
	}
	MPI_Barrier(MPI_COMM_WORLD);

} // end of SZ_Send

void SZ_Workpool(void (*__sz_init)(int *T, int *D, int *R), void (*__sz_diffuse)(int taskID,double output[]),void (*__sz_compute)(int taskID, double input[], double output[]),void (*__sz_gather)(int taskID, double input[])  ) {

	int i; 
	int size; 
	int rank;     
	MPI_Status status; 

	int __sz_T, __sz_D, __sz_R; // # of tasks, # of data items in each task, # of data items in result of each task,

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	__sz_init(&__sz_T,&__sz_D,&__sz_R);

	double *task = (double*) malloc(__sz_D * sizeof(double));	
	double *result = (double*) malloc(__sz_R * sizeof(double)); 

	int taskID = 0;  // set to zero, programmer may alter this

	if (rank == 0) {   // Master
		int task_ptr = 0;	
		for (i = 1; i < size; i++) { 
			if (task_ptr < __sz_T) { 
				taskID = task_ptr;
				(*__sz_diffuse)(taskID,task);
				MPI_Send(task, __sz_D, MPI_DOUBLE, i, taskID, MPI_COMM_WORLD); 
				task_ptr++; 
			} else MPI_Send(task, 0, MPI_DOUBLE, i, __sz_TERM, MPI_COMM_WORLD); 
		} 
		for (i = 0; i < __sz_T; i++) { 
			MPI_Recv(result, __sz_R, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status); 
			taskID = status.MPI_TAG; 
			(*__sz_gather)(taskID,result); 
			if (task_ptr < __sz_T) {
				taskID = task_ptr;  
				(*__sz_diffuse)(taskID,task); 
				MPI_Send(task, __sz_D, MPI_DOUBLE, status.MPI_SOURCE, taskID, MPI_COMM_WORLD); 
				task_ptr++; 
			} else  MPI_Send(task, 0, MPI_DOUBLE, status.MPI_SOURCE, __sz_TERM, MPI_COMM_WORLD); 
		} 
	} 
	else {			// Slave
		int terminate = 0; 
		do { 
			MPI_Recv(task, __sz_D, MPI_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status); 
			taskID = status.MPI_TAG; 
			if (taskID == __sz_TERM) terminate = 1; 
			else { 
				(*__sz_compute)(taskID,task,result);	
				MPI_Send(result, __sz_R, MPI_DOUBLE, 0, taskID, MPI_COMM_WORLD); 
			} 
		} while (terminate == 0); 
	} 
	free (task);
	free (result);
} 
