#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int local_value = //TODO // Each process has a unique value
    int total_sum = 0;

    if (world_rank == 0) {
        int* recv_buf = (int*)malloc(sizeof(int) * world_size);
        
        MPI_Request* requests = (MPI_Request*)malloc(sizeof(MPI_Request) * world_size);

        // Rank 0 also adds its own value
        recv_buf[0] = local_value;

        // Post non-blocking receives from all other processes
        for (int i = 1; i < //TODO//; ++i) {
            MPI_Irecv(&recv_buf[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, &requests[i]);
        }

        // Fill the first request slot with a completed request (own data)
        MPI_Request self_req;
        requests[0] = MPI_REQUEST_NULL;

        // Wait for all non-blocking receives
        MPI_Waitall(world_size, //TODO//, MPI_STATUSES_IGNORE);

        // Sum all values
        for (int i = 0; i < world_size; ++i) {
            total_sum += // TODO
        }
        printf("Total sum computed at root: %d\n", total_sum);

        free(recv_buf);
        free(requests);
    } else {
        MPI_Request send_req;
        MPI_Isend(&local_value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &send_req);
        MPI_Wait(&send_req, MPI_STATUS_IGNORE);
    }

    MPI_Finalize();
    return 0;
}
