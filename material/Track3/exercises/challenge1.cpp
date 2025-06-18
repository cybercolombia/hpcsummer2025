#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_size, world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    const int N = 1000000; // tamaño total del arreglo
    int* data = NULL;
    int chunk_size = N / world_size;

    int* local_data = (int*)malloc(sizeof(int) * chunk_size);

    // El proceso 0 inicializa el arreglo
    if (world_rank == 0) {
        data = (int*)malloc(sizeof(int) * N);
        for (int i = 0; i < N; ++i) {
            data[i] = 1;  // Cada elemento vale 1
        }
    }

    // Distribuye los datos a todos los procesos
    MPI_Scatter(data, chunk_size, MPI_INT, local_data, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);
    // Sincronizar todos antes de cronometrar
    MPI_Barrier(MPI_COMM_WORLD);
    double start_time = MPI_Wtime();

    // Cada proceso calcula su suma parcial con OpenMP
    int local_sum = 0;

    #pragma omp parallel for reduction(+:local_sum)
    for (int i = 0; i < chunk_size; ++i) {
        local_sum += local_data[i];
    }

    // Recolectar las sumas parciales en el proceso 0
    int global_sum = 0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    
    // Sincronizar y medir tiempo
    MPI_Barrier(MPI_COMM_WORLD);
    double end_time = MPI_Wtime();

    if (world_rank == 0) {
        printf("Total sum: %d\n", global_sum);
        printf("Total time for distributed sum: %.6f seconds\n", end_time - start_time);
    }

    free(local_data);
    if (world_rank == 0) free(data);

    MPI_Finalize();
    return 0;
}
