//
//      Matthew Colvin 
//      Spring  2019
//
//      Written to parallelize homework 6 on the input of all of the twitter data
//      from the different countries for the program to be ran all that you would have 
//      do is run this in the same directroy as a folder called tweetdata in tweetdata. 
//      tweetdata will contain shortened names for the csv files.   
//
//      exepath/tweetdata/iran.csv
//      exepath/tweetdata/venezuela1.csv
//      exepath/tweetdata/venezuela2.csv
//      exepath/tweetdata/russia.csv
//      exepath/tweetdata/iranian.csv
//      exepath/tweetdata/ira.csv
//
//       usage: ./proccess_tweets.out <gram_size> 
//              no argument defaults to word count.
//
#include <iostream>
#include <thread> // will be using thread class to parallelize map reduce 
#include<iomanip>
#include<fstream>
#include<algorithm>
#include<vector>
#include<assert.h>
#include<sstream>
#include<string>
#include<map>
#include<fstream>

bool invalidChar (char c){ // charactes must be ascii
    return !(c>=0 && c <128);   
} 

class twittercsvparser{ // helps find n-grams of csv twitter data

    /*  csv data by coloumn number
        0  tweetid
        1  userid                     2 user_display_name    3 user_screen_name      4 user_reported_location
        5  user_profile_description   6 user_profile_ur      7 follower_count        8 following_count
        9  account_creation_date     10 account_language    11 tweet_text           12 tweet_time
        13 tweet_client_name         14 in_reply_to_tweetid 15 in_reply_to_userid   16 quoted_tweet_tweetid
        17 is_retweet                18 retweet_userid      19 retweet_tweetid      20 latitude
        21 longitude                 22 quote_count         23 reply_count          24 like_count 
        25 retweet_count             26 hashtags            27 urls                 28 user_mentions  
        29 poll_choices 
    */
    
    private:
        int nth_gram_to_produce=1; //default to word count
        int num_entries=0;
        std::vector<std::string> tweet_text;//,user_display_name; // vectors for the data in the csv
        std::map<std::string,int> ngrams_of_tweet_text;
    public:
        void nth_gram(int nth_gram){ nth_gram_to_produce = nth_gram;}

