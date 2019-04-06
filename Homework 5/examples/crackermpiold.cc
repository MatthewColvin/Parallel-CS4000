#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<unistd.h>
#include<crypt.h>
#include<sstream>
#include<cstring>
#include<omp.h>
#include<mpi.h> 
#define ROOT 0 // proccess 0 is root  // #TODO make this variable in program.

using namespace std;
class passcracker{
    public:
        
        void get_mpi_info(){
            // Get the number of processes
            MPI_Comm_size(MPI_COMM_WORLD, &world_size);
            // Get the rank of the process
            MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
            // Get the name of the processor
            MPI_Get_processor_name(processorName, &name_len);
        }

        void load_vectors(int argc,char*argv[]){
        
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
            for(int t=0;t < diconary.size();t++){
                if(diconary[t].length() > maxPasswordLength){
                maxPasswordLength = diconary[t].length() +10;  // the plus 10 to allow for the number in the proccess and other data

                }
            }
        }

        void listen(){ // this function will be called on root proccess only and will manage other proccesses sendingssss
            if(world_rank == ROOT){ 
                bool proccessDone[world_size-1]={false};// array to hold when each procces is finishing 
                // case 0 proccess is not done 
                // case 1 proccess is done
                while (!donerecievingmessages(proccessDone)){
                    cout << "Waiting for message..." <<endl;
                    int count = 100;
                    char messages[count];
                    MPI_Recv(messages,count,MPI_CHAR,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
                    stringstream ss(messages);
                    int from_proccess;
                    string recieved;
                    ss >> from_proccess >> recieved; 
                    //need to proccess string to tell which proccess its from.
                    if (recieved == "non-diconary-word"){
                        proccessDone[from_proccess-1] = true;
                        cout << "Proccess "<< from_proccess << " is done!!" <<endl;
                    }else{
                        cout << "Found a password!!   " << "RECIEVED from Procces #" << from_proccess << " Password:" << recieved << endl;
                    }
                    

                }
            }    
                
            
        }

        bool donerecievingmessages(bool proccessdone[]){
            for(int i=0; i < world_size-1; i++){
                if(!proccessdone[i]){return false;}// if all of the proccesses are not done then return false because all the proccesses are not done.
            }
            return true;
        }

        void attack(){
            if(world_rank!=ROOT){// root will not attack
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



                // use i to divide the work in the proccesses.
                
                for (int i=0; i<diconary.size(); i++){ 
                    if ((i % (world_size-1))+1 == world_rank){ // world_size-1 because the proccessing elemnts are all but root.    
                        pass(diconary[i]); // word
                        for(int a=0; a<single_digit.size();a++){
                            pass(diconary[i]+single_digit[a]); // word + 1digit
                            pass(single_digit[a]+diconary[i]); // 1digit + word
                        }
                        for(int b=0; b<double_digit.size(); b++){
                            pass(diconary[i]+double_digit[b]); //word + 2digit
                            pass(double_digit[b]+diconary[i]); // 2digit + word
                        }
                        for(int c=0; c<triple_digit.size(); c++){
                            // rhino005 password  // if(world_rank==9 && triple_digit[c]== "005"){ cout << "This should be it !!! :"<< diconary[i]+triple_digit[c] << endl;}
                            pass(diconary[i]+triple_digit[c]); // word + 3digit
                            pass(triple_digit[c]+diconary[i]); // 3digit + word
                        }
                        // second word cases taken out of assignment
                        // for (int j=0; j<diconary.size(); j++){ // add second word 
                        //     pass(diconary[i]+diconary[j]); // word + word
                        //     for(int a=0; a<single_digit.size();a++){
                        //         pass(diconary[i]+single_digit[a]+diconary[j]); // word1 + 1digit + word2
                        //         pass(diconary[i]+diconary[j]+single_digit[a]); // word1 + word2 + 1digit
                        //         pass(single_digit[a]+diconary[i]+diconary[j]); // 1digit + word1 + word2
                        //     }
                        //     for(int b=0; b<double_digit.size(); b++){
                        //         pass(diconary[i]+double_digit[b]+diconary[j]); // word + 2digit + word2
                        //         pass(diconary[i]+diconary[j]+double_digit[b]); // word1 + word2 + 2digit
                        //         pass(double_digit[b]+diconary[i]+diconary[j]); // 2digit + word1 + word2
                        //     }
                        //     for(int c=0; c<triple_digit.size(); c++){
                        //         pass(diconary[i]+triple_digit[c]+diconary[j]); // word + 3digit + word2
                        //         pass(diconary[i]+diconary[j]+triple_digit[c]); // word1 + word2 + 3digit
                        //         pass(triple_digit[c]+diconary[i]+diconary[j]); // 3digit + word1 + word2
                        //     }
                        // }
                    }else{ } // do nothing because another proccess will handle it...
                }
            }
        }

        void make_test_cases(){
            vector<string> testpasses = {"ant123","464roach","78bug","99whale","783dog","fish67","rhino005"};
            for(int i=0; i < testpasses.size(); i++){
                if(world_rank==2)
                cout << crypt(testpasses[i].c_str(),salts[i].c_str()) << endl;
            }

        }
        
        void end(){
            if(world_rank!= ROOT ){
                //cout << "Proccess #" << world_rank << "is about to send the end wordl" << endl;
                const string keyNotInDiconary = "non-diconary-word";
                send_message(keyNotInDiconary); // this will hopefully allow proccess 1 to know when it is done.
            }
            if(world_rank == ROOT){
                cout << "The Root proccess has made it to the end function were in the clear." <<endl;
            }
        }

    private: 

        void send_message(string note){
            //cout << "preproccessed message: " << note << endl;
            stringstream ss;
            ss << world_rank << "  " << note; // put the proccess number firs
            //cout << "proccessed ss.str " << ss.str() << endl;
            string tmp = ss.str();
            //cout << "proccessed tmp.c_str()" << tmp.c_str() << endl; 
            const char* preparedMessage = tmp.c_str(); // ERROR watch!! 
            /////////////////cout << "sending message:"<< preparedMessage << endl;
            int count = 100; // it will only take buffers with a static size i tried to use maxPasswordSize
            MPI_Send(preparedMessage,count,MPI_CHAR,ROOT,0,MPI_COMM_WORLD);
        }


        void pass(string generated){
            for(int i=0; i < salts.size(); i++){
                if(0 == strcmp(encrypted[i].c_str(), crypt(generated.c_str(),salts[i].c_str()))){ // we found the password matched the encrypted password
                    if(world_rank!=ROOT){ //if it is the root proccess 
                        cout <<"rank:" << world_rank  << " found password: " << generated << endl; 
                        send_message(generated); // send it to the root so it can be displayed
                    }
                }
                //cout << "You found Password " << i << " at proccess " << world_rank << " it is " << generated << endl;
            }
        }

        // mpi information //////////////////////////////
        int world_size;
        int world_rank;
        char processorName[MPI_MAX_PROCESSOR_NAME];
        int name_len;
        ////////////////////////////////////////////////
        int maxPasswordLength=-1;
        vector <string> salts , diconary, encrypted;

};

// implement something for when one proccess finds a password to send that to the local machine and print it.

int main(int argc, char** argv){
    
    MPI_Init(NULL,NULL);
    passcracker crack;

    crack.get_mpi_info();
    crack.load_vectors(argc,argv);
    //crack.make_test_cases();
    
    crack.listen();
    
    crack.attack();
    
    crack.end(); // proccess ROOT acts as input loop and barrier.
    //It handles results the proccess that have been spawned.

    MPI_Finalize();

}