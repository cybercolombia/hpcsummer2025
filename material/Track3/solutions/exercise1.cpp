#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int local_value = world_rank + 1;  // Valor local, por ejemplo 1, 2, 3, ...
    int total_sum = 0;

    if (world_rank == 0) {
        total_sum = local_value; // Suma su propio valor

        for (int i = 1; i < world_size; ++i) {
            int received;
            MPI_Recv(&received, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_sum += received;
        }

        printf("Total sum computed at root (rank 0): %d\n", total_sum);
    } else {
        // Otros procesos envían su valor a rank 0
        MPI_Send(&local_value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
