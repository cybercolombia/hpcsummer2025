#include <stdio.h>
#include "mpi.h"
#include <unistd.h>
#include <limits.h>

int main(int argc, char **argv)
{
  MPI_Init(&argc, &argv); // alt.: NULL,NULL
  
  int size, rank;
  // Copy the communicator
  MPI_Comm my_world = MPI_COMM_WORLD;

  MPI_Comm_size(my_world, &size);
  MPI_Comm_rank(my_world, &rank);

  char hostname[HOST_NAME_MAX + 1];
  int result = gethostname(hostname, sizeof(hostname));
    
    if (result!= 0 ){
      printf("Hostname: %s\n", hostname);
    }    
  printf("I am rank %i out of %i on host %s \n", rank, size,hostname);

  MPI_Finalize();

  return 0;
}
