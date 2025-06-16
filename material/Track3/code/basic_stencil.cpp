#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>


#define ITERATIONS 100  // Number of iterations

#define N 100 // Initial Grid Size

// Function to initialize the grid
void initialize_grid(double grid[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            grid[i][j] = rand() % 100;
        }
    }
}

// Function to perform the stencil computation
void stencil_step(double grid[N][N], double new_grid[N][N], int start_row, int end_row) {
    for (int i = start_row; i < end_row; i++) {
        for (int j = 1; j < N-1; j++) {
            new_grid[i][j] = 0.25 * (grid[i-1][j] + grid[i+1][j] + grid[i][j-1] + grid[i][j+1]);
        }
    }
}

// Function to copy the new grid to the old grid
void copy_grid(double dest[N][N], double src[N][N], int start_row, int end_row) {
    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < N; j++) {
            dest[i][j] = src[i][j];
        }				 
    }				 
}

void print_grid(double grid[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%.2f ", grid[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}


void write_grid_to_file(const char *filename, double grid[N][N]) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file for writing\n");
        return;
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fprintf(file, "%.2f ", grid[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

int main(int argc, char *argv[]) {
	  
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (N % size != 0) {
        if (rank == 0) {
            fprintf(stderr, "The grid size N must be divisible by the number of processes.\n");
        }
        MPI_Finalize();
        return -1;
    }

    int rows_per_process = N / size;
    int start_row = rank * rows_per_process;
    int end_row = start_row + rows_per_process;

    double grid[N][N], new_grid[N][N];
    double local_grid[rows_per_process][N], local_new_grid[rows_per_process][N];

    if (rank == 0) {
        initialize_grid(grid);
								  
        write_grid_to_file("initial_grid.txt", grid);
    }

    // Scatter the initial grid to all processes
    MPI_Scatter(grid, rows_per_process * N, MPI_DOUBLE, local_grid, rows_per_process * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (int iter = 0; iter < ITERATIONS; iter++) {
        stencil_step(local_grid, local_new_grid, 1, rows_per_process-1);

        // Copy new local grid to old local grid
        copy_grid(local_grid, local_new_grid, 1, rows_per_process-1);

        // Exchange boundary rows between neighboring processes
        if (rank > 0) {
            MPI_Send(local_grid[1], N, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD);
            MPI_Recv(local_grid[0], N, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        if (rank < size-1) {
            MPI_Send(local_grid[rows_per_process-2], N, MPI_DOUBLE, rank+1, 0, MPI_COMM_WORLD);
            MPI_Recv(local_grid[rows_per_process-1], N, MPI_DOUBLE, rank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }

    // Gather the final grid from all processes
							
    MPI_Gather(local_grid, rows_per_process * N, MPI_DOUBLE, grid, rows_per_process * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
								
        write_grid_to_file("final_grid.txt", grid);
    }


    MPI_Finalize();
    return 0;
}
