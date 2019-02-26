CC=g++
DEPS = Gameoflife.cc Test_Harness.cc GOL_display.cc
OUT = Gameoflife.out
NAME = Gameoflife
TESTCASEFOLDER = Test_input
TESTS = make

parallel:$(DEPS)
	$(CC) -g $(DEPS) -pthread -o  $(OUT)

make: $(DEPS)
	$(CC) $(DEPS) -o  $(OUT)

debug: $(DEPS)
	$(CC) -g $(DEPS) -o $(OUT)

profile:$(DEPS)
	$(CC) -pg -g $(DEPS) -o $(OUT)
	./$(OUT) < $(TESTCASEFOLDER)/bigger.dat
	gprof $(OUT) | more

clean: 
	rm *.o
	rm $(OUT)

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
