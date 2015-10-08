#include <stdio.h>
#include <stdlib.h>

#define N 512

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

int main(int argc, char *argv[]){
	int i, j, k, sum;
	double a[N][N], b[N][N], c[N][N];
	FILE *fd;
	
	fd = fopen (argv[1], "r");
	
	if (fd == NULL){
		printf ("file not found");
		return 0;
	}
		
	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++);
			fscanf(fd, "%lf", &a[i][j]);
	for (i = 0; i < N; ++i)
		for (j = 0; j < N; j++);
			fscanf(fd, "%lf", &b[i][j]);
	
	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++){
			sum = 0;
			for (k = 0; k < N; ++k){
				sum += a[i][k] * b[k][j];
			}
			c[i][j] = sum;
		}
		
	print_results ("C= \n", c);
	
	return 0;
}
