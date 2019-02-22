#include <iostream>
#include "Gameoflife.h"

void GameOfLife::SimulateLife(int life_cycles){
    
    std::vector<std::vector<int>> next_board; // used to hold next rounds board 
    next_board.resize(board.size());
    for (int k=0; k < board.size();k++){
        next_board[k].resize(board.size());
    }    // board resized


    for(int i=0;i<board.size();i++){
        for(int j=0;j<board[i].size();j++){   // for every element in the board
            next_board[i][j]=alive_next_round(i,j);
        }
    }
    board=next_board;
}

int GameOfLife::alive_next_round(int x,int y){
    
    if(board[x][y] == 2){
        return 2;    // the cell is invincible 
    }
    
    int living_sourrounding_cells=0;
    int n = board.size();

  
    if(x-1 >= 0  &&  y-1 >= 0){
        if(board[(x-1) % n][(y-1) % n] >= 1){
            living_sourrounding_cells++;
        }
    }else{ // switching calculation
        if(board[(n-x-1) % n][(n-y-1) % n] >= 1){
            living_sourrounding_cells++;
        }
    }



    if(x-1 >= 0){ // - values are mapped to negative numbes in c++
        if(board[(x-1) % n][y] >= 1){
            living_sourrounding_cells++;
        }
        if(board[(x-1) % n][(y+1) % n] >= 1){
            living_sourrounding_cells++;
        }
    }else{  // switching calculation
        if(board[(n-x-1) % n][y] >= 1){
            living_sourrounding_cells++;
        }
        if(board[(n-x-1) % n][(y+1) % n] >= 1){
            living_sourrounding_cells++;
        }
    }
    
    if(y-1 >= 0){
        if(board[(x+1) % n][(y-1) % n] >= 1){
            living_sourrounding_cells++;
        }
        if(board[x][(y-1) % n] >= 1){
            living_sourrounding_cells++;
        }
    }else{ //switching calculation
        if(board[(x+1) % n][(n-y-1) % n] >= 1){
            living_sourrounding_cells++;
        }
        if(board[x][(n-y-1) % n] >= 1){
            living_sourrounding_cells++;
        }
    }


    // no negative mod issues
    if(board[(x+1) % n][(y+1) % n] >= 1){
        living_sourrounding_cells++;
    }
    if(board[(x+1) % n][y] >= 1){
        living_sourrounding_cells++;
    }
    if(board[x][(y+1) % n] >= 1){
        living_sourrounding_cells++;
    }
    
    // checks for the cells alive around the current cell at board[x][y]

    if(living_sourrounding_cells >= 4){ // the cell will die from overpopultion
        return 0;
    }
    if(living_sourrounding_cells < 2){ // the cell will die from lonliness
        return 0;
    }
    // if living_surrounding_cells == 2,3 the cell will live
        return 1;

}


void GameOfLife::display_board(std::ostream &out){

    for(int i=0;i<board.size();i++){
        for(int j=0;j<board[i].size();j++){   // for every element in the board
            out<< board[i][j] << " ";
        }
        out << std::endl;
    }
}

void GameOfLife::make_test_board(int n){

    board.resize(n);

    for(int i=0 ; i<n ; i++){
        board[i].resize(n);
    }  // board resized

    for(int i=0;i<board.size();i++){
        for(int j=0;j<board[i].size();j++){   // for every element in the board
            board[i][j] = 0;
        }
    } //board zeroed
}
