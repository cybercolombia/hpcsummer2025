/*
 * Copyright (c) 2012 Torsten Hoefler. All rights reserved.
 *
 * Author(s): Torsten Hoefler <htor@illinois.edu>
 *
 */

#include "stencil_par.h"

int main(int argc, char **argv) {

  MPI_Init(&argc, &argv); 
  int r,p;
  MPI_Comm comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm, &r);
  MPI_Comm_size(comm, &p);
  int n, energy, niters;

  if (r==0) {
      // argument checking
      if(argc < 4) {
          if(!r) printf("usage: stencil_mpi <n> <energy> <niters>\n");
          MPI_Finalize();
          exit(1);
      }

      n = atoi(argv[1]); // nxn grid
      energy = atoi(argv[2]); // energy to be injected per iteration
      niters = atoi(argv[3]); // number of iterations

      // distribute arguments
      int args[3] = {n, energy, niters};
      MPI_Bcast(args, 3, MPI_INT, 0, comm);
  }
  else {
      int args[3];
      MPI_Bcast(args, 3, MPI_INT, 0, comm);
      n=args[0]; energy=args[1]; niters=args[2];
  }

  int pdims[2]={0,0};
  MPI_Dims_create(p, 2, pdims);
  int px = pdims[0];
  int py = pdims[1];

  // determine my coordinates (x,y) -- r=x*a+y in the 2d processor array
  int rx = r % px;
  int ry = r / px;
  // determine my four neighbors
  int north = (ry-1)*px+rx; if(ry-1 < 0)   north = MPI_PROC_NULL;
  int south = (ry+1)*px+rx; if(ry+1 >= py) south = MPI_PROC_NULL;
  int west= ry*px+rx-1;     if(rx-1 < 0)   west = MPI_PROC_NULL;
  int east = ry*px+rx+1;    if(rx+1 >= px) east = MPI_PROC_NULL;
  // decompose the domain
  int bx = n/px; // block size in x
  int by = n/py; // block size in y
  int offx = rx*bx; // offset in x
  int offy = ry*by; // offset in y

  //printf("%i (%i,%i) - w: %i, e: %i, n: %i, s: %i\n", r, ry,rx,west,east,north,south);

  int size = (bx+2)*(by+2); // process-local grid (including halos (thus +2))
  double *mem;
  MPI_Win win;
  // MPI 3.0
  //MPI_Win_allocate(2*size*sizeof(double), sizeof(double), MPI_INFO_NULL, comm, &mem, &win);
  // or (MPI-2.2): 
  mem = (double*)calloc(1,2*size*sizeof(double));
  MPI_Win_create(mem, 2*size*sizeof(double), sizeof(double), MPI_INFO_NULL, comm, &win);

  double *tmp;
  double *anew=mem; // each rank's offset
  double *aold=mem+size; // second half is aold!

  // initialize three heat sources
  #define nsources 3
  int sources[nsources][2] = {{n/2,n/2}, {n/3,n/3}, {n*4/5,n*8/9}};
  int locnsources=0; // number of sources in my area
  int locsources[nsources][2]; // sources local to my rank
  for (int i=0; i<nsources; ++i) { // determine which sources are in my patch
    int locx = sources[i][0] - offx;
    int locy = sources[i][1] - offy;
    if(locx >= 0 && locx < bx && locy >= 0 && locy < by) {
      locsources[locnsources][0] = locx+1; // offset by halo zone
      locsources[locnsources][1] = locy+1; // offset by halo zone
      locnsources++;
    }
  }

  double t=-MPI_Wtime(); // take time
  // create north-south datatype
  MPI_Datatype north_south_type;
  MPI_Type_contiguous(bx, MPI_DOUBLE, &north_south_type);
  MPI_Type_commit(&north_south_type);
  // create east-west type
  MPI_Datatype east_west_type;
  MPI_Type_vector(by,1,bx+2,MPI_DOUBLE, &east_west_type);
  MPI_Type_commit(&east_west_type);

  double heat; // total heat in system
  for(int iter=0; iter<niters; ++iter) {
    // refresh heat sources
    for(int i=0; i<locnsources; ++i) {
      aold[ind(locsources[i][0],locsources[i][1])] += energy; // heat source
    }

    // exchange data with neighbors
    MPI_Win_fence(0, win);
    int offset = size*((iter+1)%2);
    MPI_Put(&aold[ind(1,1)], 1, north_south_type, north, ind(1,by+1)+offset, 1, north_south_type, win);
    MPI_Put(&aold[ind(1,by)], 1, north_south_type, south, ind(1,0)+offset, 1, north_south_type, win);
    MPI_Put(&aold[ind(bx,1)], 1, east_west_type, east, ind(bx+1,1)+offset, 1, east_west_type, win);
    MPI_Put(&aold[ind(1,1)], 1, east_west_type, west, ind(0,1)+offset, 1, east_west_type, win);
    MPI_Win_fence(0, win);

    // update grid points
    heat = 0.0;
    for(int j=1; j<by+1; ++j) {
      for(int i=1; i<bx+1; ++i) {
        anew[ind(i,j)] = aold[ind(i,j)]/2.0 + (aold[ind(i-1,j)] + aold[ind(i+1,j)] + aold[ind(i,j-1)] + aold[ind(i,j+1)])/4.0/2.0;
        heat += anew[ind(i,j)];
      }
    }

    // swap arrays
    tmp=anew; anew=aold; aold=tmp;

    // optional - print image
  	if(iter == niters-1) printarr_par(iter, anew, n, px, py, rx, ry, bx, by, offx, offy, comm);
  }
  t+=MPI_Wtime();

  MPI_Type_free(&east_west_type);
  MPI_Type_free(&north_south_type);

  // get final heat in the system
  double rheat;
  MPI_Allreduce(&heat, &rheat, 1, MPI_DOUBLE, MPI_SUM, comm);
  if(!r) printf("[%i] last heat: %f time: %f\n", r, rheat, t);

  MPI_Finalize();
}
