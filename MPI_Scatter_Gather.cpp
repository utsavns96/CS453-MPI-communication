#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

// #define size 10
// Computes the sum of an array of numbers
int addition_func(int *a, int num) {
  int sum = 0;
  int i;
  for (i = 0; i < num; i++) {
    sum += a[i];
  }
    printf("Sum of all elements is %d\n", sum);
  return sum ;
}


// Creates an array of random numbers. Each number has a value from 0 - 1
int *rand_num_gen(int n) {
  int *nums_arr = (int *)malloc(sizeof(int) * n);
  int i;
  for (i = 0; i < n; i++) {
    nums_arr[i] = (rand() / (int)RAND_MAX);
  }
  return nums_arr;
}
int main(int argc, char** argv) {
  int num_elements_per_proc = atoi(argv[1]);
  // int num_elements_per_proc = 20;
  MPI_Init(NULL, NULL);

  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // int num_elements_per_proc = size / world_size ; 

  int *random_num = NULL;
  if (world_rank == 0) {
    random_num = rand_num_gen(num_elements_per_proc * world_size);
  }

  // subset of the entire array for each process
  int *sub_rand_nums = (int *)malloc(sizeof(int) * num_elements_per_proc);

  MPI_Scatter(random_num, num_elements_per_proc, MPI_INT, sub_rand_nums,
              num_elements_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

  // Computing the addition 
  int sub_add = addition_func(sub_rand_nums, num_elements_per_proc);

  // Gather all partial sums down to the root process
  int *sub_adds = NULL;
  if (world_rank == 0) {
    sub_adds = (int *)malloc(sizeof(int) * world_size);
  }
  MPI_Gather(&sub_add, 1, MPI_INT, sub_adds, 1, MPI_INT, 0, MPI_COMM_WORLD);

  if (world_rank == 0) {
    int sum = addition_func(sub_adds, world_size);
    printf("Sum of all elements is %d\n", sum);
    // Compute the sum across the original data for comparison
    int original_data_sum =
      addition_func(random_num, num_elements_per_proc * world_size);
    printf("Sum computed across original data is %d\n", original_data_sum);
  }

  // Clean up
  if (world_rank == 0) {
    free(sub_adds);
  }
  free(sub_rand_nums);
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
}
