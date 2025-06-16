#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int data;
    MPI_Request request;
    MPI_Status status;

    if (world_rank == 0) {
        data = 123;
        MPI_Isend(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
        printf("Process 0 initiated non-blocking send of data %d\n", data);
        // Perform some work while the send operation completes
        printf("Process 0 is doing other work while waiting for send to complete\n");
        MPI_Wait(&request, &status);  // Ensure the send operation is complete
    } else if (world_rank == 1) {
        MPI_Irecv(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
        printf("Process 1 initiated non-blocking receive\n");
        // Perform some work while the receive operation completes
        printf("Process 1 is doing other work while waiting for receive to complete\n");
        MPI_Wait(&request, &status);  // Ensure the receive operation is complete
        printf("Process 1 received data %d\n", data);
    }

    MPI_Finalize();
    return 0;
}