MAPDEPS=mapper.cc
MAPOUT=mapper.out
REDUCERDEPS=
REDUCERDOUT=
CC=g++


mapper: $(MAPDEPS)
	g++ -g $(MAPDEPS) -o $(MAPOUT) 

small_data_run_1: mapper
	./$(MAPOUT) 1 < smalldata.dat 
small_data_run_2: mapper
	./$(MAPOUT) 2 < smalldata.dat 

medium_data_run_1: mapper
	./$(MAPOUT) 1 < mediumdata.dat
medium_data_run_2: mapper
	./$(MAPOUT) 2 < mediumdata.dat

venezuela_run_1: mapper
	./$(MAPOUT) 1 < venezuela_201901_2_tweets_csv_hashed.csv
venezuela_run_2: mapper
	./$(MAPOUT) 2 < venezuela_201901_2_tweets_csv_hashed.csv	

clean:
	rm $(MAPOUT)
