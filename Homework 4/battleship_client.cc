///////////////////////////////////////////////
//
//      Matthew Colvin
//      Client side of a battleship game
//
//
//
//
///////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <string>
#include <boost/asio.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/algorithm/string.hpp>
#include <algorithm>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#define MYBOARD 1
#define DECBOARD 2
#define USED 4
#define SHIP 3 
#define HIT 2
#define MISS 1
#define BLANK 0

using namespace std;
using boost::asio::ip::tcp;

class boat{

    public:
        vector<pair<int,int>> coordinates;//(row,colum)(first,second)
        
        boat(){
            board_size=0;
        }
        boat(vector<pair<int,int>> c ,int BoardSize){
            board_size = BoardSize; 
            coordinates = c;
            front = coordinates[0]; // first coordinate is always the front of the ship
        }

        void set_board_size(int size){
            board_size= size;
        }
        void set_coordinates(vector<pair<int,int>> c){
            coordinates = c;
            front = coordinates[0];
        }

        
        bool is_facing_north(){
            bool ans = false;
            for(int i=0; i<size()-1; i++){
                if(coordinates[i].second == coordinates[i+1].second){//all in the same col
                    if(coordinates[i].first < coordinates[i+1].first){// the x coordiante will greater for the front if facing north
                        ans = true;
                    }
                }else{
                    ans = false;
                    break;
                }
            }
            return ans;  

        }
        bool is_facing_south(){
            bool ans = false;
            for(int i=0; i<size()-1; i++){
                if(coordinates[i].second == coordinates[i+1].second){//all in the same col
                    if(coordinates[i].first > coordinates[i+1].first){// the x coordiante will less for the front if facing south
                        ans = true;
                    }
                }
            }
            return ans;  

        }
        bool is_facing_east(){
              bool ans= false;
            for(int i=0; i<size()-1; i++){
                if(coordinates[i].first == coordinates[i+1].first){//all in the same row
                    if(coordinates[i].second > coordinates[i+1].second){// the y coordiante will be greater for the front if facing east
                        ans = true;
                    }
                }else{
                    ans = false;
                    break;
                }
            }
            return ans;  

        }
        bool is_facing_west(){
              bool ans= false;
            for(int i=0; i<size()-1; i++){
                if(coordinates[i].first == coordinates[i+1].first){//all in the same row
                    if(coordinates[i].second < coordinates[i+1].second){// the y cordinate will be less if facing west
                        ans = true;
                    }
                }else{
                    ans = false;
                    break;
                }
            }
            return ans;  
            
        }
        bool is_facing_north_east(){
             bool ans= false;
            for(int i=0; i<size()-1; i++){
                if(coordinates[i].first < coordinates[i+1].first){
                    if(coordinates[i].second > coordinates[i+1].second){
                        ans = true;
                    }
                }else{
                    ans = false;
                    break;
                }
            }
            return ans;  
        }
        bool is_facing_north_west(){
             bool ans= false;
            for(int i=0; i<size()-1; i++){
                if(coordinates[i].first < coordinates[i+1].first){
                    if(coordinates[i].second < coordinates[i+1].second){
                        ans = true;
                    }
                }else{
                    ans = false;
                    break;
                }
            }
            return ans;  
        }
        bool is_facing_south_east(){
             bool ans= false;
            for(int i=0; i<size()-1; i++){
                if(coordinates[i].first > coordinates[i+1].first){
                    if(coordinates[i].second > coordinates[i+1].second){
                        ans = true;
                    }
                }else{
                    ans = false;
                    break;
                }
            }
            return ans;  
        }
        bool is_facing_south_west(){
             bool ans= false;
            for(int i=0; i<size()-1; i++){
                if(coordinates[i].first > coordinates[i+1].first){
                    if(coordinates[i].second < coordinates[i+1].second){
                        ans = true;
                    }
                }else{
                    ans = false;
                    break;
                }
            }
            return ans;  
        }

