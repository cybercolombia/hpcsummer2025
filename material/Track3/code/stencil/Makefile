MPICC=mpicxx
CPPFLAGS=-I/usr/include
LDFLAGS=-lm -lz
OBJS=printarr_par.o
BINS=stencil stencil_blocked stencil_mpi stencil_mpi_ddt stencil_mpi_ddt_overlap stencil_mpi_ddt_overlap_carttopo \
stencil_mpi_ddt_overlap_carttopo_neighcolls \
stencil_mpi_carttopo_neighcolls stencil_mpi_overlap_carttopo_neighcolls \
stencil_mpi_shmem stencil_mpi_ddt_rma ga_mpi_ddt_rma \
stencil_mpi_ddt_funneled stencil_mpi_ddt_multiple 

all: $(BINS)

%.o: %.c Makefile
	$(MPICC) $(CFLAGS) $(LDFLAGS)$< -c -o $@

stencil: stencil.c Makefile
	$(MPICC) $(CFLAGS)  $(LDFLAGS) -o $@ $<

stencil_blocked: stencil_blocked.c Makefile
	$(MPICC) $(CFLAGS)  $(LDFLAGS) -o $@ $<

stencil_mpi: stencil_mpi.c Makefile $(OBJS)
	$(MPICC) $(CFLAGS)  $(LDFLAGS) -o $@ $< $(OBJS)

stencil_mpi_carttopo_neighcolls: stencil_mpi_carttopo_neighcolls.c Makefile $(OBJS) 
	$(MPICC) $(CFLAGS)  $(LDFLAGS) -o $@ $< $(OBJS)

stencil_mpi_overlap_carttopo_neighcolls: stencil_mpi_overlap_carttopo_neighcolls.c Makefile $(OBJS) 
	$(MPICC) $(CFLAGS)  $(LDFLAGS) -o $@ $< $(OBJS)

stencil_mpi_ddt_overlap_carttopo_neighcolls: stencil_mpi_ddt_overlap_carttopo_neighcolls.c Makefile $(OBJS) 
	$(MPICC) $(CFLAGS)  $(LDFLAGS) -o $@ $< $(OBJS)

stencil_mpi_ddt: stencil_mpi_ddt.c Makefile $(OBJS)
	$(MPICC) $(CFLAGS)  $(LDFLAGS) -o $@ $< $(OBJS)

stencil_mpi_ddt_overlap: stencil_mpi_ddt_overlap.c Makefile $(OBJS)
	$(MPICC) $(CFLAGS)   $(LDFLAGS) -o $@ $< $(OBJS)

stencil_mpi_ddt_overlap_carttopo: stencil_mpi_ddt_overlap_carttopo.c Makefile $(OBJS)
	$(MPICC) $(CFLAGS)  $(LDFLAGS) -o $@ $< $(OBJS)

stencil_mpi_shmem: stencil_mpi_shmem.c Makefile $(OBJS)
	$(MPICC) $(CFLAGS)  $(LDFLAGS) -o $@ $<  $(OBJS)

stencil_mpi_ddt_rma: stencil_mpi_ddt_rma.c Makefile $(OBJS)
	$(MPICC) $(CFLAGS)  $(LDFLAGS) -o $@ $< $(OBJS)

ga_mpi_ddt_rma: ga_mpi_ddt_rma.c Makefile $(OBJS)
	$(MPICC) $(CFLAGS)  $(LDFLAGS) -o $@ $<  $(OBJS)

stencil_mpi_ddt_multiple: stencil_mpi_ddt_multiple.c Makefile $(OBJS)
	$(MPICC) $(CFLAGS)  $(LDFLAGS) -fopenmp -o $@ $<  $(OBJS)

stencil_mpi_ddt_funneled: stencil_mpi_ddt_funneled.c Makefile $(OBJS)
	$(MPICC) $(CFLAGS)  $(LDFLAGS) -fopenmp -o $@ $<  $(OBJS)

clean:
	rm -f $(BINS) $(OBJS)
	rm -f output*.bmp
