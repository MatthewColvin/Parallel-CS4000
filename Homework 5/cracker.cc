#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<unistd.h>
#include<crypt.h>
#include<sstream>
#include<cstring>
#include<omp.h>

using namespace std;

void pass(string generated,vector<string> &salts,vector<string> &encrypted_passwords){
    for(int i=0; i < salts.size(); i++){
            if(0 == strcmp(encrypted_passwords[i].c_str(), crypt(generated.c_str(),salts[i].c_str()))){
                cout << "You found Password " << i << " it is " << generated << endl;
            }
    }
}


int main(int argc, char** argv){
    ifstream ins_salts, ins_diconary, ins_encrypted;
    
    if(argc == 1){
        ins_salts.open("given/salts");
        ins_diconary.open("given/words");
        ins_encrypted.open("given/enc_passwords");
    }
    if(argc > 3){
        ins_salts.open(argv[1]);
        ins_diconary.open(argv[2]);
        ins_encrypted.open(argv[3]);
    }
    // else{
    //     string tmp;
    //     cout << "Please enter name of salts file" << endl;
    //     cin >> tmp;
    //     ins_salts.open("given/salts");
    //     cout << "Please enter name of Diconary file" << endl;
    //     cin >> tmp;
    //     ins_diconary.open("given/words");
    //     cout << "Please enter name of Encrypted password file"<<endl;
    //     cin >> tmp;
    //     ins_encrypted.open("given/enc_passwords");
    // }


    vector <string> salts , diconary, encrypted ;
    string tmp;
    ins_salts >>tmp;
    while (!ins_salts.eof()){
        salts.push_back(tmp);
        ins_salts >> tmp;
    }
    ins_diconary >> tmp;
    while (!ins_diconary.eof()){
        diconary.push_back(tmp);
        ins_diconary >> tmp;
    }
    ins_encrypted >> tmp;
    while (!ins_encrypted.eof()){
        encrypted.push_back(tmp);
        ins_encrypted >> tmp;
    }

    ins_salts.close();
    ins_diconary.close();
    ins_encrypted.close();


    stringstream sstmp;
    vector<string> single_digit; // made to hold single digits 
    vector<string> double_digit; // made to hold double digit numbers
    vector<string> triple_digit; // mode to hold triple digit numbers
    for(int a=0; a<10; a++){
        sstmp << a;
        single_digit.push_back(sstmp.str());
        sstmp.str(string()); // clear 
    }
    for(int b=0; b<100; b++){
        if(b<10){sstmp << 0 << b;}
        else{sstmp << b;}
        double_digit.push_back(sstmp.str());
        sstmp.str(string());
    }
    for (int c=0; c<1000; c++){
        if(c < 10){sstmp << 0 << 0 << c;}
        else if (c < 100){sstmp << 0 << c;}
        else{sstmp << c;}
        triple_digit.push_back(sstmp.str());
        sstmp.str(string());   
    }

    #pragma omp parallel for
    for (int i=0; i<diconary.size(); i++){ 
        pass(diconary[i],salts,encrypted); // word
        for(int a=0; a<single_digit.size();a++){
            pass(diconary[i]+single_digit[a],salts,encrypted); // word + 1digit
            pass(single_digit[a]+diconary[i],salts,encrypted); // 1digit + word
        }
        for(int b=0; b<double_digit.size(); b++){
            pass(diconary[i]+double_digit[b],salts,encrypted); //word + 2digit
            pass(double_digit[b]+diconary[i],salts,encrypted); // 2digit + word
        }
        for(int c=0; c<triple_digit.size(); c++){
            pass(diconary[i]+triple_digit[c],salts,encrypted); // word + 3digit
            pass(triple_digit[c]+diconary[i],salts,encrypted); // 3digit + word
        }
        // second word cases taken out of assignment
        // for (int j=0; j<diconary.size(); j++){ // add second word 
        //     pass(diconary[i]+diconary[j],salts,encrypted); // word + word
        //     for(int a=0; a<single_digit.size();a++){
        //         pass(diconary[i]+single_digit[a]+diconary[j],salts,encrypted); // word1 + 1digit + word2
        //         pass(diconary[i]+diconary[j]+single_digit[a],salts,encrypted); // word1 + word2 + 1digit
        //         pass(single_digit[a]+diconary[i]+diconary[j],salts,encrypted); // 1digit + word1 + word2
        //     }
        //     for(int b=0; b<double_digit.size(); b++){
        //         pass(diconary[i]+double_digit[b]+diconary[j],salts,encrypted); // word + 2digit + word2
        //         pass(diconary[i]+diconary[j]+double_digit[b],salts,encrypted); // word1 + word2 + 2digit
        //         pass(double_digit[b]+diconary[i]+diconary[j],salts,encrypted); // 2digit + word1 + word2
        //     }
        //     for(int c=0; c<triple_digit.size(); c++){
        //         pass(diconary[i]+triple_digit[c]+diconary[j],salts,encrypted); // word + 3digit + word2
        //         pass(diconary[i]+diconary[j]+triple_digit[c],salts,encrypted); // word1 + word2 + 3digit
        //         pass(triple_digit[c]+diconary[i]+diconary[j],salts,encrypted); // 3digit + word1 + word2
        //     }
        // }
    }



}