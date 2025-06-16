#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define ind(i,j) (i)*n+j
#define min(x,y) (x < y ? x : y)

void printarr(double *a, int n) {
  // does nothing right now, should record each "frame" as image
  FILE *fp = fopen("heat.svg", "w");
  const int size = 5;

  fprintf(fp, "<html>\n<body>\n<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">");

  fprintf(fp, "\n<rect x=\"0\" y=\"0\" width=\"%i\" height=\"%i\" style=\"stroke-width:1;fill:rgb(0,0,0);stroke:rgb(0,0,0)\"/>", size*n, size*n);
  for(int i=1; i<n+1; ++i)
    for(int j=1; j<n+1; ++j) {
      int rgb = (a[ind(i,j)] > 0) ? rgb = (int)round(255.0*a[ind(i,j)]) : 0.0;
      if(rgb>255) rgb=255;
      if(rgb) fprintf(fp, "\n<rect x=\"%i\" y=\"%i\" width=\"%i\" height=\"%i\" style=\"stroke-width:1;fill:rgb(%i,0,0);stroke:rgb(%i,0,0)\"/>", size*(i-1), size*(j-1), size, size, rgb, rgb);
    }
  fprintf(fp, "</svg>\n</body>\n</html>");


  fclose(fp);
}

int main(int argc, char **argv) {

  int n = atoi(argv[1]); // nxn grid
  int energy = atoi(argv[2]); // energy to be injected per iteration
  int niters = atoi(argv[3]); // number of iterations
  int bs = atoi(argv[4]); // block size
  double *aold = (double*)calloc(1,(n+2)*(n+2)*sizeof(double)); // 1-wide halo zones!
  double *anew = (double*)calloc(1,(n+2)*(n+2)*sizeof(double)); // 1-wide halo-zones!
  double *tmp;

  MPI_Init(NULL, NULL);

  #define nsources 3
  int sources[nsources][2] = {{n/2,n/2}, {n/3,n/3}, {n*4/5,n*8/9}};
  
  double heat; // total heat in system
  double t=-MPI_Wtime();
  for(int iter=0; iter<niters; ++iter) {
    heat = 0.0;
    for(int ib=1; ib<n+1; ib+=bs)
      for(int jb=1; jb<n+1; jb+=bs)
        for(register int i=ib; i<min(ib+bs, n+1); ++i)
          #pragma ivdep
          for(register int j=jb; j<min(jb+bs,n+1); j+=2) {
            register double x=anew[ind(i,j)]/2.0;
            register double y=anew[ind(i,j+1)]/2.0;
            x = x + (aold[ind(i-1,j)] + aold[ind(i+1,j)] + aold[ind(i,j-1)] + aold[ind(i,j+1)])/8.0;
            y = y + (aold[ind(i-1,j+1)] + aold[ind(i+1,j+1)] + aold[ind(i,j)] + aold[ind(i,j+2)])/8.0;
            heat += x + y;
            anew[ind(i,j)] = x;
            anew[ind(i,j+1)] = y;
          }
    
    for(int i=0; i<nsources; ++i) {
      anew[ind(sources[i][0],sources[i][1])] += energy; // heat source
    }
    tmp=anew; anew=aold; aold=tmp; // swap arrays
  }
  t+=MPI_Wtime();
  printarr(anew, n);
  printf("last heat: %f time: %f\n", heat, t);

  MPI_Finalize();
}
