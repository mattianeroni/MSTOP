#include <iostream>
#include <chrono>
#include <string>



#include "utils.h"
#include "solver.h"



using namespace std;
using namespace std::chrono;


int main ()
{

  
  auto filenames = array<string, 56>{
    "g12_2_c.txt",   "g14_2_c.txt",   "g26_2_c.txt",   "g356_2_c.txt",  "g47_2_c.txt",
    "g12_2_k.txt",   "g14_2_n.txt",   "g26_2_k.txt",   "g356_2_n.txt",  "g47_2_n.txt",
    "g123_2_c.txt",  "g14_4_c.txt",   "g26_4_c.txt",   "g356_4_c.txt",  "g47_4_c.txt",
    "g123_2_k.txt",  "g14_4_n.txt",   "g26_4_k.txt",   "g356_4_n.txt",  "g47_4_n.txt",
    "g123_4_c.txt",  "g146_2_c.txt",  "g346_2_c.txt",  "g45_2_c.txt",   "g57_2_c.txt",
    "g123_4_k.txt",  "g146_2_n.txt",  "g346_2_n.txt",  "g45_2_n.txt",   "g57_2_n.txt",
    "g12_4_c.txt",   "g146_4_c.txt",  "g346_4_c.txt",  "g45_4_c.txt",   "g57_4_c.txt",
    "g12_4_k.txt",   "g146_4_n.txt",  "g346_4_n.txt",  "g45_4_n.txt",   "g57_4_n.txt",
    "g127_2_c.txt",  "g256_2_c.txt",  "g35_2_c.txt",   "g456_2_c.txt",
    "g127_2_k.txt",  "g256_2_k.txt",  "g35_2_n.txt",   "g456_2_n.txt",
    "g127_4_c.txt",  "g256_4_c.txt",  "g35_4_c.txt",   "g456_4_c.txt",
    "g127_4_k.txt",  "g256_4_k.txt",  "g35_4_n.txt",   "g456_4_n.txt",

  };



  for (string filename : filenames) 
  {


    Problem problem = read_file("../tests/multi/" + filename);

    float alpha = optimize_alpha(problem);

    set_savings(problem, alpha);



    // heursitic
    
    auto start = high_resolution_clock::now();

    auto solution = heuristic(problem, GREEDY_BETA);

    auto stop = high_resolution_clock::now();

    cout << solution->cost << "  " << solution->revenue << "  " << duration_cast<milliseconds>(stop - start).count();
    



    // metaheuristic
    
    start = high_resolution_clock::now();

    solution = metaheuristic(problem, 0.1, 0.3, 1000);

    stop = high_resolution_clock::now();

    cout << solution->cost << "  " << solution->revenue << "  " << duration_cast<milliseconds>(stop - start).count();
    


    cout << "\n";

    delete solution;

  }
  
  
  return 0;

}