        void face_north(){
            int shipsize = size();
            coordinates.clear(); // destroy ship 
            for (int j =0; j < shipsize ; j++){
                coordinates.push_back(pair<int,int>(front.first+j,front.second));
            }
            
        }
        void face_south(){
            int shipsize = size();
            coordinates.clear(); // destroy ship 
            for (int j =0; j < shipsize ; j++){
                coordinates.push_back(pair<int,int>(front.first-j,front.second));
            }
        }
        void face_east(){
            int shipsize = size();
            coordinates.clear(); // destroy ship 
            for (int j =0; j < shipsize ; j++){
                coordinates.push_back(pair<int,int>(front.first,front.second-j));
            }
        }
        void face_west(){
             int shipsize = size();
            coordinates.clear(); // destroy ship 
            for (int j =0; j < shipsize ; j++){
                coordinates.push_back(pair<int,int>(front.first,front.second+j));
            }
        }
        void face_north_east(){
            int shipsize = size();
            coordinates.clear(); // destroy ship 
            for (int j =0; j < shipsize ; j++){
                coordinates.push_back(pair<int,int>(front.first+j,front.second - j));
            }
        }
        void face_north_west(){
            int shipsize = size();
            coordinates.clear(); // destroy ship 
            for (int j =0; j < shipsize ; j++){
                coordinates.push_back(pair<int,int>(front.first+j,front.second+j));
            }
        }
        void face_south_east(){
            int shipsize = size(); 
            coordinates.clear(); // destroy ship 
            for (int j =0; j < shipsize ; j++){
                coordinates.push_back(pair<int,int>(front.first-j,front.second-j));
            }
        }
        void face_south_west(){
            int shipsize = size();
            coordinates.clear(); // destroy ship 
            for (int j =0; j < shipsize ; j++){
                coordinates.push_back(pair<int,int>(front.first-j,front.second+j));
            }
        }

        int size(){ // returns the size of the boat
            return (coordinates.size());
        }

        bool is_on_board(){
            for (int i=0;i<size(); i++){
                if(coordinates[i].first >= board_size){
                    return false;
                }
                if(coordinates[i].second >= board_size){
                    return false;
                }
                if(coordinates[i].first < 0){
                    return false;
                }
                if(coordinates[i].second < 0){
                    return false;
                }
            }
            return true;
        }

        void rotate_clkwize(){
            do{
                if (is_facing_north()){
                    face_north_east();
                }else
                if(is_facing_north_east()){
                    face_east();
                }else
                if(is_facing_east()){
                    face_south_east();
                }else
                if(is_facing_south_east()){
                    face_south();
                }else
                if(is_facing_south()){
                    face_south_west();
                }else
                if(is_facing_south_west()){
                    face_west();
                }else
                if(is_facing_west()){
                    face_north_west();
                }else
                if(is_facing_north_west()){
                    face_north();
                }  
            }while(!is_on_board()); // while the boat is not on the board keep rotating 

        }

        void rotate_coutnerclkwize(){
            do{
                if (is_facing_north()){
                    face_north_west();
                }else 
                if(is_facing_north_west()){
                    face_west();
                }else
                if(is_facing_west()){
                    face_south_west();
                }else
                if(is_facing_south_west()){
                    face_south();
                }else
                if(is_facing_south()){
                    face_south_east();
                }else
                if(is_facing_south_east()){
                    face_east();
                }else
                if(is_facing_east()){
                    face_north_east();
                }else
                if(is_facing_north_east()){
                    face_north();
                }  
            }while(!is_on_board()); // while the boat is not on the board keep rotating 
        }

    private:
        int board_size; // boat needs to know size of board to check legality 
        pair<int,int> front; 
};


class battleship_client{
    
    public:

    string Recievemove(boost::shared_ptr<tcp::socket> socket){
            boost::asio::streambuf buf;
            boost::asio::read_until(*socket, buf, "\n" );
            string move = boost::asio::buffer_cast<const char*>(buf.data());
            return (move);
    }

    string board_to_string(vector<vector<int>> &board){
        stringstream str;

        for(int i=0 ; i< board.size(); i++){
            for (int j =0 ; j < board[i].size() ;j++){
                str << board[i][j];
                if(j != board[i].size()-1) {str << ",";} // dont put , before a : helps with parsing 
            }
            if(i != board.size()-1){ str << ":" ;} // dont put a : at the end of the string
        }
        return(str.str()); // converts string stream back to string 
    }

