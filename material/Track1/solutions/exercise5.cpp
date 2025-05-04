#include <cstdio>

void matrix_multiply(const int *ptr_A, const int *ptr_B, int *ptr_res, 
                     int nrows, int ncols, int nn) {
  for(int i = 0; i < nrows; i++){
    for(int j = 0; j < ncols; j++)
      ptr_res[i*ncols+j] = 0;
  }
    
  for(int i = 0; i < nrows; i++){
    for(int j = 0; j < ncols; j++){
      for(int k = 0; k < nn; k++)
    	ptr_res[i*ncols+j] += ptr_A[i*nn+k] * ptr_B[k*ncols+j];  
    }
  }
}

int main(int argc, char *argv[]) {
  int nrows = 4;
  int ncols = 2;
  int nn = 3;
    
  //allocate memory for the matrices
  int *a = new int[nrows*nn];
  int *b = new int[nn*ncols];
  int *c = new int[nrows*ncols];
  
  //initialize a and b
  for(int i = 0; i < nrows; i++){
    for(int j = 0; j < nn; j++)
        a[i*nn+j] = 1;
  }
  for(int i = 0; i < nn; i++){
    for(int j = 0; j < ncols; j++)
        b[i*ncols+j] = 1;
  } 

  matrix_multiply(a,b,c,nrows,ncols,nn);

  for(int i = 0; i < nrows; i++){
    for(int j = 0; j < ncols; j++)
      printf("%d ",c[i*ncols+j]);
    printf("\n");
  }
    
  delete[] a;
  delete[] b;
  delete[] c;
  
  return 0;
}