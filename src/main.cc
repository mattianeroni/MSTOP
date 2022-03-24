#include <iostream>
#include <chrono>



#include "utils.h"
#include "solver.h"



using namespace std;
using namespace std::chrono;


int main ()
{




  Problem problem = read_file("../tests/multi/g456_4_n.txt");

  float alpha = 0.5;

  set_savings(problem, alpha);


  
  auto start = high_resolution_clock::now();

  auto solution = heuristic(problem, GREEDY_BETA);

  auto stop = high_resolution_clock::now();

  cout << solution->cost << "  " << solution->revenue << "  " << duration_cast<milliseconds>(stop - start).count() << "\n";
  



  
  start = high_resolution_clock::now();

  solution = metaheuristic(problem, 0.1, 0.3, 1000);

  stop = high_resolution_clock::now();

  cout << solution->cost << "  " << solution->revenue << "  " << duration_cast<milliseconds>(stop - start).count() << "\n";
  

  delete solution;
  
  
  return 0;

}