    void placeship(){
        message = "Welcome to BattleShip Please use the arrow keys to place the front of the ship spacebar to set";
        board.resize(4);
        decision_board.resize(4);
        for(int i=0;i<board.size();i++){
            board[i].resize(4);
            decision_board[i].resize(4);
        }// boards resized to 4X4 



        int rows,cols; // rows and coloms in terminal standared window
        int y_top_left_corner=5,x_top_left_corner=5; // where to print board

        initscr(); // starting ncurses
        clear();
        getmaxyx(stdscr,rows,cols);
        cbreak();
        keypad(stdscr, TRUE);
        
        print();

        cursor_to_myboard();

        place_bow(); // place one peice of the boat
        message = "You may now use arrow keys to rotate \n  left key - counterclockwize\n  right key - clockwize\n  spacebar - setship"  ;
        print();
        refresh();
        finish_boat(); // place the rest of the boat on the same coloumn
        rotate_boat(); // rotate with the left and right keys

    }

    void send_board(boost::shared_ptr<tcp::socket> socket){
        string string_board = board_to_string(board);
        string_board+='\n';
        
        boost::asio::write(*socket, boost::asio::buffer(string_board));
    }

    string get_move(){
        which_board = DECBOARD;
        stringstream ss;
        int ch;
        bool done=false;

        message = "Please Select a Positon to fire on opponents board \n with arrow keys and hit space \n                      \n                      ";
        cur_col = cur_row =0; // set cursor to 0,0 
        print(); // initial print
        while(!done){
            ch = getch();
            switch (ch){
                case ' ': 
                    ss << cur_row << ',' << cur_col;
                    done =true;
                    break;
                case KEY_LEFT:
                    cur_col--;
                    cur_col = (4+cur_col)%4;
                    print();
                    break;
                case KEY_RIGHT:
                    cur_col++;
                    cur_col%=4;
                    print();
                    break;
                case KEY_UP:
                    cur_row--;
                    cur_row = (4+cur_row)%4;
                    print();
                    break;
                case KEY_DOWN:
                    cur_row++;
                    cur_row%=4;
                    print();
                    break;
            }
        }    
        return(ss.str());
    }

    int recieve_result(boost::shared_ptr<tcp::socket> socket){
        boost::asio::streambuf buf;            
        boost::asio::read_until(*socket,buf,'\n');
        string result = boost::asio::buffer_cast<const char*>(buf.data());
        int x = atoi(result.c_str());
        return x;
    }


    void opponent_hit(string move){
        vector<string> move1;
        boost::split(move1,move,[](char c){return c == ',';});
            int x = atoi(move1[0].c_str());
            int y = atoi(move1[1].c_str());

        decision_board[x][y] = HIT; 
    }
    void opponent_miss(string move){
        vector<string> move1;
        boost::split(move1,move,[](char c){return c == ',';});
            int x = atoi(move1[0].c_str());
            int y = atoi(move1[1].c_str());

        decision_board[x][y] = MISS; 
    }

    void opponets_move_board_update(string move){
        vector<string> move1;
        boost::split(move1,move,[](char c){return c == ',';});
            int x = atoi(move1[0].c_str());
            int y = atoi(move1[1].c_str());

        switch (board[x][y]){
            case SHIP: 
                board[x][y] = HIT; // your ship has been hit
            case BLANK:              
                board[x][y] = USED; // they missed the ship just update board
        }
        print();
    }

    bool winner_found(boost::shared_ptr<tcp::socket> socket){
        boost::asio::streambuf buf;            
        boost::asio::read_until(*socket,buf,'\n');
        string result = boost::asio::buffer_cast<const char*>(buf.data());
        int x = atoi(result.c_str());
        switch (x){
            case 3:
                endgamemessage = "YOU TIE";
                return true;
                break;
            case 2:
                endgamemessage = "YOU WIN";
                return true;
                break;
            case 1:
                endgamemessage = "YOU LOSE";
                return true;
                break;
            case 0: 
                return false;
        }
        return false;
    }

    bool confirm_move(string move){
        vector<string> check;
        int x,y;
        boost::split(check,move,[](char c){return c == ',';});
            x = atoi(check[0].c_str());
            y = atoi(check[1].c_str());

        switch(decision_board[x][y]){
            case USED:
            case MISS:
            case HIT:
                message = "Please select a move that has not been selected \n                                           \n                                       \n";
                return false;
            case BLANK:
                return true;
        }
        return false;
    }

    void send_move(string move,boost::shared_ptr<tcp::socket> socket){
        string out = move + '\n';
        boost::asio::write(*socket, boost::asio::buffer(out));
    }     


