MAPDEPS=mapper.cc
MAPOUT=mapper.out
REDUCERDEPS=reducer.cc
REDUCERDOUT=reducer.out
CC=g++


mapper: $(MAPDEPS)
	g++ -g $(MAPDEPS) -o $(MAPOUT) 
reducer:
	g++ -g $(REDUCERDEPS) -o $(REDUCERDOUT)

both: mapper reducer



small_data_run_1: both
	./$(MAPOUT) 1 < smalldata.dat | ./$(REDUCERDOUT) 
small_data_run_2: both
	./$(MAPOUT) 2 < smalldata.dat | ./$(REDUCERDOUT)

medium_data_run_1: both
	./$(MAPOUT) 1 < mediumdata.dat | ./$(REDUCERDOUT)
medium_data_run_2: both
	./$(MAPOUT) 2 < mediumdata.dat | ./$(REDUCERDOUT)

venezuela_run_1: both
	./$(MAPOUT) 1 < venezuela_201901_2_tweets_csv_hashed.csv | ./$(REDUCERDOUT)
venezuela_run_2: both
	./$(MAPOUT) 2 < venezuela_201901_2_tweets_csv_hashed.csv | ./$(REDUCERDOUT)


clean: both
	rm $(MAPOUT)
	rm $(REDUCERDOUT)
