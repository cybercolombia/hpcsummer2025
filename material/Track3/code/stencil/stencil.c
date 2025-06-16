#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

// row-major order
#define ind(i,j) (j)*n+i

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
  double *aold = (double*)calloc(1,(n+2)*(n+2)*sizeof(double)); // 1-wide halo zones!
  double *anew = (double*)calloc(1,(n+2)*(n+2)*sizeof(double)); // 1-wide halo-zones!
  double *tmp;

  MPI_Init(NULL, NULL);

  #define nsources 3
  int sources[nsources][2] = {{n/2,n/2}, {n/3,n/3}, {n*4/5,n*8/9}};
  
  double heat=0.0; // total heat in system
  double t=-MPI_Wtime();
  for(int iter=0; iter<niters; ++iter) {
    for(int j=1; j<n+1; ++j) {
      for(int i=1; i<n+1; ++i) {
        anew[ind(i,j)] = aold[ind(i,j)]/2.0 + (aold[ind(i-1,j)] + aold[ind(i+1,j)] + aold[ind(i,j-1)] + aold[ind(i,j+1)])/4.0/2.0;
        heat += anew[ind(i,j)];
      }
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