    void usernames(string username,boost::shared_ptr<tcp::socket> socket){
        user_name = username;
        boost::asio::write(*socket, boost::asio::buffer(user_name + '\n'));
        
        boost::asio::streambuf buf;            
        boost::asio::read_until(*socket,buf,'\n');
        other_players_username = boost::asio::buffer_cast<const char*>(buf.data());
        other_players_username.erase(std::remove(other_players_username.begin(), other_players_username.end(), '\n'), other_players_username.end());
    }
    

    void result (){
        cout << endgamemessage <<endl;
    }
    private:
    
    int cur_row=0,cur_col=0; //current row and coloumn that cursor is on 

    vector<vector<int>> decision_board;
    int dboard_start_x = 20, dboard_start_y = 7 ;  // the cordiantes for your decision board to be printed at top left corner

    vector<vector<int>> board;
    int board_start_x = 0, board_start_y = 7 ; // the cordiantes for your board to be printed at top left corner

    boat myboat;  // used to keep track of turning boat
    string message; // used to display messages to user
    string endgamemessage; // message displayed at the end

    int which_board = MYBOARD; // start by editing the origional board
    string user_name;
    string other_players_username;


    

    void cursor_to_myboard(){
        which_board = MYBOARD;
    }
    void cursor_to_decison_board(){
        which_board = DECBOARD;
    }


    void update_ship_on_board(){
        for(int q=0; q<board.size(); q++){
            for(int w=0; w < board[q].size();  w++){
                board[q][w] = 0 ; 
            }
        }
        for(int i=0 ; i < myboat.size() ;i++){
            board[myboat.coordinates[i].first][myboat.coordinates[i].second] = SHIP;
        }
    }// this function clears the board and places the ship dictated by the myboat

    void print() {
        move(0,0);
        printw("BattleShip");

        move(1,0);
        printw(message.c_str());

        move(board_start_y-1,board_start_x);
        printw((user_name + "'s" + " Board:").c_str());

        move (dboard_start_y-1,dboard_start_x); // move cursor above opponents board
        printw((other_players_username + "'s" + " Board:").c_str());
        

        for (int j=0;j<4;j++) {
            move(board_start_y+0,board_start_x+2*j);
            printw("+-");
            }
            move(board_start_y+0,board_start_x+2*4);
            printw("+");
        for (int i=0;i<4;i++) {
            for (int j=0;j<4;j++) {
            move(board_start_y+2*i+1,board_start_x+2*j);
            printw("|");
            move(board_start_y+2*i+1,board_start_x+2*j+1);
            switch(board[i][j]){
                case BLANK:
                    printw(" "); // dont know yet what will happen
                    break;
                case MISS:
                    printw("M"); // if board is set to 1 then it is a miss 
                    break;
                case HIT:
                    printw("H"); // if board is set to 2 then it is a hit
                    break;
                case SHIP:
                    printw("X"); // 3 will indicate the ship is there.
                    break;
                case USED:
                    printw("-"); // the spot has been guessed
            }
            move(board_start_y+2*i+1,board_start_x+2*4);
            printw("|");
            for (int j=0;j<4;j++) {
            move(board_start_y+2*i+2,board_start_x+2*j);
            printw("+-");
            }
            move(board_start_y+2*i+2,board_start_x+2*4);
            printw("+");
        }
        }

        /////////////////////////////////////////////////////////////print the decision board
        

        for (int j=0;j<4;j++) {
            move(dboard_start_y+0,dboard_start_x+2*j);
            printw("+-");
            }
            move(dboard_start_y+0,dboard_start_x+2*4);
            printw("+");
        for (int i=0;i<4;i++) {
            for (int j=0;j<4;j++) {
            move(dboard_start_y+2*i+1,dboard_start_x+2*j);
            printw("|");
            move(dboard_start_y+2*i+1,dboard_start_x+2*j+1);
            switch(decision_board[i][j]){
                case BLANK:
                    printw(" "); // dont know yet what will happen
                    break;
                case MISS:
                    printw("M"); // if board is set to 1 then it is a miss 
                    break;
                case HIT:
                    printw("H"); // if board is set to 2 then it is a hit
                    break;
                case SHIP:
                    printw("X"); // 3 will indicate the ship is there.
                    break;
                case USED:
                    printw("-"); // the spot has been guessed
            }
            move(dboard_start_y+2*i+1,dboard_start_x+2*4);
            printw("|");
            for (int j=0;j<4;j++) {
            move(dboard_start_y+2*i+2,dboard_start_x+2*j);
            printw("+-");
            }
            move(dboard_start_y+2*i+2,dboard_start_x+2*4);
            printw("+");
            }
        }
        
        switch(which_board){
            case MYBOARD:
                move(board_start_y+2*cur_row+1,board_start_x+2*cur_col+1);
                break;
            case DECBOARD:
                move(dboard_start_y+2*cur_row+1,dboard_start_x+2*cur_col+1);
                break;
        }
                
    refresh();   
    }

