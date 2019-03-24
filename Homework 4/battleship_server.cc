///////////////////////////////////////////////
//
//      Matthew Colvin
//      server side of a battleship game
//
//
//
//
///////////////////////////////////////////////////
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <boost/asio.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/algorithm/string.hpp>
#include <stdio.h>
#include <stdlib.h>
#define SHIP 3 
#define HIT 2
#define MISS 1
#define BLANK 0

using namespace std;
using boost::asio::ip::tcp;

class battleship_server{    
    public:
        vector<vector<int>> string_to_board(string string_board){
            vector<vector<int>> board;
            vector<string> vectors, elements;
            vector<int> int_tmp;
            
            stringstream ss;

            boost::split(vectors,string_board,[](char c){return c == ':';});
            for (int i=0 ; i < vectors.size();i++){
                boost::split(elements,vectors[i],[](char c){return c == ',';});
                for(int j =0; j < elements.size();j++){
                   int_tmp.push_back(atoi(elements[j].c_str()));
                }
                board.push_back(int_tmp);
                int_tmp.clear();
            }

            return board;
            
        /*
            stringstream board_stream ,vector_stream ,element_stream; // streams for parsing 
            
            board_stream.str(string_board); 
            int x = 0;
            int i=0;
            string vector ,element;

            while(getline(board_stream,vector,':')){
            vector_stream.clear();
            vector_stream.str(vector);
            if(vector.empty()){break;} // the last one has been empty messing up results
            cout <<" vector" << i << ": " << vector << endl;
            while(getline(vector_stream,element,',')){
                    element_stream.str(element);
                    element_stream >> x;
                    tmp.push_back(x);
            }   
            board.push_back(tmp);
            tmp.clear();
            i++;
            }
            
            return board;

        */ 
        }
        
        bool game_over(boost::shared_ptr<tcp::socket> socket1,boost::shared_ptr<tcp::socket> socket2){
            bool p1wins = true, p2wins = true;

            for(int i = 0; i< Player1board.size() ; i++){
                if(!p1wins){break;}
                for (int j=0; j< Player1board[i].size();j++){
                    if(Player1board[i][j] == BLANK){
                        p1wins = true;
                    }else{
                        p1wins = false;
                        break;
                    }
                }
            }//if the board is completely blank then play2wins

            for(int i = 0; i< Player2board.size() ; i++){
                if(!p2wins){break;} 
                for (int j=0; j< Player2board[i].size();j++){
                    if(Player2board[i][j] == BLANK){
                        p2wins = true;
                    }else{
                        p2wins = false;
                        break;
                    }   
                }
            } // if the board is completely blank then p1 wins

            if (p1wins && !p2wins){
                boost::asio::write(*socket1,boost::asio::buffer(("1\n")));
                boost::asio::write(*socket2,boost::asio::buffer(("2\n")));

            }else
            if (p2wins && !p1wins){
                boost::asio::write(*socket2,boost::asio::buffer(("1\n")));
                boost::asio::write(*socket1,boost::asio::buffer(("2\n")));
            }else
            if (p1wins && p2wins){
                boost::asio::write(*socket2,boost::asio::buffer(("3\n")));
                boost::asio::write(*socket1,boost::asio::buffer(("3\n")));
            }else
            if (!p1wins && !p2wins){
                boost::asio::write(*socket2,boost::asio::buffer(("0\n")));
                boost::asio::write(*socket1,boost::asio::buffer(("0\n")));
            }
            
            return (p1wins || p2wins); // if either person wins game is over
        }

        void RecieveP1board(boost::shared_ptr<tcp::socket> socket){
            boost::asio::streambuf buf;
            boost::asio::read_until(*socket, buf, "\n" );
            string player1_string_board = boost::asio::buffer_cast<const char*>(buf.data());
            Player1board = string_to_board(player1_string_board); 
            cout << "Player1: " << player1_string_board;
        }
        void RecieveP2board(boost::shared_ptr<tcp::socket> socket){
            boost::asio::streambuf buf;            
            boost::asio::read_until(*socket, buf, "\n" );
            string player2_string_board = boost::asio::buffer_cast<const char*>(buf.data());
            Player2board = string_to_board(player2_string_board);
            cout << "Player2: " << player2_string_board;

        }

        void print_boards () {
            cout << "Player 1 Board:" << endl;
            print_board(Player1board);
            
            cout << endl << "Player 2 Board: " << endl;
            print_board(Player2board);
        }

