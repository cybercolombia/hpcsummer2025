/*
 * Copyright (c) 2014 Xin Zhao. All rights reserved.
 *
 * Author(s): Xin Zhao <xinzhao3@illinois.edu>
 *
 */

#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <time.h>

#define RAND_RANGE (10)

void setup(int rank, int nprocs, int argc, char **argv,
           int *mat_dim_ptr, int *blk_dim_ptr, int *px_ptr, int *py_ptr,
           int *final_flag);
void init_mats(int mat_dim, double *win_mem,
               double **mat_a_ptr, double **mat_b_ptr, double **mat_c_ptr);
void dgemm(double *local_a, double *local_b, double *local_c, int blk_dim);
void print_mat(double *mat, int mat_dim);
void check_mats(double *mat_a, double *mat_b, double *mat_c, int mat_dim);

int main(int argc, char **argv)
{
    int rank, nprocs;
    int mat_dim, blk_dim, blk_num;
    int px, py, bx, by, rx, ry;
    int final_flag;
    double *mat_a, *mat_b, *mat_c;
    double *local_a, *local_b, *local_c;
    MPI_Aint disp_a, disp_b, disp_c;
    MPI_Aint offset_a, offset_b, offset_c;
    int i, j, k;
    int global_i, global_j;

    double *win_mem;
    MPI_Win win;

    double t1, t2;

    MPI_Datatype blk_dtp;

    /* initialize MPI environment */
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    /* argument checking and setting */
    setup(rank, nprocs, argc, argv, &mat_dim, &blk_dim,
          &px, &py, &final_flag);
    if (final_flag == 1) {
        MPI_Finalize();
        exit(0);
    }

    /* number of blocks in one dimension */
    blk_num = mat_dim / blk_dim;

    /* determine my coordinates (x,y) -- r=x*a+y in the 2d processor array */
    rx = rank % px;
    ry = rank / px;

    /* determine distribution of work */
    bx = blk_num / px;
    by = blk_num / py;

    if (!rank) {
        /* create RMA window */
        MPI_Win_allocate(3*mat_dim*mat_dim*sizeof(double), sizeof(double),
                         MPI_INFO_NULL, MPI_COMM_WORLD, &win_mem, &win);

        /* initialize matrices */
        init_mats(mat_dim, win_mem, &mat_a, &mat_b, &mat_c);
    }
    else {
        MPI_Win_allocate(0, sizeof(double), MPI_INFO_NULL, MPI_COMM_WORLD,
                         &win_mem, &win);
    }

    /* allocate local buffer */
    MPI_Alloc_mem(3*blk_dim*blk_dim*sizeof(double), MPI_INFO_NULL, &local_a);
    local_b = local_a + blk_dim * blk_dim;
    local_c = local_b + blk_dim * blk_dim;

    /* create block datatype */
    MPI_Type_vector(blk_dim, blk_dim, mat_dim, MPI_DOUBLE, &blk_dtp);
    MPI_Type_commit(&blk_dtp);

    disp_a = 0;
    disp_b = disp_a + mat_dim * mat_dim;
    disp_c = disp_b + mat_dim * mat_dim;

    MPI_Barrier(MPI_COMM_WORLD);

    t1 = MPI_Wtime();

    MPI_Win_lock(MPI_LOCK_SHARED, 0, 0, win);

    for (i = 0; i < by; i++) {
        for (j = 0; j < bx; j++) {

            global_i = i + by * ry;
            global_j = j + bx * rx;

            /* get block from mat_a */
            offset_a = global_i * blk_dim * mat_dim + global_j * blk_dim;
            MPI_Get(local_a, blk_dim*blk_dim, MPI_DOUBLE,
                    0, disp_a+offset_a, 1, blk_dtp, win);

            MPI_Win_flush(0, win);

            for (k = 0; k < blk_num; k++) {

                /* get block from mat_b */
                offset_b = global_j * blk_dim * mat_dim + k * blk_dim;
                MPI_Get(local_b, blk_dim*blk_dim, MPI_DOUBLE,
                        0, disp_b+offset_b, 1, blk_dtp, win);

                MPI_Win_flush(0, win);

                /* local computation */
                dgemm(local_a, local_b, local_c, blk_dim);

                /* accumulate block to mat_c */
                offset_c = global_i * blk_dim * mat_dim + k * blk_dim;
                MPI_Accumulate(local_c, blk_dim*blk_dim, MPI_DOUBLE,
                               0, disp_c+offset_c, 1, blk_dtp, MPI_SUM, win);

                MPI_Win_flush(0, win);
            }
        }
    }

    MPI_Win_unlock(0, win);

    t2 = MPI_Wtime();

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        check_mats(mat_a, mat_b, mat_c, mat_dim);

        // print_mat(mat_a, mat_dim);
        // print_mat(mat_b, mat_dim);
        // print_mat(mat_c, mat_dim);

        printf("[%i] time: %f\n", rank, t2-t1);
    }

    MPI_Type_free(&blk_dtp);
    MPI_Free_mem(local_a);
    MPI_Win_free(&win);
    MPI_Finalize();
}

