/*
 * Copyright (c) 2012 Torsten Hoefler. All rights reserved.
 *
 * Author(s): Torsten Hoefler <htor@illinois.edu>
 *
 */

#include "stencil_par.h"
#include <omp.h>

#define THX_START (thread_id % nthreads)==0 ? 1 : (thread_id % nthreads) * Thx + 1
#define THX_END (thread_id % nthreads) == nthreads -1 ? bx + 1 : ((thread_id + 1) % nthreads) * Thx + 1

void setup(int rank, int proc, int argc, char **argv,
           int *n_ptr, int *energy_ptr, int *niters_ptr, int *px_ptr, int *py_ptr,
           int *final_flag);

void init_sources(int bx, int by, int offx, int offy, int n,
                  const int nsources, int sources[][2],
                  int *locnsources_ptr, int locsources[][2]);

int main(int argc, char **argv)
{
    int rank, size, provided;
    int n, energy, niters, px, py;

    int rx, ry;
    int north, south, west, east;
    int bx, by, offx, offy;

    /* three heat sources */
    const int nsources = 3;
    int sources[nsources][2];
    int locnsources;             /* number of sources in my area */
    int locsources[nsources][2]; /* sources local to my rank */

    double t1, t2;

    int iter, i, j;

    double *aold, *anew, *tmp;

    double heat, rheat;

    int final_flag;

    int nthreads, Thx;

    /* initialize MPI envrionment */
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    if (provided < MPI_THREAD_MULTIPLE)
        MPI_Abort(MPI_COMM_WORLD, 1);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /* argument checking and setting */
    setup(rank, size, argc, argv,
          &n, &energy, &niters, &px, &py, &final_flag);

    if (final_flag == 1) {
        MPI_Finalize();
        exit(0);
    }

    /* determine my coordinates (x,y) -- rank=x*a+y in the 2d processor array */
    rx = rank % px;
    ry = rank / px;

    /* determine my four neighbors */
    north = (ry - 1) * px + rx; if (ry-1 < 0)   north = MPI_PROC_NULL;
    south = (ry + 1) * px + rx; if (ry+1 >= py) south = MPI_PROC_NULL;
    west = ry * px + rx - 1;    if (rx-1 < 0)   west = MPI_PROC_NULL;
    east = ry * px + rx + 1;    if (rx+1 >= px) east = MPI_PROC_NULL;

    /* decompose the domain */
    bx = n / px;    /* block size in x */
    by = n / py;    /* block size in y */
    offx = rx * bx; /* offset in x */
    offy = ry * by; /* offset in y */

    /* divide blocks in x amongst threads */
    nthreads = omp_get_max_threads();
    Thx = bx / nthreads;

    /* printf("nthreads: %d, Thx: %d\n", nthreads, Thx); */

    /* printf("%i (%i,%i) - w: %i, e: %i, n: %i, s: %i\n", rank, ry,rx,west,east,north,south); */

    /* allocate working arrays & communication buffers */
    MPI_Alloc_mem((bx+2)*(by+2)*sizeof(double), MPI_INFO_NULL, &aold); /* 1-wide halo zones! */
    MPI_Alloc_mem((bx+2)*(by+2)*sizeof(double), MPI_INFO_NULL, &anew); /* 1-wide halo zones! */

    /* initialize three heat sources */
    init_sources(bx, by, offx, offy, n,
                 nsources, sources, &locnsources, locsources);

    /* create north-south datatype */
    MPI_Datatype north_south_type;
    MPI_Type_contiguous(Thx, MPI_DOUBLE, &north_south_type);
    MPI_Type_commit(&north_south_type);

    /* create east-west type */
    MPI_Datatype east_west_type;
    MPI_Type_vector(by,1,bx+2,MPI_DOUBLE, &east_west_type);
    MPI_Type_commit(&east_west_type);

    t1 = MPI_Wtime(); /* take time */

    for (iter = 0; iter < niters; ++iter) {

      /* refresh heat sources */
      for (i = 0; i < locnsources; ++i) {
	aold[ind(locsources[i][0],locsources[i][1])] += energy; /* heat source */
      }

      /* reset the total heat */
      heat = 0.0;

      #pragma omp parallel private(i,j) reduction(+:heat)
      {
	int thread_id = omp_get_thread_num();
	int xstart = THX_START;
	int xend = THX_END;

	/* create request arrays */
	MPI_Request north_reqs[2];
	MPI_Request south_reqs[2];
	MPI_Request east_reqs[2];
	MPI_Request west_reqs[2];

        /* exchange data with neighbors */
	if (south >= 0) {
	  MPI_Isend(&aold[ind(xstart,by)] /* south */, 1, north_south_type,
		    south, 9, MPI_COMM_WORLD, &south_reqs[0]);
	  MPI_Irecv(&aold[ind(xstart,by+1)] /* south */, 1, north_south_type,
		    south, 9, MPI_COMM_WORLD, &south_reqs[1]);
	  MPI_Waitall(2, south_reqs, MPI_STATUSES_IGNORE);
	}
	if (north >= 0) {
	  MPI_Isend(&aold[ind(xstart,1)] /* north */, 1, north_south_type,
		    north, 9, MPI_COMM_WORLD, &north_reqs[0]);
	  MPI_Irecv(&aold[ind(xstart,0)] /* north */, 1, north_south_type,
		    north, 9, MPI_COMM_WORLD, &north_reqs[1]);
	  MPI_Waitall(2, north_reqs, MPI_STATUSES_IGNORE);
	}
	if ((west >= 0) && (xstart == 1)) {
	  MPI_Isend(&aold[ind(1,1)] /* west */, 1, east_west_type,
		    west, 9, MPI_COMM_WORLD, &west_reqs[0]);
	  MPI_Irecv(&aold[ind(0,1)] /* east */, 1, east_west_type,
		    west, 9, MPI_COMM_WORLD, &west_reqs[1]);
	  MPI_Waitall(2, west_reqs, MPI_STATUSES_IGNORE);
	}
	if ((east >= 0) && (xend == bx + 1)) {
	  MPI_Isend(&aold[ind(bx,1)] /* east */, 1, east_west_type,
		    east, 9, MPI_COMM_WORLD, &east_reqs[0]);
	  MPI_Irecv(&aold[ind(bx+1,1)] /* west */, 1, east_west_type,
		    east, 9, MPI_COMM_WORLD, &east_reqs[1]);
	  MPI_Waitall(2, east_reqs, MPI_STATUSES_IGNORE);
	}

	/* update grid */
        for (j = 1; j < by+1; ++j) {
          for (i = xstart; i < xend; ++i) {
            anew[ind(i,j)] = aold[ind(i,j)]/2.0 + (aold[ind(i-1,j)] + aold[ind(i+1,j)] + aold[ind(i,j-1)] + aold[ind(i,j+1)])/4.0/2.0;
            heat += anew[ind(i,j)];
	  }
	}

      } /* end parallel region */


      /* swap working arrays */
      tmp = anew; anew = aold; aold = tmp;

      /* optional - print image */
      if (iter == niters-1)
	printarr_par(iter, anew, n, px, py, rx, ry,
		     bx, by, offx, offy, MPI_COMM_WORLD);
    }

    t2 = MPI_Wtime();

    /* free working arrays and communication buffers */
    MPI_Free_mem(aold);
    MPI_Free_mem(anew);

    MPI_Type_free(&east_west_type);
    MPI_Type_free(&north_south_type);

    /* get final heat in the system */
    MPI_Allreduce(&heat, &rheat, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    if (!rank) printf("[%i] last heat: %f time: %f\n", rank, rheat, t2-t1);

    MPI_Finalize();
}

void setup(int rank, int proc, int argc, char **argv,
           int *n_ptr, int *energy_ptr, int *niters_ptr, int *px_ptr, int *py_ptr,
           int *final_flag)
{
    int n, energy, niters, px, py;

    (*final_flag) = 0;

    if (argc < 6) {
        if (!rank)
            printf("usage: stencil_mpi <n> <energy> <niters> <px> <py>\n");
        (*final_flag) = 1;
        return;
    }

    n = atoi(argv[1]);      /* nxn grid */
    energy = atoi(argv[2]); /* energy to be injected per iteration */
    niters = atoi(argv[3]); /* number of iterations */
    px = atoi(argv[4]);     /* 1st dim processes */
    py = atoi(argv[5]);     /* 2nd dim processes */

    if (px * py != proc)
        MPI_Abort(MPI_COMM_WORLD, 1);  /* abort if px or py are wrong */
    if (n % py != 0)
        MPI_Abort(MPI_COMM_WORLD, 2);  /* abort px needs to divide n */
    if (n % px != 0)
        MPI_Abort(MPI_COMM_WORLD, 3);  /* abort py needs to divide n */

    (*n_ptr) = n;
    (*energy_ptr) = energy;
    (*niters_ptr) = niters;
    (*px_ptr) = px;
    (*py_ptr) = py;
}

void init_sources(int bx, int by, int offx, int offy, int n,
                  const int nsources, int sources[][2],
                  int *locnsources_ptr, int locsources[][2])
{
    int i, locnsources = 0;

    sources[0][0] = n / 2;
    sources[0][1] = n / 2;
    sources[1][0] = n / 3;
    sources[1][1] = n / 3;
    sources[2][0] = n * 4 / 5;
    sources[2][1] = n * 8 / 9;

    for (i = 0; i < nsources; ++i) { /* determine which sources are in my patch */
        int locx = sources[i][0] - offx;
        int locy = sources[i][1] - offy;
        if (locx >= 0 && locx < bx && locy >= 0 && locy < by) {
            locsources[locnsources][0] = locx+1; /* offset by halo zone */
            locsources[locnsources][1] = locy+1; /* offset by halo zone */
            locnsources++;
        }
    }

    (*locnsources_ptr) = locnsources;
}
