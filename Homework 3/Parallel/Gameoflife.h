#include <iostream>
#include <vector>
#include <thread>
#include <fstream>
#include <future>
#include <cstdlib>

class GameOfLife{
    public:
        void SimulateLife(int life_cycles);
        /*

        */
        void read_board(std::istream &in);
        /*
            Function to read the board in from the main program
        */
        void display_board(std::ostream &out);
        /*
            Displays the board to the parameter out 
        */
        int alive_next_round(int x,int y);
        /*
            function to check if board[x][y] will be alive in the next generation
            
            returns 1 if the cell will stay alive or be born  
                    0 if the cell will die or stay dead

        */
       int down_left(int x, int y);
       int down__down_right(int x, int y);
       int up_left__left(int x, int y);
       int up_right__up__Right(int x, int y);

       void make_test_board(int n);
       /*
            the function resizes board to a nxn 2D vector 
            and loads zeros into it.
       */
       
       void live_forever(int x,int y){board[x][y]=2;}
       void live(int x,int y){board[x][y]=1;}
       void die (int x,int y){board[x][y]=0;}
        /* 
            functions used to kill or bring cells to life 
        */

       
    private:
        std::vector<std::vector<int>> board; // the board for the game of life
        int get_board_size(){ return (board.size());}

};