        void send_move(string move,boost::shared_ptr<tcp::socket> socket){
            string out = move + '\n';
            boost::asio::write(*socket, boost::asio::buffer(out));
        }    

        string Recievemove(boost::shared_ptr<tcp::socket> socket){
            boost::asio::streambuf buf;
            boost::asio::read_until(*socket, buf, "\n" );
            string move = boost::asio::buffer_cast<const char*>(buf.data());
            return (move);
        }

        void swap_usernames(boost::shared_ptr<tcp::socket> socket1,boost::shared_ptr<tcp::socket> socket2){
            boost::asio::streambuf buf1,buf2;

            boost::asio::read_until(*socket1,buf1,"\n");
            boost::asio::read_until(*socket2,buf2,"\n");
            string P1Username = boost::asio::buffer_cast<const char*>(buf1.data());
            string P2Username = boost::asio::buffer_cast<const char*>(buf2.data());
            P1Username+= "\n";
            P2Username+= "\n";
            cout << "Player1 :" << P1Username << "Player2 : " << P2Username <<  endl ;
            boost::asio::write(*socket1,boost::asio::buffer(P2Username));
            boost::asio::write(*socket2,boost::asio::buffer(P1Username));

        }

        void make_moves(string P1move,string P2move){
            vector<string> move;

            int P1x,P1y,P2x,P2y;
            boost::split(move,P1move,[](char c){return c == ',';});
            P1x = atoi(move[0].c_str());
            P1y = atoi(move[1].c_str());
            boost::split(move,P2move,[](char c){return c == ',';});
            P2x = atoi(move[0].c_str());
            P2y = atoi(move[1].c_str());
 
            switch (Player1board[P2x][P2y]){
                case SHIP:
                    P2moveresult = HIT;
                    Player1board[P2x][P2y] = BLANK;
                    break;
                case BLANK:
                    P2moveresult = MISS;
                    break;
            }

            switch (Player2board[P1x][P1y]){
                case SHIP:
                    P1moveresult = HIT;
                    Player2board[P1x][P1y] = BLANK;
                    break;
                case BLANK:
                    P1moveresult = MISS;
                    break;
            }
            
        }

        void send_results(boost::shared_ptr<tcp::socket> socket1,boost::shared_ptr<tcp::socket> socket2){
            stringstream ss,ss1;
            ss << P1moveresult << '\n';
            boost::asio::write(*socket1,boost::asio::buffer(ss.str()));
            ss1 << P2moveresult << '\n' ;
            boost::asio::write(*socket2,boost::asio::buffer(ss1.str()));
        }


    private:
        vector<vector<int>> Player1board;
        vector<vector<int>> Player2board;
        int P1moveresult,P2moveresult;
    

        void print_board(vector<vector<int>>& board){
            for (int i=0 ;i < board.size(); i++){
                for (int j=0 ; j< board[i].size();j++){
                    cout<< board[i][j] << ' ' ;
                }
                cout<<endl;
            }
        }

};

int main(){

    boost::asio::io_service my_service;
    boost::shared_ptr<tcp::socket> socket1(new tcp::socket(my_service));
    boost::shared_ptr<tcp::socket> socket2(new tcp::socket(my_service));
    tcp::acceptor acceptor(my_service, tcp::endpoint(tcp::v4(), 4700));
    acceptor.accept(*socket1);
    acceptor.accept(*socket2);      
            
    battleship_server battle_server;
    
    battle_server.swap_usernames(socket1,socket2);

    battle_server.RecieveP1board(socket1);
    battle_server.RecieveP2board(socket2);

    battle_server.print_boards();

    string P1move,P2move;


    while (!battle_server.game_over(socket1,socket2)){
        
        usleep(100);
        P1move = battle_server.Recievemove(socket1);
        cout << " player 1 move:"<<P1move <<endl ;

        usleep(100);
        P2move = battle_server.Recievemove(socket2);
        cout << " player 2 move:"<<P2move <<endl ;


        usleep(100);

        battle_server.make_moves(P1move,P2move);
        usleep(100);
        battle_server.send_results(socket1,socket2);


        usleep(100);

        battle_server.send_move(P1move,socket2);
        usleep(100);
        battle_server.send_move(P2move,socket1);
        usleep(100);

        //i have no idea why i needed to put the sleep in between but withough them this program will not run


    }




    return 0;
}