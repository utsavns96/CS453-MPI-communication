#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>


short int *A_init(int n){
    short int *A = (short int *) malloc(n * sizeof(short int));
    srand(time(NULL)); 
    // assign array A randomly {-1,0,1} 
    for (int i = 0; i < n; i++) {
    A[i] = rand() % 3 - 1;
    }
	return A;
}

int getSubarray(int n, int p, int rank, int *offsetter) {
int size = n / p, bin;

if (offsetter == NULL)
// if offsetter == NULL, put values in discard 
    offsetter = &bin; 
*offsetter = size * rank;

/* if n is not a multiple of p, let the first subarray be bigger by one element */ 
if (rank < n % p) {
size++;
*offsetter += rank; 
}
else{
*offsetter += n % p;
}
return size;
}

int main(int argc, char* argv[]) {
    int world_size, world_rank, i, size, offsetter, total, total_receive; 
    double start, end;
	
    short int *A;
    MPI_Request request;
	MPI_Status status;
    // Initialization
    MPI_Init(NULL, NULL); 
    MPI_Comm_size(MPI_COMM_WORLD, &world_size); 
    MPI_Comm_rank (MPI_COMM_WORLD, &world_rank);

    // Create A 
    int n = atoi(argv[2]);
    size = getSubarray(n, world_size, world_rank, &offsetter);
    if (world_rank == 0) {
        start = MPI_Wtime();
    // root node 
    A = A_init(n); 
	// check time here
    }
    else {
    // for the sub nodes 
    A = (short int *) malloc((n - offsetter) * sizeof(short int));
    // start listening for data 
    MPI_Recv(A, n - offsetter, MPI_SHORT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
    }
    // send left over array.  
    if (world_rank < world_size - 1)
        MPI_Isend(A + size, n - offsetter - size, MPI_SHORT, world_rank + 1, 0, MPI_COMM_WORLD, &request);
    // compute sum of sub-node 
    for (i = 0, total = 0; i < size; i++)
    total += A[i];
    if (world_rank != 1) {
    // receive from previous node except for source node
    MPI_Recv(&total_receive, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
    total += total_receive ;
    }

    if (world_rank != 0){
        // send result to next node 
        MPI_Send(&total, 1, MPI_INT, (world_rank + 1) % world_size, 1, MPI_COMM_WORLD);
    }

    if (world_rank == 0) {
        // check time here
        // print total 
        end = MPI_Wtime();
        printf("\n From processor %d: total sum is %d \n", world_rank, total);
        printf("The program ran for %f\n", end - start);
    }
    // Finalize the MPI environment. 
    MPI_Finalize();
    free(A); 
    return 0;
}