void setup(int rank, int nprocs, int argc, char **argv,
           int *mat_dim_ptr, int *blk_dim_ptr, int *px_ptr, int *py_ptr,
           int *final_flag)
{
    int mat_dim, blk_dim, px, py;

    (*final_flag) = 0;

    if (argc < 5) {
        if (!rank) printf("usage: ga_mpi <m> <b> <px> <py>\n");
        (*final_flag) = 1;
        return;
    }

    mat_dim = atoi(argv[1]);    /* matrix dimension */
    blk_dim = atoi(argv[2]);    /* block dimension */
    px = atoi(argv[3]);         /* 1st dim processes */
    py = atoi(argv[4]);         /* 2st dim processes */

    if (px * py != nprocs)
        MPI_Abort(MPI_COMM_WORLD, 1);
    if (mat_dim % blk_dim != 0)
        MPI_Abort(MPI_COMM_WORLD, 1);
    if ((mat_dim / blk_dim) % px != 0)
        MPI_Abort(MPI_COMM_WORLD, 1);
    if ((mat_dim / blk_dim) % py != 0)
        MPI_Abort(MPI_COMM_WORLD, 1);

    (*mat_dim_ptr) = mat_dim;
    (*blk_dim_ptr) = blk_dim;
    (*px_ptr) = px;
    (*py_ptr) = py;
}

void init_mats(int mat_dim, double *win_mem,
               double **mat_a_ptr, double **mat_b_ptr, double **mat_c_ptr)
{
    int i, j;
    double *mat_a, *mat_b, *mat_c;

    srand(time(NULL));

    mat_a = win_mem;
    mat_b = mat_a + mat_dim * mat_dim;
    mat_c = mat_b + mat_dim * mat_dim;

    for (j = 0; j < mat_dim; j++) {
        for (i = 0; i < mat_dim; i++) {
            mat_a[j+i*mat_dim] = (double) rand() / (RAND_MAX / RAND_RANGE + 1);
            mat_b[j+i*mat_dim] = (double) rand() / (RAND_MAX / RAND_RANGE + 1);
            mat_c[j+i*mat_dim] = (double) 0.0;
        }
    }

    (*mat_a_ptr) = mat_a;
    (*mat_b_ptr) = mat_b;
    (*mat_c_ptr) = mat_c;
}

void dgemm(double *local_a, double *local_b, double *local_c, int blk_dim)
{
    int i, j, k;

    memset(local_c, 0, blk_dim*blk_dim*sizeof(double));

    for (j = 0; j < blk_dim; j++) {
        for (i = 0; i < blk_dim; i++) {
            for (k = 0; k < blk_dim; k++)
                local_c[j+i*blk_dim] += local_a[k+i*blk_dim] * local_b[j+k*blk_dim];
        }
    }
}

void print_mat(double *mat, int mat_dim)
{
    int i, j;
    for (i = 0; i < mat_dim; i++) {
        for (j = 0; j < mat_dim; j++) {
            printf("%.4f ", mat[j+i*mat_dim]);
        }
        printf("\n");
    }
    printf("\n");
}

void check_mats(double *mat_a, double *mat_b, double *mat_c, int mat_dim)
{
    int i, j, k;
    int bogus = 0;
    double temp_c;
    double diff, max_diff = 0.0;

    for (j = 0; j < mat_dim; j++) {
        for (i = 0; i < mat_dim; i++) {
            temp_c = 0.0;
            for (k = 0; k < mat_dim; k++)
                temp_c += mat_a[k+i*mat_dim] * mat_b[j+k*mat_dim];
            diff = mat_c[j+i*mat_dim] - temp_c;
            if (fabs(diff) > 0.00001) {
                bogus = 1;
                if (fabs(diff) > fabs(max_diff))
                    max_diff = diff;
            }
        }
    }

    if (bogus)
        printf("\nTEST FAILED: (%.5f MAX diff)\n\n", max_diff);
    else
        printf("\nTEST PASSED\n\n");
}
