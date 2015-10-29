#include <stdio.h>
#include <string.h>
#include "suzaku.h"

int main (int argc, char *argv[]){
	int i, P, PID;
	double a[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	double b[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	char a_message[20];
	char b_message[20];
	strcpy (a_message, "Hello, world");
	strcpy (b_message, "------------");
	double p, q;
	p = 123;
	
	SZ_Init (P);
	
	SZ_Parallel_begin
	
		PID = SZ_Get_process_num ();
		
		SZ_Point_to_point (0, 1, a_message, b_message);
		
		if (PID == 1) printf ("Received by process %d = %s\n", PID, b_message);
		
		SZ_Point_to_point (0, 1, a, b);
		if (PID == 1){
			printf ("Received by process %d = ", PID);
			for (i = 0; i < 10; ++i)
				printf ("%2.2f ", b[i]);
			printf ("\n");
		}
		
		SZ_Point_to_point (0, 1, &p, &q);
		if (PID == 1) printf ("Received by process %d = %f\n", PID, q);
	
	SZ_Parallel_end;
	
	SZ_Finalize ();
	return 0;
}
