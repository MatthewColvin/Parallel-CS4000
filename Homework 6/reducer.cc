// 
//          Homework 6
//          Matthew Colvin 
//          Reducer pourpose : Take from standard input multiple mapper
//                             outputs to map them into a different input 
//          
//             
//
#include <iostream>
#include <iomanip>
#include <map>

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
    for(auto mapit:output){
        if(mapit.second > num_occurences ){ 
            out << mapit.first << " , " << mapit.second << std::endl;
        } 
    }  
}    

int main (int argc, char* argv[]){
    std::map<std::string,int> reducedmap =
    reduce_map(std::cin); 
    output_map(std::cout,reducedmap,1000); // output all n-grams that occured more than 1000 times.
    return(0);
}