#include <stdio.h>
#include "mpi.h"


int main(int argc, char **argv)
{
  MPI_Init(&argc, &argv); // alt.: NULL,NULL
  printf("MPI environment initialized.\n");
  int size, rank;

  // Copy the communicator

  int world_rank;
  int world_size;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  printf("Number of rannks: %d\n",   world_size);
    if (world_rank == 0) {
        int data = 100;
        MPI_Send(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("Process 0 sent data %d to process 1\n", data);
    } else if (world_rank == 1) {
        int data;
        MPI_Recv(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1 received data %d from process 0\n", data);
    }
  MPI_Finalize();
  printf("MPI environment finalized.\n");

  return 0;
}
