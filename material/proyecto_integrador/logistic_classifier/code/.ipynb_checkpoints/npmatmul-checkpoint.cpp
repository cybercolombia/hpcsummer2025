#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h> // Include this for automatic conversion of std::tuple
#include <omp.h>
#include <tuple>

namespace py = pybind11;


std::tuple<py::array_t<double>, double> matrix_multiply(py::array_t<double,py::array::c_style> A,
				    py::array_t<double,py::array::c_style> B) {
  double start = omp_get_wtime();
    
  //Read the numpy array buffers
  py::buffer_info A_buf = A.request();
  py::buffer_info B_buf = B.request();

  if(A_buf.ndim != 2 || B_buf.ndim != 2)
    throw std::runtime_error("Number of dimensions must be two");
  if(A_buf.shape[1] != B_buf.shape[0])
    throw std::runtime_error("Matrix dimensions do not match for multiplication");

  int nrows = A_buf.shape[0];
  int ncols = B_buf.shape[1];
  int nn = A_buf.shape[1];

  //Create a numpy array bufferto store the result
  py::array_t<double> result = py::array_t<double>({nrows,ncols});
  result[py::make_tuple(py::ellipsis())] = 0.f; //initialize to zeros
  py::buffer_info res_buf = result.request();

  //Create pointers to the buffers
  double *ptr_A = static_cast<double *>(A_buf.ptr);
  double *ptr_B = static_cast<double *>(B_buf.ptr);
  double *ptr_res = static_cast<double *>(res_buf.ptr);

  for(int i = 0; i < nrows; i++){
    for(int j = 0; j < ncols; j++){
      for(int k = 0; k < nn; k++)
	    ptr_res[i*ncols+j] += ptr_A[i*nn+k] * ptr_B[k*ncols+j];  
    }
  }
    
  double end = omp_get_wtime();
  double run_time = end-start;
  
  return std::make_tuple(result, run_time);
}


std::tuple<py::array_t<double>, double>  matrix_multiply_omp(py::array_t<double,py::array::c_style> A,
					py::array_t<double,py::array::c_style> B) {
  double start = omp_get_wtime();
    
  //Read the numpy array buffers
  py::buffer_info A_buf = A.request();
  py::buffer_info B_buf = B.request();

  if(A_buf.ndim != 2 || B_buf.ndim != 2)
    throw std::runtime_error("Number of dimensions must be two");
  if(A_buf.shape[1] != B_buf.shape[0])
    throw std::runtime_error("Matrix dimensions do not match for multiplication");

  int nrows = A_buf.shape[0];
  int ncols = B_buf.shape[1];
  int nn = A_buf.shape[1];

  //Create a numpy array bufferto store the result
  py::array_t<double> result = py::array_t<double>({nrows,ncols});
  result[py::make_tuple(py::ellipsis())] = 0.f; //initialize to zeros
  py::buffer_info res_buf = result.request();

  //Create pointers to the buffers
  double *ptr_A = static_cast<double *>(A_buf.ptr);
  double *ptr_B = static_cast<double *>(B_buf.ptr);
  double *ptr_res = static_cast<double *>(res_buf.ptr);

  omp_set_num_threads(4);
    
  //Perform the matrix multiplication
  #pragma omp parallel default(shared)
  {
    //if(omp_get_thread_num() == 0)
    //  printf("num threads: %d\n",omp_get_num_threads());
    #pragma omp for schedule(auto), collapse(2)
    for(int i = 0; i < nrows; i++){
      for(int j = 0; j < ncols; j++){
	    for(int k = 0; k < nn; k++)
	      ptr_res[i*ncols+j] += ptr_A[i*nn+k] * ptr_B[k*ncols+j]; 
      }
    }
  }

  double end = omp_get_wtime();
  double run_time = end-start;
    
  return std::make_tuple(result, run_time);
}


//Pybind11 module macro that creates an entry point that will be invoked
// when the python interpreter imports an extension module.
PYBIND11_MODULE(npmatmul, m) {
  m.doc() = "Pybind11 module that performs numpy array multiplication";
  m.def("matrix_multiply",&matrix_multiply,"Function that multiplies two matrices");
  m.def("matrix_multiply_omp",&matrix_multiply_omp,"Function that multiplies two matrices. Uses OMP");
}

