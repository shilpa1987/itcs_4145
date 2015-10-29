#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "suzaku.h"

#define T 100
#define D 1
#define R 1

#define S 1000000
double total = 0;

void init (int *tasks, int *data_items, int *result_items){
	*tasks = T;
	*data_items = D;
	*result_items = R;
}

void diffuse (int taskID, double output[D]){
	static int temp = 0;
	output[0] = ++temp;
}

void compute (int taskID, double input[D], double output[R]){
	int i;
	double x, y;
	double inside = 0;
	
	srand (input[0]);
	for (i = 0; i < S; ++i){
		x = rand () / (double) RAND_MAX;
		y = rand () / (double) RAND_MAX;
		if ((x * x + y * y) <= 1) inside++;
	}
	output[0] = inside;
}

void gather (int taskID, double input[R]){
	total += input[0];
}

double get_pi (){
	double pi;
	pi = 4 * total / (S * T);
	printf ("\nWorkpool results, Pi = %f\n", pi);
}

int main (int argc, char *argv[]){
	int i;
	int P;
	double time1, time2;
	
	SZ_Init (P);
	
	printf ("number of tasks = %d\n", T);
	printf ("number of samples done in slave per task = %d\n", S);
	
	time1 = SZ_Wtime ();
	SZ_Parallel_begin
		SZ_Workpool (init, diffuse, compute, gather);
	SZ_Parallel_end
	
	time2 = SZ_Wtime ();
	
	get_pi ();
	printf ("elapsed_time = %f (seconds)\n", time2 - time1);
	
	SZ_Finalize ();
	return 0;
}
