#include <mpi.h>
#include <stdio.h>
#include <unistd.h>
#include <cstdlib>

int main(int argc, char *argv[])
{
  int size = atoi(argv[2]);
  int A[size];
  int world_size, rank;
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int name_len;
  // Initialize the MPI environment
  MPI_Init(&argc,&argv);
  // get the total number of processes
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  int count = size / (world_size);
  int PartA[count];
  // get the rank of current process
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Status status;
  double start, end;
  int total = 0;
  if (rank == 0)
  {
	  for (int i = 0; i < size; i++)
	  {
		  A[i] = rand() % 3 - 1;
	  }
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if(rank==0)
  {
	  start = MPI_Wtime();
	int dest;
	if (world_size>1)
	{
		for(dest=1;dest<world_size-1;dest++)
		{
			MPI_Send(&A[dest*count],count,MPI_INT,dest,0,MPI_COMM_WORLD);
			printf("Processor 0 sends partial array to %d\n", dest);
		}
		int leftover = size-(dest*count);
		MPI_Send(&A[dest*count],leftover,MPI_INT,dest,0,MPI_COMM_WORLD);
		printf("Processor 0 is left with elements:\n");
		for(int i=0;i<count;i++)
		{
			printf("%d - ",A[i]);
		}
		printf("\n");
	}
	for(int i=0; i<count; i++)
	{
		total +=A[i];
	}
	printf("I am processor %d, and my sum is %d\n", rank, total);
	
	int temp = 0;
	for(int i=1; i<world_size; i++)
	{
		temp =0;
		MPI_Recv(&temp,1,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,&status);
		int sender = status.MPI_SOURCE;
		printf("Received partial sum = %d from processor %d\n", temp, sender);
		total += temp;
	}
  }
  else
  {
	MPI_Recv(&PartA,count,MPI_INT,0,0,MPI_COMM_WORLD,&status);
	printf("Processor %d received partial array from 0\n", rank);
	for(int i=0;i<count;i++)
	{
		printf("%d-",PartA[i]);
	}
	printf("\n");	
	int my_sum = 0;
	for(int i=0;i<count;i++)
	{
		my_sum += PartA[i];
	}
	printf("I am processor %d, and my sum is %d\n", rank, my_sum);
	MPI_Send(&my_sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  } 
  MPI_Barrier(MPI_COMM_WORLD);
  end = MPI_Wtime();
  MPI_Finalize();
  if (rank == 0)
  {
	  printf("\n\nThe sum of all values in the array is = %d\n", total);
	  printf("The program started at %d, ended at %d, which means it ran for %d", start, end, end - start);
  }
  return 0;
}
