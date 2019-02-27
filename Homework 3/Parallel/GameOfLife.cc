#include <iostream>
#include <thread>
#include "GameOfLife.h"

std::vector<std::vector<int>> GameOfLife::SimulateLife(std::vector<std::vector<int>>& b, int life_cycles){
    
    board = b; // laod the passed in board as a private member
    
    int n = board.size();
    int num_elems_for_proccessing = n*n;


    
    next_board.resize(board.size());
    for (int k=0; k < board.size();k++){
        next_board[k].resize(board.size());
    }  

    int num_threads = std::thread::hardware_concurrency();
    int elements_per_thread = num_elems_for_proccessing / num_threads;

    std::thread threads[num_threads]; // create # of threads dictated by hardware 

    for (int u=0; u < life_cycles; u++){
        for (int i=0; i < num_threads; i++){
             if(i == num_threads-1){ // the last thread will have more work than the others
                threads[i] = std::thread(&GameOfLife::calculate_board_section,this,i, i*elements_per_thread, num_elems_for_proccessing);    
            }else{
                threads[i] = std::thread(&GameOfLife::calculate_board_section,this,i,i*elements_per_thread, (i+1)*elements_per_thread);
            }
        }
        for(int j=0; j < num_threads; j++){
            threads[j].join();
        }// join all threads and set the new board
    
        board = next_board;
    }

    return(board);
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
        if(x-1 >= 0){
            if(board[(x-1) % n][(n-y-1) % n] >= 1)
            living_sourrounding_cells++;
        }
        if(y-1 >= 0){
            if(board[(n-x-1) % n][(y-1) % n] >= 1)
            living_sourrounding_cells++;
        }if(x-1 < 0 & y-1 < 0){
            if(board[(n-x-1) % n][(n-y-1) % n] >= 1)
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
void GameOfLife::calculate_board_section(int thread,int from ,int to){
    int n=board.size();
    int x;
    int y;
    
    for(int i=from ; i < to; i++){
         x = i/n;
         y = i%n;
        next_board[i/n][i%n] = alive_next_round(i/n,i%n);
    }
}
