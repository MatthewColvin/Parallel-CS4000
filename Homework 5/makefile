DEPS=crackermpi.cc
OUT=cracker.out
MPICC=/usr/bin/mpic++
MPIEXE=/usr/bin/mpirun

no_mpi:
	g++ cracker.cc -o $(OUT)

cracker: $(DEPS)
	$(MPICC) -o $(OUT) $(DEPS) -lcrypt

mpi_example:
	$(MPICC) -g examples/helloworldmpi.cc

example_run: mpi_example
	$(MPIEXE) --hostfile hostfile1 ./a.out 

example_run_local : mpi_example
	$(MPIEXE) -np 4 ./a.out

example_leak: mpi_example
	valgrind mpirun --hostfile hostfile1 ./a.out

cracker_run_local: clean cracker
	$(MPIEXE) --oversubscribe -np 8 $(OUT)

cracker_run_server: clean cracker
	$(MPIEXE) --hostfile hostfile $(OUT)  	

clean:
	rm $(OUT)
