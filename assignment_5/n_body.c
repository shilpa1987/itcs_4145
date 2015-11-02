#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define G 100

int main (int argc, char *argv[]){
	int T, t;
	double A[6][5] = {{25.0, 400.0, 400.0, 0.0, 0.0}, {20.0, 200.0, 400.0, 3.0, 4.0}, {30.0, 50.0, 600.0, 1.0, 0.0},
				   {50.0, 400.0, 200.0, 1.0, 0.0}, {40.0, 700.0, 700.0, -1.0, 0.0}, {70.0, 200.0, 100.0, -1.0, 0.0}}
	int N = 6;
	double x_diff, y_diff, r, F, Fx[N], Fy[N];
	int i, j, a;
	
	printf ("Number of time intervals?\n");
	scanf ("%f", &T);
	
	printf ("Length of time interval?\n");
	scanf ("%f", &t);
	
	for (i = 0; i < T; ++i){
		for (a = 0; a < N; ++a){
			for (j = 0; j < N; ++j){
				if (a != i){
					x_diff = ;
					y_diff = ;
					r = pow (((A[i][1] - A[a][1]) * (A[i][1] - A[a][1])) + ((A[i][2] - A[a][2]) * (A[i][2] - A[a][2])), 0.5);
					F = ;
					Fx[a] += ((G * A[a][0] * A[i][0]) / (r * r)) * (() / r);
					Fy[a] += ;
	} 
}
