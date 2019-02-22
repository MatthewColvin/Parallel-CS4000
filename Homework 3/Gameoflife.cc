#include <iostream>
#include "Gameoflife.h"

std::vector<std::vector<int>>  GameOfLife::SimulateLife(int life_cycles){

    for(int i=0;i<board.size();i++){
        for(int j=0;j<board[i].size();j++){   // for every element in the board
            

        }
    }

}

int GameOfLife::alive_next_round(int x,int y){
    int living_sourrounding_cells=0;
    int n = board.size();

    if(board[x+1 % n][y] == 1){
        living_sourrounding_cells++;
    }
    if(board[x-1 % n][y] == 1){
        living_sourrounding_cells++;
    }
    if(board[x-1 % n][y+1 % n] == 1){
        living_sourrounding_cells++;
    }
    if(board[x-1 % n][y-1 % n] == 1){
        living_sourrounding_cells++;
    }
    if(board[x+1 % n][y+1 % n] == 1){
        living_sourrounding_cells++;
    }
    if(board[x+1 % n][y-1 % n] == 1){
        living_sourrounding_cells++;
    }
    if(board[x][y+1 % n] == 1){
        living_sourrounding_cells++;
    }
    if(board[x][y-1 % n] == 1){
        living_sourrounding_cells++;
    }

    if(living_sourrounding_cells > 4){ // the cell will die from overpopultion
        return 0;
    }
    if(living_sourrounding_cells < 2){ // the cell will die from lonliness
        return 0;
    }
    // if living_surrounding_cells == 2,3,4 the cell will live
        return 1;

}


void GameOfLife::display_board(){

    for(int i=0;i<board.size();i++){
        for(int j=0;j<board[i].size();j++){   // for every element in the board
            std::cout<< board[i][j] << " ";
        }
        std:: cout<< std::endl;
    }
}