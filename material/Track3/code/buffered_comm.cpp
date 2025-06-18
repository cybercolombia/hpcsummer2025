#include <stdio.h>
#include <cstdlib>
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
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int name_len;
  MPI_Get_processor_name(processor_name, &name_len);
  printf("Running on node: %s (rank %d of %d)\n", processor_name, world_rank, world_size);
  // Declare the times 
        double start_i, end_i;
           
    if (world_size < 2) {
        if (world_rank == 0) {
            fprintf(stderr, "This example needs at least two processes.\n");
        }
        MPI_Finalize();
        return 1;
    }

    if (world_rank == 0) {
        int data = 42;

        // Calculate size needed for the buffer
        int bufsize;
        MPI_Pack_size(1, MPI_INT, MPI_COMM_WORLD, &bufsize);
        // Calculate the Buffer Size. 
        // MPI_BSEND_OVERHEAD represents the size, in bytes, of the memory overhead generated 
        // everytime an MPI_Bsend or MPI_Ibsend is issued.
        bufsize += MPI_BSEND_OVERHEAD;

        // Allocate and attach buffer
        void* buffer = malloc(bufsize);
        if (buffer == NULL) {
            fprintf(stderr, "Could not allocate buffer\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        // Setuo the buffer 
        MPI_Buffer_attach(buffer, bufsize);
         
        // Send data using buffered send
        double start = MPI_Wtime();
        MPI_Bsend(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        double end = MPI_Wtime();
        printf("Time taken: %f seconds\n", end - start);
        printf("Process 0 sent buffered data %d to process 1\n", data);

        // Detach buffer (waits for completion of internal copies)
        void* detached_buffer;
        int detached_size;
        MPI_Buffer_detach(&detached_buffer, &detached_size);
        free(detached_buffer);
    }

    else if (world_rank == 1) {
        int received;
        start_i = MPI_Wtime();
        MPI_Recv(&received, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        end_i = MPI_Wtime();
        printf("Time taken: %f seconds\n", end_i - start_i);
        printf("Process 1 received data %d from process 0\n", received);
    }

    MPI_Finalize();
    return 0;
}