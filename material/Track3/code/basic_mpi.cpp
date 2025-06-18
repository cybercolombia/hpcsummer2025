#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    printf("MPI environment initialized.\n");
    int version, subversion;
    MPI_Get_version(&version, &subversion);
    printf("MPI Version: %d.%d\n", version, subversion);
    MPI_Finalize();
    printf("MPI environment finalized.\n");
    return 0;
}
