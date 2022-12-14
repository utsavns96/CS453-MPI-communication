#include <mpi.h>
#include <stdio.h>
#include <unistd.h>
#include <cstdlib>
//#define size 8
//int A[size] = { 1,2,3,4,5,6,7,8 };

int main(int argc, char** argv)
{
    printf("Question 3 - calculating sum using Scatter and Reduce\n");
    int world_size, rank;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    double start, end;
    int size = atoi(argv[2]);

    int A[size];
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    // get the total number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // get the rank of current process
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;
    int count = size / (world_size);
    int total = 0;
    int rbuf[count];    
    if (rank == 0)
    {
        start = MPI_Wtime();
        //A = A_init(size);
        for (int i = 0; i < size; i++)
        {
            //A[i] = rand() % 3 - 1;
            A[i] = i;
        }
    }
    MPI_Scatter(&A, count, MPI_INT, rbuf, count, MPI_INT, 0, MPI_COMM_WORLD);
    printf("Scattering array\n");
    printf("I am processor %d. I have elements:\n", rank);
    for (int i=0;i<count;i++)
    {
        printf("%d-",rbuf[i]);
    }
    printf("\n");
    int localsum=0;
    for(int i=0;i<count;i++)
    {
        localsum+=rbuf[i];
    }
    printf("I am processor %d. My sum is = %d\n",rank,localsum);
    int sum = 0;
    MPI_Reduce(&localsum, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    //if (rank == 0)
    //{
    //  printf("The sum of the array is %d.\n", sum);
    //}
    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();
    MPI_Finalize();
    if (rank == 0)
    {
        printf("\n\nThe sum of all values in the array is = %d\n", sum);
        printf("The program started at %f, ended at %f, which means it ran 		for %f\n", start, end, end - start);
    }
    return 0;
}
