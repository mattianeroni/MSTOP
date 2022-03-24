#ifndef MAPPER_H
#define MAPPER_H

#include <vector>
#include <limits>
#include <utility>
#include <math.h>
#include <unordered_map>
#include <algorithm>
#include <iostream>

#include "utils.h"
#include "node.h"
#include "bra.h"


using namespace std;
using Mapping = vector<vector<int>>;



Mapping mapper (const Problem& problem, float beta) {

    // move useful variables to the stack 
    auto dists = problem.dists;
    int N = problem.nodes.size();
    int S = problem.sources.size();
    auto sources = problem.sources;
    auto nodes = problem.nodes;
    auto depot = problem.depot;

    // init the mapping
    Mapping mapping;

    // reset the previous solutions
    for (Node* source : sources) {

        source->preferences = vector<pair<float, Node*>>();
        source->nodes = unordered_map<int, Node*>();
        mapping.push_back(vector<int>(N + S, 0));

    }


    // compose list of preferences for each source
    for (Node* node : nodes ) {

        node->assigned = false;

        for (Node* source : sources) {
            
            // get absolute distance
            float absolute = dists[source->id][node->id];

            // find the best other source 
            float m = numeric_limits<float>::max();
            for (Node* other : sources) {
                if (other != source) 
                    m = min( m, dists[other->id][node->id] );
            }

            source->preferences.push_back( pair<float,Node*>( absolute - m,  node  ) );

        }

    }

    // sort preferences 
    for (Node* source : sources) {
        sort(source->preferences.begin(), source->preferences.end());
    }

    // round-robin assignment process
    int total_assigned = 0;
    int csource = 0;

    
    while (total_assigned < N) {

        Node* current_source = sources[csource];

        int n_assigned = 0;

        while ( n_assigned < current_source->n_vehicles   &&   total_assigned < N    &&   current_source->preferences.size() > 0 ) {
                
            int idx = BRA(current_source->preferences.size(), beta);
            auto pref = current_source->preferences[idx];
            auto picked_node = pref.second;
            current_source->preferences.erase( current_source->preferences.begin() + idx  );
            
            if (!picked_node->assigned){
                picked_node->assigned = true;
                n_assigned++;
                total_assigned++;
                current_source->nodes[picked_node->id] = picked_node;
                mapping[current_source->id][picked_node->id] = 1;
            }

        }

        // next source is considered
        csource++;
        if (csource == S) 
            csource = 0;
    }


    return mapping;
}


#endif