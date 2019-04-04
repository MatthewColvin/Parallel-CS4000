DEPS=crackermpi.cc
OUT=cracker
MPICC=/usr/bin/mpic++.openmpi
MPIEXE=/usr/bin/mpirun.openmpi

no_mpi:
	g++ cracker.cc -o cracker

cracker: $(DEPS)
	$(MPICC) -o $(OUT) $(DEPS) -lcrypt

mpi_example:
	$(MPICC) -g helloworldmpi.cc

example_run: mpi_example
	$(MPIEXE) --hostfile hostfile1 ./a.out 

example_run_local : mpi_example
	$(MPIEXE) -np 4 ./a.out

example_leak: mpi_example
	valgrind mpirun --hostfile hostfile1 ./a.out

cracker_run_local: clean cracker
	$(MPIEXE) -np 4 cracker 

cracker_run_server: clean cracker
	$(MPIEXE) -np 8 --host 127.0.0.1,odd03.cs.ohio.edu cracker  	

clean:
	rm $(OUT) 
