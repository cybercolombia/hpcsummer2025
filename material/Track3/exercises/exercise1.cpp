#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    const int N = 100;  // Tamaño total del arreglo
    int *data = NULL;

    // Solo el proceso 0 inicializa el arreglo
    if (world_rank == 0) {
        data = (int*) malloc(N * sizeof(int));
        for (int i = 0; i < N; i++) {
            data[i] = 1;  // valor simple para facilitar validación (suma esperada: 100)
        }
    }

    // Calculamos cuántos elementos recibe cada proceso
    int elements_per_proc = N / world_size;
    int *sub_data = (int*)malloc(elements_per_proc * sizeof(int));

    // Distribuir los datos a todos los procesos (bloqueante)
    MPI_Scatter(data, elements_per_proc, MPI_INT,
                sub_data, elements_per_proc, MPI_INT,
                0, MPI_COMM_WORLD);

    // Cada proceso calcula su suma parcial
    int partial_sum = 0;
    for (int i = 0; i < elements_per_proc; i++) {
        partial_sum += sub_data[i];
    }

    // Recolectamos los resultados en el proceso 0
    int total_sum = 0;
    MPI_Reduce(&partial_sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (world_rank == 0) {
        printf("Suma total = %d\n", total_sum);  // Debería ser 100 si todos los valores son 1
        free(data);
    }

    free(sub_data);
    MPI_Finalize();
    return 0;
}
