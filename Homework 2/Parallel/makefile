CC=g++
DEPS = Hamiltonian_Path.cc
OUT = Hamiltonian_Path.out
NAME = Hamiltonian_Path
TESTS = parallel

parallel:$(DEPS)
	$(CC) -g $(DEPS) -fopenmp -o  $(OUT)

make: $(DEPS)
	$(CC) $(DEPS) -o  $(OUT)

debug: $(DEPS)
	$(CC) -g $(DEPS) -o $(OUT)

profile:$(DEPS)
	$(CC) -pg -g $(DEPS) -o $(OUT)
	./$(OUT) < test_cases/bigger.dat
	gprof $(OUT) | more

clean: 
	rm *.o
	rm $(OUT)

small_graph: $(TESTS)
	./$(OUT) < test_cases/small_graph.dat
small_graph2: $(TESTS)
	./$(OUT) < test_cases/small_graph2.dat
small_graph3: $(TESTS)
	./$(OUT) < test_cases/small_graph3.dat
big: $(TESTS)
	./$(OUT) < test_cases/big.dat
bigger: $(TESTS)
	./$(OUT) < test_cases/bigger.dat
bigger2: $(TESTS)
	./$(OUT) < test_cases/bigger2.dat
biggest: $(TESTS)
	./$(OUT) < test_cases/biggest.dat