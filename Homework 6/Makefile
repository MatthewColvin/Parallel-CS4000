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

proccess_tweets: Proccess_tweets.cc
	g++ Proccess_tweets.cc -o Proccess_tweets.out -lpthread

tweets_proccess: proccess_tweets
	./Proccess_tweets.out 
	#must have proper directory and files names to use this
tweets_proccess_2: proccess_tweets
	./Proccess_tweets.out 2

small_data_run_1: both
	cat smalldata.dat |./$(MAPOUT) 1 | ./$(REDUCERDOUT) 
small_data_run_2: both
	cat smalldata.dat |./$(MAPOUT) 2 | ./$(REDUCERDOUT) 

medium_data_run_1: both
	./$(MAPOUT) 1 < mediumdata.dat | ./$(REDUCERDOUT)
medium_data_run_2: both
	./$(MAPOUT) 2 < mediumdata.dat | ./$(REDUCERDOUT)

venezuela_run_1: both
	cat venezuela_201901_2_tweets_csv_hashed.csv | ./$(MAPOUT) 1 | ./$(REDUCERDOUT)
venezuela_run_2: both
	cat venezuela_201901_2_tweets_csv_hashed.csv | ./$(MAPOUT) 2 | ./$(REDUCERDOUT)


clean: both
	rm $(MAPOUT)
	rm $(REDUCERDOUT)
