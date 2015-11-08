#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define G 100

int main (int argc, char *argv[]){
	float t;
	int T;
	double A[6][5] = {{25.0, 400.0, 400.0, 0.0, 0.0}, {20.0, 200.0, 400.0, 3.0, 4.0}, {30.0, 50.0, 600.0, 1.0, 0.0},
				   {50.0, 400.0, 200.0, 1.0, 0.0}, {40.0, 700.0, 700.0, -1.0, 0.0}, {70.0, 200.0, 100.0, -1.0, 0.0}};
	int N = 6;
	double x_diff, y_diff, r, F, Fx[N], Fy[N], new_x[N], new_y[N];
	int i, j, a;
	
	printf ("Number of time intervals?\n");
	scanf ("%d", &T);
	
	printf ("Length of time interval?\n");
	scanf ("%f", &t);
	
	printf ("\n\n");
	printf ("Mass\tx pos\ty pos\tx vel\t y vel\n");
	
	for (i = 0; i < 6; ++i){
		for (j = 0; j < 5; ++j){
			printf ("%.2f\t", A[i][j]);
		}
		printf ("\n");
	}
	
	for (i = 0; i < T; ++i){
		for (a = 0; a < N; ++a){
			Fx[a] = A[a][3];
			Fy[a] = A[a][4];
			
			for (j = 0; j < N; ++j){
				if (a != j){
					x_diff = A[j][1] - A[a][1];
					y_diff = A[j][2] - A[a][2];
					r = sqrt ((x_diff * x_diff) + (y_diff * y_diff));
					F = (G * A[a][0] * A[j][0]) / (r * r);
					Fx[a] += F * ((A[a][3] - A[j][3]) / r);
					Fy[a] += F * ((A[a][4] - A[j][4]) / r);
					
					if (((A[a][0] + A[j][0]) / 2) > r){
						A[a][0] = 0.0;
						A[j][0] = 0.0;
					}
				}
			}
			
			new_x[a] = (Fx[a] * t) + A[a][1];
			new_y[a] = (Fy[a] * t) + A[a][2];
			
			if (new_x[a] > 1000.0){
				new_x[a] = 1000.0 - (new_x[a] - 1000.0);
			} else if (new_x[a] < 0){
				new_x[a] = new_x[a] * -1; 
			}
			
			if (new_y[a] > 1000.0){
				new_y[a] = 1000.0 - (new_y[a] - 1000.0);
			} else if (new_y[a] < 0){
				new_y[a] = new_y[a] * -1;  
			}
			
			A[a][3] = Fx[a];
			A[a][4] = Fy[a];
			A[a][1] = new_x[a];
			A[a][2] = new_y[a];
		}
	}
	
	printf ("\n");
	printf ("Mass\tx pos\ty pos\tx vel\t y vel\n");
	
	for (i = 0; i < 6; ++i){
		for (j = 0; j < 5; ++j){
			printf ("%.2f\t", A[i][j]);
		}
		printf ("\n");
	} 
}
