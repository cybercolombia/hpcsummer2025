#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>

#define MAX_MSG_SIZE (1 << 20) // hasta 1 MB
#define N_ITER 1000

void benchmark_send(int rank, int size) {
    for (int msg_size = 1; msg_size <= MAX_MSG_SIZE; msg_size *= 2) {
        char *buffer = (char*)malloc(msg_size);
        memset(buffer, 0, msg_size);

        MPI_Barrier(MPI_COMM_WORLD);  // sincronización

        double t_start = MPI_Wtime();
        for (int i = 0; i < N_ITER; i++) {
            if (rank == 0) {
                MPI_Send(buffer, msg_size, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
            } else if (rank == 1) {
                MPI_Recv(buffer, msg_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
        double t_end = MPI_Wtime();
        if (rank == 0) {
            printf("[MPI_Send] size: %d bytes, avg latency: %f us\n",
                   msg_size, 1e6 * (t_end - t_start) / N_ITER);
        }

        free(buffer);
    }
}

void benchmark_bsend(int rank, int size) {
    for (int msg_size = 1; msg_size <= MAX_MSG_SIZE; msg_size *= 2) {
        char *buffer = (char*)malloc(msg_size);
        memset(buffer, 0, msg_size);

        int pack_size;
        MPI_Pack_size(msg_size, MPI_CHAR, MPI_COMM_WORLD, &pack_size);
        int bsend_bufsize = N_ITER * (pack_size + MPI_BSEND_OVERHEAD);
        void *bsend_buffer = malloc(bsend_bufsize);

        MPI_Buffer_attach(bsend_buffer, bsend_bufsize);

        MPI_Barrier(MPI_COMM_WORLD);  // sincronización

        double t_start = MPI_Wtime();
        for (int i = 0; i < N_ITER; i++) {
            if (rank == 0) {
                MPI_Bsend(buffer, msg_size, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
            } else if (rank == 1) {
                MPI_Recv(buffer, msg_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
        double t_end = MPI_Wtime();
        if (rank == 0) {
            printf("[MPI_Bsend] size: %d bytes, avg latency: %f us\n",
                   msg_size, 1e6 * (t_end - t_start) / N_ITER);
        }

        void *detach_buf;
        int detach_size;
        MPI_Buffer_detach(&detach_buf, &detach_size);
        free(detach_buf);
        free(buffer);
    }
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    if (world_size != 2) {
        if (world_rank == 0) {
            fprintf(stderr, "This benchmark needs exactly 2 processes.\n");
        }
        MPI_Finalize();
        return 1;
    }

    if (world_rank == 0) {
        printf("Running benchmark on 2 MPI ranks...\n");
    }

    benchmark_send(world_rank, world_size);
    benchmark_bsend(world_rank, world_size);

    MPI_Finalize();
    return 0;
}
