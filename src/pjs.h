#ifndef PJS_H
#define PJS_H

#include <vector>
#include <unordered_map>
#include <numeric>
#include <algorithm>
#include <chrono>
#include <iostream>

#include "mapper.h"
#include "route.h"
#include "bra.h"
#include "node.h"
#include "timedelta.h"

using std::cout;
using std::endl;
using std::vector;
using std::unordered_map;
using std::accumulate;
using namespace std::chrono;




struct PJS_Solution {
   
    vector<Route*> routes;
    int revenue;
    float cost;

    PJS_Solution(vector<Route*>& routes, int revenue, float cost) {
        this->routes = routes;
        this->revenue = revenue;
        this->cost = cost;
    }


    ~PJS_Solution() {
        for (auto route : routes)
            delete route;
        routes.clear();
    }

};





PJS_Solution*   PJS (Problem& problem, Node* source, Node* depot, const unordered_map<int,Node*>& nodes, float beta) {

    // move useful variables to the stack
    auto dists = problem.dists;
    int sourceid = source->id;
    int depotid = depot->id;
    float Tmax = problem.Tmax;
    int n_vehicles = source->n_vehicles;

    // copy problem edges
    vector<Edge*> edges; //(problem.edges);
    std::copy_if(problem.edges.begin(), problem.edges.end(), std::back_inserter(edges), [&](Edge* i) {
        return (nodes.find(i->inode->id) != nodes.end() && nodes.find(i->jnode->id) != nodes.end());
    });

    // init vector of routes
    vector<Route*> routes;


    // build the dummy solution 
    for ( auto iter = cbegin(nodes); iter != cend(nodes); ++iter ){

        Node* node = iter->second;
        node->from_source = dists[source->id][node->id];
        node->to_depot = dists[node->id][depot->id];
        node->link_source = true;
        node->link_depot = true;
        node->route = nullptr;

        if ( node->from_source + node->to_depot <=  Tmax ) {
            Route* newRoute = new Route(source, depot, node);
            node->route = newRoute;
            routes.push_back( newRoute );
        } 
    }


    // sort the savings list 
    sort( edges.begin(), edges.end(), [&]( Edge* iedge, Edge* jedge){
        return iedge->savings[sourceid] > jedge->savings[sourceid];
    });

    // iterative merging process
    while  ( edges.size() > 0 && routes.size() > n_vehicles ) 
    {

        // pick an edge and remove it form the list of alternatives
        int idx = BRA(edges.size(), beta);
        Edge* edge = edges[idx];
        edges.erase( edges.begin() + idx);

        auto inode = edge->inode; 
        auto jnode = edge->jnode;
        auto iroute = inode->route;
        auto jroute = jnode->route;
        
        // routes must be different 
        if (iroute == jroute || iroute == nullptr  ||  jroute == nullptr)
            continue;

        // nodes must be connected in the right way 
        if (!inode->link_depot  ||   !jnode->link_source)
            continue;

        // length of new route doesn't exceed the maximum allowed
        if (iroute->cost + jroute->cost + edge->cost - inode->to_depot - jnode->from_source > Tmax)
            continue;

        // if all conditions are respected, the routes are merged 
        iroute->merge(jroute, edge);
        routes.erase( std::find(routes.begin(), routes.end(), jroute) );

    }

    

  
    // if the number of routes is higher than the number of vehicles, only the bests are kept
    if (routes.size() > n_vehicles) {
        
        sort (routes.begin(), routes.end(), [](Route* iroute, Route* jroute) {
            return iroute->revenue > jroute->revenue;
        });

        routes.resize(n_vehicles);
    }

    // calculate cost and revenue of routes
    float cost = 0.0f;
    int revenue = 0;
    for (auto r : routes) {
        cost += r->cost;
        revenue += r->revenue;
    }

    return new PJS_Solution(routes, revenue, cost);
    
}

#endif