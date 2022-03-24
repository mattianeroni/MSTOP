#ifndef SOLVER_H
#define SOLVER_H


#include "edge.h"
#include "utils.h"
#include "node.h"
#include "mapper.h"
#include "pjs.h"

#include <unordered_map>
#include <iostream>
#include <vector>
#include <random>


using std::unordered_map;
using std::vector;
using std::cout;


// value of beta parameter for a greedy behaviour in the biased randomisation
const float GREEDY_BETA = 0.9999;



struct Solution {

    Mapping mapping;
    vector<PJS_Solution*> pjs_sols;
    int revenue;
    float cost;

    Solution(Mapping mapping, vector<PJS_Solution*> pjs_sols, int revenue, float cost){
        this->mapping = mapping;
        this->pjs_sols = pjs_sols;
        this->revenue = revenue;
        this->cost = cost;
    }

    ~Solution(){
        for (auto sol : pjs_sols)
            delete sol;
        pjs_sols.clear();
    }

};


void set_savings (Problem& problem, float alpha) {

    auto dists = problem.dists;
    auto depotid = problem.depot->id;

    for (Edge* edge : problem.edges) {

        Node* inode = edge->inode;
        Node* jnode = edge->jnode;
        int inodeid = inode->id;
        int jnodeid = jnode->id;
        edge->savings = unordered_map<int,float>();

        for (auto source : problem.sources) {

            edge->savings[source->id] = (1.0 - alpha) * (dists[inodeid][depotid] + dists[source->id][jnodeid] - edge->cost) + alpha * (inode->revenue + jnode->revenue);

        }

    }

}



Solution* heuristic (Problem& problem, float beta) {

    // execute the mapper with eventually a certain biased randomisation
    Mapping mapping = mapper(problem, beta);

    // init elements of the final solutions 
    float cost = 0.0f;
    int revenue = 0;
    vector<PJS_Solution*> sols;

    // execute the pjs and update final overall solution
    for (Node* source : problem.sources) {

        PJS_Solution* pjs_solution = PJS(problem, source, problem.depot, source->nodes, GREEDY_BETA);

        cost += pjs_solution->cost;
        revenue += pjs_solution->revenue;
        sols.push_back(pjs_solution);

    }

    return new Solution (mapping, sols, revenue, cost);

}





Solution* metaheuristic ( Problem& problem, float minbeta, float maxbeta, int maxiter) {

    // random engine
    std::mt19937 random_engine(std::random_device{}());
    std::uniform_real_distribution<float> randombeta (minbeta, maxbeta);

    // generate starting greedy solution
    Solution* best = heuristic(problem, GREEDY_BETA);

    for (int i = 0; i < maxiter; i++) {

        // generate a random beta 
        float beta = randombeta(random_engine);

        // generate new solution
        auto newSolution = heuristic(problem, beta);

        // eventually update new best
        if ( newSolution->revenue > best->revenue ) {
            best = newSolution;
        }

    }

    return best;

}









#endif