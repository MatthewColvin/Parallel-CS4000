//*************************************************************************
//File: GOL_display.cc
//Author: David W. Juedes
//Date: Written several years ago, but modified Spring 2019 for CS 4000
//Purpose: To display via "ncurses" the progress of the Game of Life for Homework #3.
// This is not the test harness for the assignment; however, it can be used to
// help debug your code.
//
//***************************************************************************

#include <iostream>
#include <cstdlib>
#include <vector>
#include <unistd.h>
extern "C" {
  #include <ncurses.h>
}
using namespace std;
#include "GameOfLife.h"


//*********************************************************************
// Function: print_board
// Input: Matrix (vector vector int) board
// Action: Outputs the board to the terminal.  0 == blank, 1 == *, 2 == E
//
//**************************************************************************
void print_board(vector<vector<int> > &board) {
  int n = board.size();
  
  for (int i=0;i<n;i++) {
    for (int j=0;j<n;j++) {
      move(i,j);
      if (board[i][j] == 2) {
	printw("E");
      }
      if (board[i][j]==1) {
	printw("*");
      } else {
	printw(" ");
      }
    }
  }
}
//************************************************************************
// Function: main()
// Purpose:  Reads in the data file and runs the simulation.
//************************************************************************
int main() {
  int rows;
  int cols;
  int n;
  cin >> n;
  vector<vector<int> > board;
  board.resize(n);
  for (int i=0;i<n;i++) {
    board[i].resize(n);
    for (int j=0;j<n;j++) {
      cin >> board[i][j];
    }
  }
  int k;
  cin >> k;

    // Screen initialization

  initscr();
  // Clear the screen

  clear();

  // Get the size of the window!
  getmaxyx(stdscr,rows,cols);

  cbreak();  // Pass all characters to this program!

  keypad(stdscr, TRUE); // Grab the special keys, arrow keys, etc.

  
  GameOfLife obj;
  vector<vector<int> > result;

  for (int i=0;i<k;i++) {
    sleep(1);
    result = obj.SimulateLife(board,i);
    print_board(result);
    refresh(); // Put the stuff on the screen
  }
  endwin();
}
  
  

 
