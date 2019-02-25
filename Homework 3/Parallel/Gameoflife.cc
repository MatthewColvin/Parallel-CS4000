#include <iostream>
#include <thread>
#include <future>
#include <cstdlib>
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

    std::future<int> check1 = async(std::launch::async, down_left,x,y);
    std::future<int> check2 = async(std::launch::async,down__down_right,x,y);
    std::future<int> check3 = async(std::launch::async,up_left__left,x,y);
    std::future<int> check4 = async(std::launch::async,up_right__up__Right,x,y);

    living_sourrounding_cells += check1.get();
    living_sourrounding_cells += check2.get();  
    living_sourrounding_cells += check3.get();
    living_sourrounding_cells += check4.get();


    // checks for the cells alive around the current cell at board[x][y]

    if(living_sourrounding_cells >= 4){ // the cell will die from overpopultion
        return 0;
    }
    if(living_sourrounding_cells == 3){ 
        return 1; // cell is born or stays alive
    }
    if(living_sourrounding_cells == 2){ 
        return board[x][y]; // cell stays in its current state
    }
    if(living_sourrounding_cells < 2){ // the cell will die from lonliness
        return 0;
    }
    

}

int GameOfLife::down_left(int x,int y){
    int living_sourrounding_cells=0;
    int n= get_board_size();
    if(x-1 >= 0  &&  y-1 >= 0){
        if(board[(x-1) % n][(y-1) % n] >= 1){
            living_sourrounding_cells++;
        }
    }else{ // switching calculation
        if(board[(n-x-1) % n][(n-y-1) % n] >= 1){
            living_sourrounding_cells++;
        }
    }
    return(living_sourrounding_cells);
}

int GameOfLife::down__down_right(int x, int y){
    int living_sourrounding_cells=0;
    int n= get_board_size();
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
   return(living_sourrounding_cells);

}

int GameOfLife::up_left__left(int x, int y){
      int living_sourrounding_cells=0;
    int n= get_board_size();
      if(y-1 >= 0){
        if(board[(x+1) % n][(y-1) % n] >= 1){  // up left
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
    return(living_sourrounding_cells);
}

int GameOfLife::up_right__up__Right(int x, int y){
    int living_sourrounding_cells=0;
    int n= get_board_size();
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
    return(living_sourrounding_cells);

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
