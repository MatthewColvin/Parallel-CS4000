#include <iostream>
#include <ncurses.h>
#include <vector>
using namespace std;

void draw_matrix(vector<vector<int> > &board,
		     int cur_row,
		     int cur_col) {

  for (int j=0;j<4;j++) {
      move(0,2*j);
      printw("+-");
    }
    move(0,2*4);
    printw("+");
  for (int i=0;i<4;i++) {
    for (int j=0;j<4;j++) {
      move(2*i+1,2*j);
      printw("|");
      move(2*i+1,2*j+1);
      switch(board[i][j]){
          case 0:
            printw(" "); // dont know yet what will happen
          case 1:
            printw("M"); // if board is set to 1 then it is a miss 
          case 2:
            printw("H"); // if board is set to 2 then it is a hit
      }
    move(2*i+1,2*4);
    printw("|");
    for (int j=0;j<4;j++) {
      move(2*i+2,2*j);
      printw("+-");
    }
    move(2*i+2,2*4);
    printw("+");
  }
  move(2*cur_row+1,2*cur_col+1);
  }
}


int main(){

int rows;
 
int cols;
  int cur_row=0;
  int cur_col=0;
  int ch;

vector<vector<int> > board;
    
    for (int i=0;i<4;i++) {
        vector<int> t;
        for (int j=0;j<4;j++) {
            t.push_back(1);
        }
        board.push_back(t);
    }

    board[2][2]=2;
    board[1][1]=0;



    initscr();
  // Clear the screen
    clear();
  // Get the size of the window!
    getmaxyx(stdscr,rows,cols);
    cbreak();  // Pass all characters to this program!
    keypad(stdscr, TRUE); // Grab the special keys, arrow keys, etc.
  // Paint the row and column markers.
  //paint_markers(rows,cols,10,0,0);
  // Redraw the screen.
    refresh();

    draw_matrix(board,0,0);

    refresh();

    endwin();

}