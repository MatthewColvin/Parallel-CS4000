CC=g++
DEPS = GameOfLife.cc Test_Harness.cc 
OUT = GameOfLife.out
NAME = Gameoflife
TESTCASEFOLDER = Test_input
TESTS = parallel

parallel:$(DEPS)
	$(CC) -g $(DEPS) -pthread -lncurses -o  $(OUT)

make: $(DEPS)
	$(CC) $(DEPS) -lncurses -o  $(OUT)

debug: $(DEPS)
	$(CC) -g $(DEPS) -o $(OUT)

profile:$(DEPS)
	$(CC) -pg -g $(DEPS) -o $(OUT)
	./$(OUT) < $(TESTCASEFOLDER)/blinker.dat
	gprof $(OUT) | more

clean: 
	rm -f *.o
	rm -f *.out

blinker: $(TESTS)
	./$(OUT) < $(TESTCASEFOLDER)/blinker.dat
eternal_blinker: $(TESTS)
	./$(OUT) < $(TESTCASEFOLDER)/eternal_blinker.dat
mat1: $(TESTS)
	./$(OUT) < $(TESTCASEFOLDER)/mat1.dat
original: $(TESTS)
	./$(OUT) < $(TESTCASEFOLDER)/original.dat
pentadecathlon3: $(TESTS)
	./$(OUT) < $(TESTCASEFOLDER)/pentadecathlon3.dat
pulsar: $(TESTS)
	./$(OUT) < $(TESTCASEFOLDER)/pulsar.dat
rand: $(TESTS)
	./$(OUT) < $(TESTCASEFOLDER)/rand.dat
