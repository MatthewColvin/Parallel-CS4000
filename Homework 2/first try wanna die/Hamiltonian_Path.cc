//***************************************************************
// File: Hamiltonian_Path.cc
// Author: David W. Juedes
// Purpose: This code finds whether there exists a Hamiltonian Path 
// from one vertex to another in a graph.
//****************************************************************
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <cassert>
#include <vector>
#include <list>
#include <omp.h>
#include <cmath>
using namespace std;


size_t factorial(size_t n)
{
    if(n > 1)
        return n * factorial(n - 1);
    else
        return 1;
}

void create_perm(int k , vector<int>& perm) 
{ 
    sort(perm.begin(), perm.end()); 
    long int i = 1; 
    do { 
        if (i == k) 
            break; 
        i++; 
    } while (next_permutation(perm.begin(), perm.end())); 
} 



bool valid_tour(vector<list<int> >&adj_list, vector<int> &tour) {  
  // Tests whether the tour is valid
  // Returns the total distance

  int start = tour[0];  // the first node in the tour
  bool valid = true;
  for (int i=1;i<tour.size();i++) {
    bool found = false;
    // Is there an edge from tour[i-1] to tour[i]
    for (list<int >::iterator p = adj_list[tour[i-1]].begin(); p!=adj_list[tour[i-1]].end();++p) {  // traverse the tour list
      if (*p == tour[i]) { // derefrence the iterator into the values in the list 
	found = true;
	break;
      }
    }
    if (!found) valid = false;
  }
  return valid;
}


vector<int> Hamiltonian_tour(vector<list<int> >&adj_list, int i1, int j1, bool &found_it) {

  int n = adj_list.size();
  vector<int> perm;
  vector<int> opt;
  vector<int> Hamiltonian_tour;;

  for (size_t i=0;i<n;i++) {  // load perm with all nodes other than the beginning and end (travel to and travel from)
      if ((i!=i1)&&(i!=j1)) {   
        perm.push_back(i);
      }
    }

  size_t num_permutations = factorial(perm.size());

  #pragma omp parallel
  {
    vector<int> local_perm = perm;  
    for (int permutation = 1; permutation < num_permutations+1 ; permutation++){ // for the number of permutations
        if(permutation % omp_get_num_threads() == omp_get_thread_num()){  
          create_perm(permutation,local_perm);

          
          /*
          #pragma omp critical 
          {
            cout << "permutation: " << permutation << "::";
            for (int j=0;j<local_perm.size();j++){
              cout << local_perm[j] << ' ' ;
            }
            cout << endl;
          }
          */
              vector<int> tour;
              tour.resize(n);
              tour[0] = i1;   // loads the first node into the tour     //  i1 comes from the main
              tour[n-1] = j1; // load the last node into the tour      // j1  
              for (int i=0;i<n-2;i++) {  // loads the middle of the tour with the just genertated permutiaton
                tour[i+1] = local_perm[i];
              }
              bool found;
              found = valid_tour(adj_list,tour);
              if (found) {
                #pragma omp critical // incase multiple tours are found at once only one will be written
                {
                   found_it=true;
                   Hamiltonian_tour = tour;
                }
              }else{
                found_it = false;
              }
          }
    }
  }
  return(Hamiltonian_tour);
} 

  int main() {
    int n;  // number of nodes
    int i1; // location to travel from 
    int j1; // location to travel to 
    // goal is to travel to every node inbetween 
    cin >> n;
    cin >> i1;
    cin >> j1;
    string line;
    getline(cin,line); // Get the newline
    vector<list<int> > adj_list; 
    adj_list.resize(n); // makeing the lists the size of the number of nodes.

    while (!cin.eof()) { // this looks reads all of the data into adj_list 
      getline(cin,line); 
      if (!cin.fail()) {      
        istringstream in(line); //* look up string streams...
        int j;
        in >> j;
        char c;
        in >> c;
        assert(c==':'); // assert ends the program when the condition evalutates to false
        while (!in.eof()) {
          int i;
          in >> i;
          if (!in.fail()) {
          adj_list[j].push_back(i); // j is the node you are on and the list holds the connections to node j 
          }
        }  
      }
    }
     
  bool found=false;
  vector<int> opt;
  opt = Hamiltonian_tour(adj_list,i1,j1,found);
  if (found) {
    cout << "Tour = ";
    for (int i=0;i<opt.size();i++) {
      cout << opt[i] << " ";
    }
    cout << endl;
  } else {
    cout << "No Hamiltonian Tour" << endl;
    }
}

