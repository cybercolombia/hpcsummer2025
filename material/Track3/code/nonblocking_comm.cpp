#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    int world_rank;
    int world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    printf("Running on node: %s\n", processor_name);
    printf("Number of ranks: %d\n", world_size);

    if (world_rank == 0) {
        int data = 100;
        // Start the request and Start Send with Wait 
        MPI_Request request;
        MPI_Isend(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);  // Non-blocking send
        MPI_Wait(&request, MPI_STATUS_IGNORE);  // Wait for completion
        printf("Process 0 sent data %d to process 1\n", data);
    } else if (world_rank == 1) {
        int data;
        //Start the receive with wait 
        MPI_Request request;
        MPI_Irecv(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);  // Non-blocking receive
        MPI_Wait(&request, MPI_STATUS_IGNORE);  // Wait for completion
        printf("Process 1 received data %d from process 0\n", data);
    }

    MPI_Finalize();
    printf("MPI environment finalized.\n");

    return 0;
}
