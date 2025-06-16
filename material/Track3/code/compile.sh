echo "For compile and running basic example"
mpicc hello_mpi.c -o hello_mpi
mpirun -np ./hello_mpi
echo "For compile and montecarlo example"
mpicc pi_montecarlo.c -o pi_montecarlo -lm #-lm link to maths libraries required for this code
mpirun -np ./pi_montecarlo

