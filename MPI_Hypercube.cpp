#include <mpi.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <iostream>

int main(int argc, char* argv[])
{
    int world_size, rank, dest, source, index=0;
    int sum;
    double start, end;
    int* A;
    int my_sum;
    int recv_sum;
    int n = atoi(argv[1]);
    int dim = log2(n);
    int size = atoi(argv[2]);
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);
    // get the total number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    // get the rank of current process
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;
    MPI_Request request;
    //creating array
    if (rank == 0)
    {
        printf("\n\nn = %d, size = %d\n",n,size);
        start = MPI_Wtime();
        A = new int[size];
        for (int i = 1; i <= size; i++)
        {
            A[i]= 1 - rand() % 3;
        }
    }
    //printf("I am processor = %d\n", rank);
    int total = 0,s;
    //distributing our array
    int mask = (int)pow(2, dim) - 1;
    int my_id = rank;
    for (int i = dim - 1; i >= 0; i--)
    {
        int power = (int)pow(2, i);
        mask = mask ^ power;
        int iteration = dim - i;
        int count = n / pow(2, iteration);
        if ((my_id & mask) == 0)
        {
            if ((my_id & power) == 0)
            {
                dest = (my_id ^ power);
                MPI_Isend(A+index,count,MPI_INT,dest,0,MPI_COMM_WORLD,&request);
                index += count;
                s = count;
            }
            else
            {
                source = my_id ^ power;
                A = new int[count];
                MPI_Recv(A, count, MPI_INT, source, 0, MPI_COMM_WORLD, &status);
                s = count;
            }
        }
    }
    //accumulating our partial sums
    mask = 0;
    for (int j = 0; j < s; j++)
    {
        my_sum += A[j];
    }
    for (int i = 0; i < dim; i++)
    {
        int power = (int)pow(2, i);
        if ((my_id & mask) == 0)
        {
            if ((my_id & power) != 0)
            {
                dest = my_id ^ power;
                MPI_Isend(&my_sum,1,MPI_INT,dest,0,MPI_COMM_WORLD,&request);
            }
            else
            {
                source = my_id ^ power;
                MPI_Recv(&recv_sum, 1, MPI_INT, source, 0, MPI_COMM_WORLD, &status);
                my_sum += recv_sum;
            }
            mask = mask ^ power;
        }
    }
    if (rank == 0)
    {
        printf("\n\nThe sum of all values in the array is = %d\n", sum);
        printf("\n");
    }
    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();
    MPI_Finalize();
    if (rank == 0)
    {
        printf("\n\nThe program ran for = %f\n", end - start);
    }
    return 0;
}
