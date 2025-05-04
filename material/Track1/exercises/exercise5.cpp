#include <cstdio>

void matrix_multiply(const int *ptr_A, const int *ptr_B, int *ptr_res, 
                     int nrows, int ncols, int nn) {
  for(int i = 0; i < nrows; i++){
    for(int j = 0; j < ncols; j++)
      ptr_res[i*ncols+j] = 0;
  }
    
  //Implement matrix multiplication
    
    
}

int main(int argc, char *argv[]) {
  int nrows = 4;
  int ncols = 2;
  int nn = 3;
    
  //allocate memory for the matrices
  
  //initialize a and b to 1

  matrix_multiply(a,b,c,nrows,ncols,nn);

  for(int i = 0; i < nrows; i++){
    for(int j = 0; j < ncols; j++)
      printf("%d ",c[i*ncols+j]);
    printf("\n");
  }
    
  //Free the memory
  
  return 0;
}