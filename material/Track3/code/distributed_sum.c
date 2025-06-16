#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Define the initial dataset size
    const int dataset_size = 1000;
    int *dataset = NULL;

    // Allocate and initialize the dataset on the root process
    if (world_rank == 0) {
        dataset = (int*) malloc(sizeof(int) * dataset_size);
        printf("The initial size is %d\n",dataset_size);
        for (int i = 0; i < dataset_size; i++) {
            dataset[i] = 1; // Example data, all elements set to 1
        }
    }

    // Calculate the size of the subset for each rank
    int subset_size = dataset_size / world_size;
    int *subset = (int*) malloc(sizeof(int) * subset_size);

    // Distribute the dataset to all processes
    MPI_Scatter(dataset, subset_size, MPI_INT, subset, subset_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Each rank computes its partial sum
    int partial_sum = 0;
    for (int i = 0; i < subset_size; i++) {
        partial_sum += subset[i];
    }

    // Use MPI_Reduce to gather all partial sums and compute the total sum on the root rank
    int total_sum = 0;
    MPI_Reduce(&partial_sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Root process prints the result
    if (world_rank == 0) {
        printf("Partial sum %d on Rank: %d\n", total_sum,world_rank);
        printf("Total sum: %d\n", total_sum);
    }
    else {

         printf("Running on rank%d\n", world_rank);
         printf("Partial Sum is %d\n", partial_sum);
        printf("Total Sum %d on rank %d\n", total_sum,world_rank);  
    }

    // Clean up
    if (world_rank == 0) {
        free(dataset);
    }
    free(subset);

    MPI_Finalize();
    return 0;
}
