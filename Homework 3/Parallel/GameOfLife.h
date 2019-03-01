/*
        Matthew Colvin
        Homework 3 

        Decription: Class used to simulate the Game of life invented by John Conway.
        This class uses c++ threads to speed up the execution.

*/
#include <iostream>
#include <vector>
#include <fstream>

class GameOfLife{
    public:

        std::vector<std::vector<int>> SimulateLife(std::vector<std::vector<int>>& b,int life_cycles);
        /*
            Function used to play the game of life. The game will be ran
            life_cycle times and will retrun the board that is generated.
        */
        void calculate_board_section(int thread,int from ,int to);
        /*
            This function is used to split up the work for each thread. 
            thread-

        */
        void display_board(std::ostream &out);
        /*
            Displays the board to the parameter out 
        */
        int alive_next_round(int x,int y);
        /*
            function to check if board[x][y] will be alive in the next generation
            
            returns 2 if the cell was 2 already
                    1 if the cell will stay alive or be born  
                    0 if the cell will die or stay dead
        */
       void make_test_board(int n);
       /*
            the function resizes board to a nxn 2D vector 
            and loads zeros into it.
       */

       void live_forever(int x,int y){board[x][y]=2;}
       void live(int x,int y){board[x][y]=1;}
       void die (int x,int y){board[x][y]=0;}
        /* 
            functions used to kill or bring cells to life for testing
        */

       
    private:
        std::vector<std::vector<int>> board; // the board for the game of life
        std::vector<std::vector<int>> next_board; // used to hold next rounds board 

};