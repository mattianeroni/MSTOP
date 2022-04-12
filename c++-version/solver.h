#ifndef SOLVER_H
#define SOLVER_H


#include "edge.h"
#include "utils.h"
#include "node.h"
#include "mapper.h"
#include "pjs.h"
#include "route.h"
#include "bra.h"

#include <unordered_map>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>


using std::unordered_map;
using std::vector;
using std::cout;
using std::endl;


// value of beta parameter for a greedy behaviour in the biased randomisation
const float GREEDY_BETA = 0.9999;



struct Solution {

    Mapping mapping;
    vector<Route*> routes;
    int revenue;
    float cost;

    Solution(Mapping& mapping, vector<Route*>& routes, int revenue, float cost){
        this->mapping = mapping;
        this->routes = routes;
        this->revenue = revenue; 
        this->cost = cost;
    }

    ~Solution(){
        for (auto r : routes)
            delete r;
        routes.clear();
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
    vector<Route*> routes;

    // execute the pjs and update final overall solution
    for (Node* source : problem.sources) {

        PJS_Solution* pjs_solution = PJS(problem, source, problem.depot, source->nodes, GREEDY_BETA);

        cost += pjs_solution->cost;
        revenue += pjs_solution->revenue;
        routes.insert( routes.end(), pjs_solution->routes.begin(), pjs_solution->routes.end() );

    }

    return new Solution (mapping, routes, revenue, cost);

}




float optimize_alpha ( Problem& problem )
{
    float bestalpha = 0.0f;
    int bestrevenue = 0;

    for (float alpha = 0.0; alpha < 1.1; alpha += 0.1) {

        set_savings(problem, alpha);

        Solution* solution = heuristic(problem, GREEDY_BETA);

        if (solution->revenue > bestrevenue){
            bestalpha = alpha;
            bestrevenue = solution->revenue;
        }

        delete solution;

    }

    return bestalpha;
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






Solution* intensive_metaheuristic ( Problem& problem, float minbeta, float maxbeta, int maxiter, int nelites) {

    // random engine
    std::mt19937 random_engine(std::random_device{}());
    std::uniform_real_distribution<float> randombeta (minbeta, maxbeta);

    // generate the heap of elite solutions (i.e., sorted form the worst to the best)
    // NOTE: the revenue is changes of sign (+/-)
    Solution* best = heuristic(problem, GREEDY_BETA); 
    vector<pair<int, Solution*>> elites = { make_pair(-best->revenue, best) };


    for (int i = 0; i < maxiter; i++) 
    {

        // generate a random beta 
        float beta = randombeta(random_engine);

        // generate new solution
        auto newSolution = heuristic(problem, beta);

        // eventually update the elites 
        auto worst_element = elites.front();
        int worst_revenue = - worst_element.first;

        // eventually append new solution to elites 
        if ( newSolution->revenue > worst_revenue || elites.size() < nelites ) {
            elites.push_back(make_pair (-newSolution->revenue, newSolution) );
            push_heap( elites.begin(), elites.end() );
        }
        
        // if elites heap is getting too long remove the worst solution
        if ( elites.size() > nelites ){
            pop_heap(elites.begin(), elites.end());
            elites.pop_back();
            delete worst_element.second;
        } 
    }

    int N = problem.nodes.size();
    int S = problem.sources.size();

    // init a current best solution and revenue (not necessarily the real one)
    best = elites.front().second;

    // optimize the elites and find the best one
    for ( pair<int, Solution*> elite : elites ) {
        
        // consider the current elite mapping and delete the solution
        Solution* elite_solution = elite.second;
        Mapping mapping = elite_solution->mapping;
        delete elite_solution;

        // generate a new solution
        vector<Route*> routes;
        float cost = 0.0f;
        int revenue = 0;

        // for each subset of nodes assigned to a different source
        // a multistart PJS is made.
        for (int s = 0; s < S; s++) {

            Node* source = problem.sources[s];
            unordered_map<int,Node*> nodes;

            for (int n = S; n < S + N; n++) {
                Node* node = problem.nodes[n - S];
                if (mapping[s][n] == 1) 
                    nodes[node->id] = node;
            }

            PJS_Solution* pjs_solution = PJS(problem, source, problem.depot, nodes, GREEDY_BETA);

            
            for (int i = 0; i < maxiter; i++ )
            {

                // generate a random beta 
                float beta = randombeta(random_engine);

                // generate new solution
                PJS_Solution* new_pjs_solution = PJS(problem, source, problem.depot, nodes, beta);

                if (new_pjs_solution->revenue > pjs_solution->revenue)
                    pjs_solution = new_pjs_solution;

            }

            cost += pjs_solution->cost;
            revenue += pjs_solution->revenue;
            routes.insert( routes.end(), pjs_solution->routes.begin(), pjs_solution->routes.end() );

        } 

        // if the elite is better than the best, update the best solution
        if (revenue > best->revenue) 
            best = new Solution (mapping, routes, revenue, cost);

    }

    return best;
}






PJS_Solution* MultiStartPJS ( Problem& problem, Node* source, Node* depot, const unordered_map<int,Node*>& nodes, 
                              float minbeta, float maxbeta, int maxiter)
{

    // random engine
    std::mt19937 random_engine(std::random_device{}());
    std::uniform_real_distribution<float> randombeta (minbeta, maxbeta);

    PJS_Solution* best = PJS(problem, source, problem.depot, nodes, GREEDY_BETA);

            
    for (int i = 0; i < maxiter; i++ )
    {

        // generate a random beta 
        float beta = randombeta(random_engine);

        // generate new solution
        PJS_Solution* newsol = PJS(problem, source, problem.depot, nodes, beta);

        if (newsol->revenue > best->revenue)
            best = newsol;

    }

    return best;

}



#endif