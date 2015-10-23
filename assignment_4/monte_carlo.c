#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define PI 3.14159265358979
#define N 10000

int main (){
	int i, count;
	double my_pi;
	double x, y;
	double diff;
	time_t time;
	
	srand (time (&time));
	
	for (i = 0; i < N; ++i){
		x = rand () / MAX_INT;
		y = rand () / MAX_INT;
		
		if (((x ^ 2) + (y ^ 2)) <= 1)
			count++;
	}
	
	my_pi = 4 * (count / N);
	
	diff = PI - my_pi;
	
	printf ("calculated pi: %f\n", my_pi);
	printf ("difference: %f\n", diff);
	
	return 0;
}
