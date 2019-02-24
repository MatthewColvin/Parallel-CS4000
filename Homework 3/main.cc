#include <iostream>
#include "Gameoflife.h"

using namespace std;

int main(){
    
    GameOfLife game;

    game.make_test_board(10);
    
    game.live(5,5);
    game.live(5,4);
    game.live(5,3);
    game.live(6,5);
    game.live(6,4);
    game.live(6,3);
 
    

    for (int i=0; i<4; i++){
        game.display_board(cout);
        game.SimulateLife(i);  
        cout << endl << endl << endl << endl;  
    }
    
}