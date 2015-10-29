// Suzaku macros, version 1.00
// Implemented by B. Wilkinson 
// Modification date: October 19, 2015. See Implementation Notes for more information on implementation.

#include "mpi.h"

// Suzaku global variables
	int __sz_count; 
	int __sz_size;     
	int __sz_rank;     
	int __sz_tag=1; 
	int __sz_datasize;
	int __sz_i;    
	int __sz_j;
	MPI_Status __sz_status; 

// Suzaku macros

#define SZ_Init(P) \
	MPI_Init(&argc, &argv); \
	MPI_Comm_size(MPI_COMM_WORLD, &__sz_size); \
	MPI_Comm_rank(MPI_COMM_WORLD, &__sz_rank); \
	P = __sz_size; \
	if (__sz_rank == 0) {
	
#define SZ_Get_process_num() \
	__sz_rank

#define SZ_Master \
	if (__sz_rank == 0) 

#define SZ_Process(x) \
	if (__sz_rank == x)                     

#define SZ_Parallel_begin \
	} \
	// blank line 

#define SZ_Parallel_end \
	MPI_Barrier(MPI_COMM_WORLD);\
	if (__sz_rank == 0) { 

#define SZ_Finalize()    \
	} \
	MPI_Finalize()

#define SZ_Point_to_point(p1, p2, a, b) \
do {\
	__sz_datasize = sizeof(*(a)); \
	if (sizeof(a) == sizeof(double*)) __sz_count = 1; else __sz_count = sizeof(a) / __sz_datasize;\
	if (__sz_datasize == sizeof(double)) {  \
	  if (__sz_rank == p1)  \
		MPI_Ssend(a, __sz_count, MPI_DOUBLE, p2, __sz_tag, MPI_COMM_WORLD); \
	  if (__sz_rank == p2)  \
		MPI_Recv(b, __sz_count, MPI_DOUBLE, p1, __sz_tag, MPI_COMM_WORLD, &__sz_status);\
	} \
	else if (__sz_datasize == sizeof(char)) { \
	  if (__sz_rank == p1)  \
		MPI_Ssend(a, __sz_count, MPI_CHAR, p2, __sz_tag, MPI_COMM_WORLD); \
	  if (__sz_rank == p2)  \
		MPI_Recv(b, __sz_count, MPI_CHAR, p1, __sz_tag, MPI_COMM_WORLD, &__sz_status);\
	}\
} while (0)

#define SZ_Scatter(a, b) \
do {\
	__sz_count = sizeof(a) / ( sizeof(double) * __sz_size); \
	MPI_Scatter(a,__sz_count,MPI_DOUBLE,b,__sz_count,MPI_DOUBLE,0,MPI_COMM_WORLD);\
	MPI_Barrier(MPI_COMM_WORLD);\
} while (0)

#define SZ_Broadcast(a) \
do {\
	if (sizeof(a) == sizeof(double*)) __sz_count = 1; else __sz_count = sizeof(a) / sizeof(double);\
	MPI_Bcast(a, __sz_count, MPI_DOUBLE, 0, MPI_COMM_WORLD); \
	MPI_Barrier(MPI_COMM_WORLD);\
} while (0)
// Notes: if size of double ptr, 4, must be a single variable, e.g. &x, smallest array is x[1] 8 bytes

#define SZ_AllBroadcast(a) \
do {\
	__sz_count = sizeof(a) / (sizeof(double) * __sz_size); \
	for (__sz_i = 0; __sz_i < __sz_size; __sz_i++)  \
	  MPI_Bcast(a[__sz_i], __sz_count, MPI_DOUBLE, __sz_i, MPI_COMM_WORLD);\
	MPI_Barrier(MPI_COMM_WORLD);\
} while (0)

#define SZ_Gather(a, b) \
do {\
	__sz_count = sizeof(a) / (sizeof(double)  * __sz_size); \
	MPI_Gather(a,__sz_count,MPI_DOUBLE,b,__sz_count,MPI_DOUBLE,0,MPI_COMM_WORLD); \
	MPI_Barrier(MPI_COMM_WORLD);\
} while (0)

#define SZ_Barrier() \
	MPI_Barrier(MPI_COMM_WORLD)

#define SZ_Wtime() \
	MPI_Wtime()

// Workpool function prototype

extern void SZ_Workpool(void (*__sz_init)(int *T, int *D, int *R), void (*__sz_diffuse)(int taskID,double output[]),void (*__sz_compute)(int taskID, double input[], double output[]),void (*__sz_gather)(int taskID, double input[])  );

// Generalized pattern routines

extern void SZ_Pattern(const char* pattern);
// extern void SZ_Send(double a[N], double b[P][N]);
extern void SZ_PrintGraph(void);