        void load_data(std::istream& csv_in){ // the stream the csv is coming in on 
            while(!csv_in.fail()){
            
                std::string rawdata;

                for(int i=0; i<30; i++){     
                    csv_in >> quoted(rawdata);
                    if(csv_in.fail()){
                        break; // leave the loop so tweet was not 
                    }
                    if ( csv_in.peek() == ',' || csv_in.peek()=='\n' ){
                        csv_in.ignore(); // ignore the comma
                    }else{
                    
                    // cout << "why would it not get a comma here?" <<endl;// fount out why the input file has data like this 
                        std::string tmp;                                                 // ...,"entry1","entry2""entry2","entry3"
                                                                                    //another case "entry1","entry2"ihavenoclue?, sutff"  
                    
                        while(csv_in.peek()!=',' && !csv_in.fail()){
                            csv_in >> quoted(tmp);
                            rawdata += tmp; 
                        }
                        csv_in.ignore(); // found the comma so ignore it 

                    }
                    switch (i){  //ith coloumn of the spreadsheet
                        case 11:  // tweet_text the 11th colomn 
                            tweet_text.push_back(rawdata);
                            break;
                        case 26: //Hashtags
                            break;
                        case 2:
                            //user_display_name.push_back(rawdata);
                            break;
                    }
                }num_entries++;
            }
            load_tweet_text_map();
        }
        void load_tweet_text_map(){
            for(std::vector<std::string>::iterator it = tweet_text.begin();it!=tweet_text.end();it++){ // for every tweet...
                std::string tweet = *it;
                std::string gram; // the string to be check if it is in the map
                
                

                tweet.erase(remove_if(tweet.begin(),tweet.end(), invalidChar), tweet.end());//  remove every non ascii character.

                std::vector<char> to_remove = {'"','!','?',';',':','.','|',',','&',')','(','-','[',']','/'};
                std::vector<char>::iterator chars;
                for(chars = to_remove.begin();chars!=to_remove.end();chars++){
                    tweet.erase(remove(tweet.begin(),tweet.end(),*chars),tweet.end()); // remove all single charactes in the to_remove vector
                } 

                remove_web_and_hashtags(tweet);
                std::stringstream tweet_ss(tweet);

                int next_loop_stream_start_pos = 0;
                while(!tweet_ss.fail()){
                    for(int i=0; i < nth_gram_to_produce && !tweet_ss.fail(); i++ ){
                        std::string tmp;
                        tweet_ss >> tmp;
                        if(i == 0){next_loop_stream_start_pos = tweet_ss.tellg();} // save position after reading one word
                        if(tweet_ss.fail()){break;} // when it failes we are done with that tweet and an ngram was not produced.
                        if(i!=nth_gram_to_produce-1){ tmp += " ";}  // put a space in between words except last word 
                        gram += tmp;
                        
                        std::map<std::string,int>::iterator it = ngrams_of_tweet_text.find(gram);

                        if(i == nth_gram_to_produce-1){//the string was long enougth to be added
                            if (it != ngrams_of_tweet_text.end()) { // the string was not found in the map
                                it->second++;
                            }else{
                                std::pair<std::string,int> new_gram;
                                new_gram.first = gram;
                                new_gram.second = 1;
                                ngrams_of_tweet_text.insert(new_gram);
                            }
                        } // gram added to the map or incremented the occurence
                    }// creates the gram with the proper number of strings or untill we run out of words in the tweet
                    gram="";// reset gram for next one to be produced.
                    tweet_ss.seekg(next_loop_stream_start_pos); // move stream pointer where it was perviously stored
                }
            }
        }
        void remove_web_and_hashtags(std::string& tweet){
            std::stringstream tweet_ss(tweet);
            tweet_ss.str(tweet); // sync tweet_ss with tweet after single chars are removed
            int starttoremove = tweet.find("https//"); // find the first web address. // i removed all the :
            while( starttoremove != std::string::npos ){
                
                tweet_ss.seekg(starttoremove); // got to one postion before the web address.

                std::string address;
                tweet_ss >> address; // get the address need to know the length to remove

                tweet.replace(starttoremove,address.length(),""); // delete the address out of string
                tweet_ss.str(tweet); // update the stream so it matches the tweet.

                starttoremove = tweet.find("https//",starttoremove+1); // find next location of http:// in new string 
            }// removed all https web addresses from tweet.
        
            starttoremove = tweet.find("#"); // find the first web address.
            while( starttoremove != std::string::npos ){
                tweet_ss.seekg(starttoremove); // got to one postion before the hashtag.

                std::string hashtag;
                tweet_ss >> hashtag; // get the address need to know the length to remove

                tweet.replace(starttoremove,hashtag.length(),""); // delete the address out of string
                tweet_ss.str(tweet); // update the stream so it matches the tweet.

                starttoremove = tweet.find("#",starttoremove+1); // find next location of http:// in new string 
            }// removed all https web addresses from tweet.
        




        }
        void output_map(std::ostream& out,int num_occurences){ //// output map
            std::map<std::string,int>:: iterator mapit;
            for(mapit = ngrams_of_tweet_text.begin(); mapit!= ngrams_of_tweet_text.end(); mapit++ ){
                if(mapit->second > num_occurences ) 
                    out << quoted(mapit->first)<< " " << mapit->second << std::endl;
            } 
                /////////////// 
        }

};

std::map<std::string,int> reduce_map(std::istream& in){
    std::map<std::string,int> map;
    while (!in.fail()){
        std::string gram ; 
        int value;
        in >> quoted(gram);
        in >> value; 

        std::map<std::string,int>:: iterator mapit = map.find(gram); // see if we already have it in map?

        if (mapit != map.end()){ //it is in the map already 
            mapit->second += value; // add the value since it has the same key.
        }else{ // add it to to the map
            map.emplace(std::pair<std::string,int>(gram,value)); // put the value just recieved into the map.
        }

    }
    return(map);
 
};
void output_map(std::ostream& out,std::map<std::string,int> output, int num_occurences){ //// output map entries that have value greater than num_occurences
    std::map<std::string,int>:: iterator mapit;
    for(mapit = output.begin(); mapit!= output.end(); mapit++ ){
        if(mapit->second > num_occurences ){ 
            out << mapit->first << " , " << mapit->second << std::endl;
        } 
    }  
}   

