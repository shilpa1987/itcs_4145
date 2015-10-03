#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main () {
	int i, j, iteration;
	double *h;
	int current = 0;
	int size = 100, t = 5000;
	
	printf ("Enter a size of room (default is 100)\n");
	scanf("%d", &size);
	printf("How many iterations (default is 5000)\n");
	scanf("%d", &t);
	
	int next = size ^ 2;
	
	h = calloc(2 * size * size, sizeof(double));
	
	for (i = 0; i < size; ++i) {
		for (j = 0; j < size; ++j) {
			if (i == 0 && (j > size * 0.4 && j < size * .60)) {
				h[(size * i) + j] = 100;
				h[(size ^ 2) + (size * i) + j] = 100;
			} else {
				h[(size * i) + j] = 20;
				h[(size ^ 2) + (size * i) + j] = 20;
			}
		}
	}
	
	printf("Initial Values:\n");
	for (i = 0; i < size; ++i) {
		for (j = 0; j < size; ++j) {
			if(j % 10 == 0 && i % 10 == 0)
				printf ("%.2f\t", h[current + (i * size) + j]);
		}
		if(i % 10 == 0)
			printf ("\n");
	}
	
	for (iteration = 0; iteration < t; iteration++) {
 		for (i = 1; i < size - 1; i++)
 			for (j = 1; j < size - 1; j++) {
				h[next + (size * i) + j] = 0.25 * (h[current + (size * (i + 1)) + j] + h[current + (size * (i - 1)) + j] + h[current + (size * i) + (j + 1)]  + h[current + (size * i) + (j - 1)]);
			}
 		current = next;
 		next = (size ^ 2) - current;
 	}
	
	printf ("\nSequential final values:\n");
	for (i = 0; i < size; ++i) {
		for (j = 0; j < size; ++j) {
			if(j % 10 == 0 && i % 10 == 0)
				printf ("%.2f\t", h[current + (i * size) + j]);
		}
		if(i % 10 == 0)
			printf ("\n");
	}
	
	return 0;		
}
