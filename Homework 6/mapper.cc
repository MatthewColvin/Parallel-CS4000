#include<iostream>
#include<iomanip>
#include<fstream>
#include<algorithm>
#include<vector>
#include<assert.h>
#include<sstream>
#include<string>
#include<map>

bool invalidChar (char c) // charactes must be ascii
{  
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
    std::vector<std::string> tweet_text,user_display_name; // vectors for the data in the csv
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
                        user_display_name.push_back(rawdata);
                }
            }num_entries++;

        }
    }
    void load_tweet_text_map(){
        for(std::vector<std::string>::iterator it = tweet_text.begin();it!=tweet_text.end();it++){ // for every tweet...
            std::string tweet = *it;
            std::stringstream tweet_ss(tweet);

            std::string gram; // the string to be check if it is in the map
            
            size_t starttoremove = tweet.find("https://"); // find the first web address.
            while( starttoremove != std::string::npos ){
                
                tweet_ss.seekg(starttoremove-1); // got to one postion before the web address.

                std::string address;
                tweet_ss >> address; // get the address need to know the length to remove

                tweet.replace(starttoremove,address.length(),""); // delete the address out of string
                tweet_ss.str(tweet); // update the stream so it matches the tweet.

                starttoremove = tweet.find("https://",starttoremove+1); // find next location of http:// in new string 
            }// removed all https web addresses from tweet.

            tweet.erase(remove_if(tweet.begin(),tweet.end(), invalidChar), tweet.end());//  remove every non ascii character.

            std::vector<char> to_remove = {'"','!','?',';',':','.','|','#',','};
            std::vector<char>::iterator chars;
            for(chars = to_remove.begin();chars!=to_remove.end();chars++){
                tweet.erase(remove(tweet.begin(),tweet.end(),*chars),tweet.end()); // remove all single charactes in the to_remove vector
            } 

            tweet_ss.str(tweet); // sync tweet_ss with tweet after single chars are removed

            int next_loop_stream_start_pos = 0;
            tweet_ss.seekg(next_loop_stream_start_pos); // reset reading of the ss of the tweet
            
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
                            new_gram.second = 0;
                            ngrams_of_tweet_text.insert(new_gram);
                        }
                    } // gram added to the map or incremented the occurence
                }// creates the gram with the proper number of strings or untill we run out of words in the tweet
                gram="";// reset gram for next one to be produced.
                tweet_ss.seekg(next_loop_stream_start_pos);
            }
        }
    }
    void output_map(std::ostream& out,int num_occurences){ //// output map
        std::map<std::string,int>:: iterator mapit;
        for(mapit = ngrams_of_tweet_text.begin(); mapit!= ngrams_of_tweet_text.end(); mapit++ ){
            if(mapit->second > num_occurences ) 
                out << mapit->first << " " << mapit->second << std::endl;
            } 
            /////////////// 
    }

};

int main(int argc,char *argv[]){
    int nth_gram = 0;
    if (argc == 2){
        nth_gram = atoi(argv[1]);
    }else{
        std::cout << "Error Usage: mapper <int> Please rerun." << std::endl;
        assert(-1);
    }
    twittercsvparser twitterfile;
    twitterfile.nth_gram(nth_gram); // sets what string size we will be looking for

    twitterfile.load_data(std::cin);// can take any ifstream so could potentiall use if stream to load multiple files into the parser.
    twitterfile.load_tweet_text_map();
    twitterfile.output_map(std::cout,100000); // only output if has over 1000 occurences

}