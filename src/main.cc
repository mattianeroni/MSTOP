#include <iostream>
#include <chrono>
#include <string>

#include "utils.h"
#include "solver.h"



using namespace std;
using namespace std::chrono;



int main ()
{

  
  auto filenames = vector<vector<string>> {
    {"g12_4_k.txt","p1.4.k.txt","p2.4.k.txt",""},
    {"g26_2_k.txt","p2.2.k.txt","p6.2.k.txt",""},
    {"g57_2_n.txt","p5.2.n.txt","p7.2.n.txt",""},
    {"g45_2_n.txt","p4.2.n.txt","p5.2.n.txt",""},
    {"g57_4_c.txt","p5.4.c.txt","p7.4.c.txt",""},
    {"g45_4_n.txt","p4.4.n.txt","p5.4.n.txt",""},
    {"g14_4_n.txt","p1.4.n.txt","p4.4.n.txt",""},
    {"g35_2_c.txt","p3.2.c.txt","p5.2.c.txt",""},
    {"g35_4_c.txt","p3.4.c.txt","p5.4.c.txt",""},
    {"g14_4_c.txt","p1.4.c.txt","p4.4.c.txt",""},
    {"g12_2_k.txt","p1.2.k.txt","p2.2.k.txt",""},
    {"g45_2_c.txt","p4.2.c.txt","p5.2.c.txt",""},
    {"g47_2_n.txt","p4.2.n.txt","p7.2.n.txt",""},
    {"g12_2_c.txt","p1.2.c.txt","p2.2.c.txt",""},
    {"g26_2_c.txt","p2.2.c.txt","p6.2.c.txt",""},
    {"g35_4_n.txt","p3.4.n.txt","p5.4.n.txt",""},
    {"g14_2_n.txt","p1.2.n.txt","p4.2.n.txt",""},
    {"g57_2_c.txt","p5.2.c.txt","p7.2.c.txt",""},
    {"g47_4_c.txt","p4.4.c.txt","p7.4.c.txt",""},
    {"g14_2_c.txt","p1.2.c.txt","p4.2.c.txt",""},
    {"g35_2_n.txt","p3.2.n.txt","p5.2.n.txt",""},
    {"g47_2_c.txt","p4.2.c.txt","p7.2.c.txt",""},
    {"g47_4_n.txt","p4.4.n.txt","p7.4.n.txt",""},
    {"g12_4_c.txt","p1.4.c.txt","p2.4.c.txt",""},
    {"g57_4_n.txt","p5.4.n.txt","p7.4.n.txt",""},
    {"g26_4_c.txt","p2.4.c.txt","p6.4.c.txt",""},
    {"g26_4_k.txt","p2.4.k.txt","p6.4.k.txt",""},
    {"g45_4_c.txt","p4.4.c.txt","p5.4.c.txt",""},
    {"g123_4_k.txt","p1.4.k.txt","p2.4.k.txt","p3.4.k.txt"},
    {"g456_2_n.txt","p4.2.n.txt","p5.2.n.txt","p6.2.n.txt"},
    {"g356_4_c.txt","p3.4.c.txt","p5.4.c.txt","p6.4.c.txt"},
    {"g346_2_n.txt","p3.2.n.txt","p4.2.n.txt","p6.2.n.txt"},
    {"g127_4_c.txt","p1.4.c.txt","p2.4.c.txt","p7.4.c.txt"},
    {"g127_4_k.txt","p1.4.k.txt","p2.4.k.txt","p7.4.k.txt"},
    {"g356_4_n.txt","p3.4.n.txt","p5.4.n.txt","p6.4.n.txt"},
    {"g256_2_c.txt","p2.2.c.txt","p5.2.c.txt","p6.2.c.txt"},
    {"g123_2_c.txt","p1.2.c.txt","p2.2.c.txt","p3.2.c.txt"},
    {"g146_4_n.txt","p1.4.n.txt","p4.4.n.txt","p6.4.n.txt"},
    {"g346_4_c.txt","p3.4.c.txt","p4.4.c.txt","p6.4.c.txt"},
    {"g346_4_n.txt","p3.4.n.txt","p4.4.n.txt","p6.4.n.txt"},
    {"g127_2_c.txt","p1.2.c.txt","p2.2.c.txt","p7.2.c.txt"},
    {"g127_2_k.txt","p1.2.k.txt","p2.2.k.txt","p7.2.k.txt"},
    {"g456_4_n.txt","p4.4.n.txt","p5.4.n.txt","p6.4.n.txt"},
    {"g256_2_k.txt","p2.2.k.txt","p5.2.k.txt","p6.2.k.txt"},
    {"g456_4_c.txt","p4.4.c.txt","p5.4.c.txt","p6.4.c.txt"},
    {"g256_4_k.txt","p2.4.k.txt","p5.4.k.txt","p6.4.k.txt"},
    {"g146_2_n.txt","p1.2.n.txt","p4.2.n.txt","p6.2.n.txt"},
    {"g256_4_c.txt","p2.4.c.txt","p5.4.c.txt","p6.4.c.txt"},
    {"g146_2_c.txt","p1.2.c.txt","p4.2.c.txt","p6.2.c.txt"},
    {"g123_4_c.txt","p1.4.c.txt","p2.4.c.txt","p3.4.c.txt"},
    {"g356_2_c.txt","p3.2.c.txt","p5.2.c.txt","p6.2.c.txt"},
    {"g146_4_c.txt","p1.4.c.txt","p4.4.c.txt","p6.4.c.txt"},
    {"g456_2_c.txt","p4.2.c.txt","p5.2.c.txt","p6.2.c.txt"},
    {"g356_2_n.txt","p3.2.n.txt","p5.2.n.txt","p6.2.n.txt"},
    {"g123_2_k.txt","p1.2.k.txt","p2.2.k.txt","p3.2.k.txt"},
    {"g346_2_c.txt","p3.2.c.txt","p4.2.c.txt","p6.2.c.txt"}
  };


  cout << "Problem, P1, P2, P3, HeurC, HeurR, HeurT, RTMetaC, RTMetaR, RTMetaT, IntMetaC, IntMetaR, IntMetaT, SepHeurC, SepHeurR, SepHeurT, SepMetaC, SepMetaR, SepMetaT,\n";



  for (vector<string> filename : filenames) 
  {
    
    // read multi source problem
    Problem problem = read_file("../tests/multi/" + filename[0]);


    // plot problems names
    cout << filename[0] << ", ";
    for (int i = 1; i < 4; i++)
      cout << filename[i] << ", ";



    // optimise alpha and set problems savings 
    float alpha = optimize_alpha(problem);
    set_savings(problem, alpha);




    // heuristic
    
    auto start = high_resolution_clock::now();

    auto solution = heuristic(problem, GREEDY_BETA);

    auto stop = high_resolution_clock::now();

    cout << (int)solution->cost << ", " << solution->revenue << ", " << duration_cast<milliseconds>(stop - start).count() << ", ";
    



    // metaheuristic
    
    start = high_resolution_clock::now();

    solution = metaheuristic(problem, 0.1, 0.3, 1000);

    stop = high_resolution_clock::now();

    cout << (int)solution->cost << ", " << solution->revenue << ", " << duration_cast<milliseconds>(stop - start).count() << ", ";




    // intensive search metaheuristic
    
    start = high_resolution_clock::now();

    solution = intensive_metaheuristic(problem, 0.1, 0.3, 1000, 5);
  
    stop = high_resolution_clock::now();

    cout << (int)solution->cost << ", " << solution->revenue << ", " << duration_cast<milliseconds>(stop - start).count() << ", ";



    // separated heuristic approaches
    float total_cost = 0.0f;
    int total_revenue = 0;
    long long total_time = 0LL;

    for (int i = 1; i < 4; i++) {

      if (filename[i] == "")
        continue;

      Problem ss_problem = read_single_source("../tests/single/" + filename[i]);
      ss_problem.Tmax = problem.Tmax;

      unordered_map<int,Node*> nodes;

      for (Node* n : problem.nodes) {
        nodes[n->id] = n;
      }

      start = high_resolution_clock::now();

      PJS_Solution* pjs_solution = PJS(problem, problem.sources[0], problem.depot, nodes, GREEDY_BETA);

      stop = high_resolution_clock::now();

      total_cost += pjs_solution->cost;
      total_revenue += pjs_solution->revenue;
      total_time += duration_cast<milliseconds>(stop - start).count();

    }

    cout << (int)total_cost << ", " << total_revenue << ", " << total_time << ", ";


    cout << "\n";

    delete solution;

  }
  
  
  return 0;

}
