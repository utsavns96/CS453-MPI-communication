#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <math.h>

void print_coords(int *coords, int m) {
printf("(%d", coords[0]); 
for (int i = 1; i < m; i++) 
    printf(", %d", coords[i]); 
    printf(")");
}

short int *A_init(int n){
    short int *A = malloc(n * sizeof(int));
    srand(time(NULL)); 
    // assign array A randomly {-1,0,1} 
    for (int i = 0; i < n; i++) {
    A[i] = rand() % 3 - 1;
    }
    return A;
}

// int main(int n, int m) {
int main(int argc, char* argv[]) {
    int n = atoi(argv[2]);
    int procs = atoi(argv[1]);
    int m = log2(procs);
    int world_size, world_rank, i, size; 
    int group_size, buffer_size, level, total, tmp;  
    short int *A; 
    
    MPI_Request request;
    // Initialize the MPI environment 
    MPI_Init(NULL, NULL); 
    MPI_Comm_size(MPI_COMM_WORLD, &world_size); 
    MPI_Comm_rank (MPI_COMM_WORLD, &world_rank);
    MPI_Status status;
    // Initialize Cartesian environment
    MPI_Comm comm_cart; 
    int dims[m], periods[m], cord[m];
    for (i = 0; i < m; i++) {
        dims[i] = 0; 
        periods[i] = 1;
    }
    MPI_Dims_create(world_size, m, dims);
    MPI_Cart_create(MPI_COMM_WORLD, m, dims, periods, 1, &comm_cart); 
    MPI_Cart_coords(comm_cart, world_rank, m, cord);
    //printf("rank %d coor1 %d coor2 %d\n", world_rank, cord[0], cord[1]);
    size = n / world_size; 
    // level = m is a subnode 
    // level = 0 is a root node 
    level = m; 
    while (level > 0 && cord[level - 1] == 0)
    {
        //printf("level = %d\n", level);
        level--;
        //printf("level-- = %d\n", level);
    }
    group_size = n; 
    for (i = 0; i < level; i++)
        group_size /= dims[i];//at group_size=n=10, this will not divide evenly with dims[1], which would be 4 since we have a 2x4 grid  
    int check_last_val = (level == 0 || cord[level - 1] == dims[level - 1] - 1) ? 1 : 0; 

    int single_proc_count = 0; 
    for (i = 0; i < m; i++) if (dims[i] <= 1) single_proc_count++;

    // Create A 
    if (level == 0) {
        // root node 
        A = A_init(n);
        // check time here
    } else {
        buffer_size = group_size * (dims[level - 1] - cord[level - 1]); 
        A = malloc(buffer_size * sizeof(int));
        // sub nodes get data 
        MPI_Recv(A, buffer_size, MPI_SHORT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
    }
    // pass subnode behind if this node is not the last in the row
    int dest; 
    if (!check_last_val) {
        MPI_Cart_shift(comm_cart, level - 1, 1, &tmp, &dest); 
        MPI_Isend(A + group_size, buffer_size - group_size, MPI_SHORT, dest, 0, MPI_COMM_WORLD, &request);
        printf("Processor %d sending data to processor %d at %f\n", world_rank, dest, MPI_Wtime());
    }
    // send subarray in all directions 
    int kept_data = group_size, unsent_data; 
    for (i = level; i < m; i++) {
        // Send on i'th dimension (if dimension is not declined) 
        if (dims[i] > 1) {
            unsent_data = kept_data; 
            kept_data = kept_data / dims[i]; 
            MPI_Cart_shift(comm_cart, i, 1, &tmp, &dest);
            MPI_Isend(A + kept_data, unsent_data - kept_data, MPI_SHORT, dest, 0, MPI_COMM_WORLD, &request);
            printf("Processor %d sending data to processor %d at %f\n", world_rank, dest, MPI_Wtime());
        }
    }
    // compute local sum 
    for(i = 0,total = 0; i < size; i++)
        total += A[i];
    // receive result from other nodes 
    int no_of_requests = m - level + (1 - check_last_val) - single_proc_count; 
    //printf("m= %d, level = %d, check_last_val= %d, single_proc_count = %d\n",m,level,check_last_val, single_proc_count);
    MPI_Request requests[no_of_requests]; 
    //printf("no_of_req= %d\n",no_of_requests);
    int buffer[no_of_requests];
    for (i = 0; i < no_of_requests; i++) {

        MPI_Irecv(buffer + i, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, requests + i);
        printf("*Process %d receives data at %f\n",world_rank, MPI_Wtime());
    }
    MPI_Waitall(no_of_requests, requests, MPI_STATUSES_IGNORE); 
    for(i = 0; i < no_of_requests; i++){
        total += buffer[i];
    }
    printf("I am process %d and my sum is %d at %f\n",world_rank,total, MPI_Wtime());
    // send result to next node 
    if (level != 0) {
        MPI_Cart_shift(comm_cart, level - 1, 1, &dest, &tmp); 
        MPI_Send(&total, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
        printf("**Process %d sends data to process %d at %f\n",world_rank,dest, MPI_Wtime());
    }
    MPI_Barrier(MPI_COMM_WORLD);
    // print total 
    if (level == 0) {
        // check time here
        print_coords(cord, m); 
        printf(" The total sum is %d\n", total);
    }
    // Finalize the MPI environment. 
    MPI_Finalize();
    free(A);
    return 0;
}