int main(int argc,char* argv[]){
    
    int ngram_size;
    switch(argc){
        case 1:
            ngram_size =1; // default to word count
            break;
        case 2:
            ngram_size = atoi(argv[1]);

    }
    twittercsvparser *iran,*venezuela1,*venezuela2,*russia,*ira,*iranian;
    std::ifstream iran_ins,venezuela1_ins,venezuela2_ins,russia_ins,ira_ins,iranian_ins;

    iran = new twittercsvparser();
    venezuela1 = new twittercsvparser();
    venezuela2 = new twittercsvparser();
    russia = new twittercsvparser();
    ira = new twittercsvparser();
    iranian = new twittercsvparser();

    iran->nth_gram(ngram_size);
    venezuela1->nth_gram(ngram_size);
    venezuela2->nth_gram(ngram_size);
    russia->nth_gram(ngram_size);
    ira->nth_gram(ngram_size);
    iranian->nth_gram(ngram_size);

    iran_ins.open("tweetdata/iran.csv");
    venezuela1_ins.open("tweetdata/venezuela1.csv");
    venezuela2_ins.open("tweetdata/venezuela2.csv");
    russia_ins.open("tweetdata/russia.csv");
    iranian_ins.open("tweetdata/iranian.csv");
    ira_ins.open("tweetdata/ira.csv");

    if(iran_ins.fail()){ std::cout << "iran.csv create tweetdata and rename .csv file" << std::endl; }
    if(venezuela1_ins.fail()){ std::cout << "venezuela1.csv create tweetdata and rename .csv file" << std::endl; }
    if(venezuela2_ins.fail()){ std::cout << "venezuela2.csv create tweetdata and rename .csv file" << std::endl; }
    if(russia_ins.fail()){ std::cout << "russia.csv create tweetdata and rename .csv file" << std::endl; }
    if(iranian_ins.fail()){ std::cout << "iranian.csv create tweetdata and rename .csv file" << std::endl; }
    if(ira_ins.fail()){ std::cout << "ira.csv create tweetdata and rename .csv file" << std::endl; }

    std::cout << "FILES  OPENED" << std::endl;
    std::cout.flush();

    std::thread iranthread (&twittercsvparser::load_data,iran,std::ref(iran_ins));
    std::thread venezuela1thread(&twittercsvparser::load_data,venezuela1,std::ref(venezuela1_ins));
    std::thread venezuela2thread(&twittercsvparser::load_data,venezuela1,std::ref(venezuela1_ins));
    std::thread russiathread(&twittercsvparser::load_data,russia,std::ref(russia_ins));
    std::thread iranianthread(&twittercsvparser::load_data,iranian,std::ref(iranian_ins));
    std::thread irathread(&twittercsvparser::load_data,ira,std::ref(ira_ins));


    iranthread.join();
    std::cout << "Iran done" << std::endl;
    std::cout.flush();
    venezuela1thread.join();
    std::cout << "venezeual done" << std::endl;
    std::cout.flush();
    venezuela2thread.join();
    std::cout << "vensula 2 done" << std::endl;
    std::cout.flush();
    russiathread.join();
    std::cout << "russia done" << std::endl;
    std::cout.flush();
    iranianthread.join();
    std::cout << "iranian done" << std::endl;
    std::cout.flush();
    irathread.join();
    std::cout << "ira done" << std::endl;
    std::cout.flush();


    std::cout << "DONE Proccessing Files" << std::endl ;
    std::cout.flush();

    std::ofstream tmp_ofs;

    tmp_ofs.open("0xAA55EF53"); // make temporary directory to hold maps // didnt know if ram woudl be able to hold it.
        iran->output_map(tmp_ofs,0);
        venezuela1->output_map(tmp_ofs,0);
        venezuela2->output_map(tmp_ofs,0);
        russia->output_map(tmp_ofs,0);
        iranian->output_map(tmp_ofs,0);
        ira->output_map(tmp_ofs,0);
    tmp_ofs.close();

    delete iran;
    delete venezuela1;
    delete venezuela2;
    delete russia;
    delete iranian;
    delete ira;

    std::ifstream to_be_reduced;
    to_be_reduced.open("0xAA55EF53"); // open the map dump file 

    output_map(std::cout,reduce_map(to_be_reduced),10000);
    // output map that has been reduced to standard output if the key is grater than num_occurences

    to_be_reduced.close();
    remove("0xAA55EF53");

    return 0;
}