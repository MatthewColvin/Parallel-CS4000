#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<unistd.h>
#include<crypt.h>
#include<sstream>

using namespace std;

int main(int argc, char** argv){

    // ifstream ins_salts, ins_diconary;
    
    // if(argc > 2){
    //     ins_salts.open(argv[1]);
    //     ins_diconary.open(argv[2]);
    // }else{
    //     string tmp;
    //     cout << "Please enter name of salts file" << endl;
    //     cin >> tmp;
    //     ins_salts.open(tmp);
    //     cout << "Please enter name of Diconary file" << endl;
    //     cin >> tmp;
    //     ins_diconary.open(tmp);
    // }


    vector <string> salts , diconary= {"oneword"} ;
    // while (!ins_salts.eof()){
    //     string tmp;
    //     ins_salts >> tmp;
    //     salts.push_back(tmp);
    // }
    // while (!ins_diconary.eof()){
    //     string tmp;
    //     ins_diconary >> tmp;
    //     diconary.push_back(tmp);
    // }

    

    //Generate passwords
    vector <string> possiblepasswords;
    string tmp;
    stringstream ss;
    for (int i=0 ; i< diconary.size(); i++){
        possiblepasswords.push_back(diconary[i]); // just one word no number
        for (int o=0;o<diconary.size();o++){
            tmp = diconary[i] + diconary[o]; // diconary word + diconary word no numbbers
            possiblepasswords.push_back(tmp); 
        }
        for(int j=0;j<10;j++){
            ss << j;
            tmp = diconary[i] + ss.str(); // create a dictonary word with a number after it;
            possiblepasswords.push_back(tmp); // put in possible password vector
            for (int q=0;q<diconary.size();q++){
                possiblepasswords.push_back(tmp + diconary[q]); // append another diconary word to (word + single digit) 
            }
            ss.str(string());// clear stream for next password
        }
        for(int k=0;k<100;k++){
            if(k < 10) { // k is less than 10 so we need to put a zero in front of the single diget ; 
                ss << 0 << k;
            }else{      // k is double diget
                ss << k;
            }
            tmp = diconary[i] + ss.str(); // dicontary word + 2 digit number 
            possiblepasswords.push_back(tmp);
            for(int t=0;t<diconary.size();t++){
                possiblepasswords.push_back(tmp + diconary[t]); // dictonary word + 2 digit number + diconary word
            }
            ss.str(string());// clear stream for next password
        }
        for(int l=0;l<1000;l++){
            if(l<10){
                ss << 0 << 0 << l; // single digit so need to append 2 zeros 
            }else
            if(l<100){ // double digit so need to append 1 zero 
                ss << 0 << l ;
            }else{ 
                ss << l;
            }
            tmp = diconary[i] + ss.str();  // diconary word + 3 digit number 
            possiblepasswords.push_back(tmp); 
            for (int u=0; u < diconary.size();u++){
                possiblepasswords.push_back(tmp + diconary[u]); // diconary word + 3 digits + diconary word
            }
            ss.str(string());
        }
    }




    cout << possiblepasswords.size();
   
    string t;

    string salt = "$6$mk3DMfWk3hwXSva1";
    string pass = "dumbpassword";
    t = crypt(pass.c_str(),salt.c_str());

    cout << t << endl;

}