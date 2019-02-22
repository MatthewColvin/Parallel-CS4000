#include <iostream>
#include <vector>
#include <fstream>

class GameOfLife{
    public:
        std::vector<std::vector<int>> SimulateLife(int life_cycles);
        /*

        */
        void read_board(std::ifstream in);
        /*
            Function to read the board in from the main program
        */
        void display_board();
        /*

        */
        int alive_next_round(int x,int y);
        /*
            function to check if board[x][y] will be alive in the next generation
            
            returns 1 if the cell will stay alive or be born  
                    0 if the cell will die or stay dead

        */
    private:
        std::vector<std::vector<int>> board; // the board for the game of life
}