    void place_bow(){
        int ch;
        bool done = false;
        
        while (!done) {
            ch = getch();
            switch (ch) {
            case ' ':
                board[cur_row][cur_col]=3;
             print();
                done = true;
                break;
            case KEY_RIGHT:
                cur_col++;
                cur_col%=4;
             print();
                break;
            case KEY_LEFT:
                cur_col--;
                cur_col = (4+cur_col)%4;
             print();
                break;
            case KEY_UP:
                cur_row--;
                cur_row=(4+cur_row) % 4;
             print();
                break;
            case KEY_DOWN:
                cur_row++;
                cur_row%=4;
             print();
                break;
            }
        }

        
    }
    
    void finish_boat(){
            vector<pair<int,int>> boats_cooridates;
            boats_cooridates.push_back(pair<int,int>(cur_row,cur_col));
            if (cur_row >= 2){
                board[cur_row-1][cur_col] = SHIP;
                boats_cooridates.push_back(pair<int,int>(cur_row-1,cur_col));
                board[cur_row-2][cur_col] = SHIP;
                boats_cooridates.push_back(pair<int,int>(cur_row-2,cur_col));
            }else{
                board[cur_row+1][cur_col] = SHIP;
                boats_cooridates.push_back(pair<int,int>(cur_row+1,cur_col));
                board[cur_row+2][cur_col] = SHIP;
                boats_cooridates.push_back(pair<int,int>(cur_row+2,cur_col));                
            }
         print();
            myboat.set_coordinates(boats_cooridates);
            myboat.set_board_size(board.size()); // let the boat know where it can go.

    }// finish boat should alwasy be called after place_boat() function so cursor is in correct place.   
       
    void rotate_boat(){
        int ch;
        bool done = false;
        while(!done){
            ch = getch();
            switch(ch){
                case KEY_LEFT:
                    myboat.rotate_coutnerclkwize();
                    update_ship_on_board();
                 print();
                    break;
                case KEY_RIGHT:
                    myboat.rotate_clkwize();
                    update_ship_on_board();
                 print();
                    break;
                case ' ' :
                    done = true; 
                    print();
            }
        }
    }

};




int main(int argc, char *argv[]) {
    string ip_address;
    int portnum;
    if (argc < 2){
            cout << "Please enter ip address: " << endl;
            cin >> ip_address;
            cout << "Please enter port number: " << endl;
            cin >> portnum;
    }else {
        ip_address = argv[1];
        portnum = atoi(argv[2]);
    }

    

    boost::asio::io_service my_service;
    tcp::resolver resolver(my_service);
    boost::shared_ptr<tcp::socket> socket(new tcp::socket(my_service));
    boost::asio::ip::address address = boost::asio::ip::make_address(ip_address);
    socket->connect(tcp::endpoint(address,portnum));

    string username;

    cout << "Welcome to Battleship Please enter your username :" <<endl;
    cin >> username;
    while(username.size() > 15){
        cout << "Please use smaller username"<<endl;
        cin >> username; 
    }
    

    battleship_client battle_client;

    battle_client.usernames(username,socket);

    battle_client.placeship();

    battle_client.send_board(socket);
    


    string move , opponents_move;

    while (!battle_client.winner_found(socket)){
        
        do{
        move = battle_client.get_move();
        }while(!battle_client.confirm_move(move));
        
        battle_client.send_move(move,socket);

        switch (battle_client.recieve_result(socket)){
            case HIT:
                battle_client.opponent_hit(move);
                break;
            case MISS:
                battle_client.opponent_miss(move);
                break;
        } 
        opponents_move = battle_client.Recievemove(socket);    
        battle_client.opponets_move_board_update(opponents_move);
    
    }

    
    endwin();

    battle_client.result();


    return 0;

}