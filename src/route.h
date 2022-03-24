#ifndef ROUTE_H
#define ROUTE_H

#include <vector>
#include "node.h"
#include "edge.h"


using std::vector;


class Route {


    public:
    Node* source;
    Node* depot;
    vector<Node*> nodes;
    float cost;
    int revenue;


    Route(Node* source, Node*depot, Node* starting_node) {
        this->source = source;
        this->depot = depot;
        this->nodes.push_back(starting_node);
        this->revenue = starting_node->revenue;
        this->cost = starting_node->from_source + starting_node->to_depot;
    }

    ~Route () = default;


    void merge (Route* other, const Edge* edge) {

        auto inode = edge->inode;
        auto jnode = edge->jnode;

        // update cost 
        cost += other->cost + edge->cost - inode->to_depot - jnode->from_source;
        
        // update revenue
        revenue += other->revenue;

        // update nodes ownership and connections
        inode->link_depot = false;
        jnode->link_source = false;
        for (Node* n : other->nodes){
            n->route = this;
            nodes.push_back(n);
        }

    }

};


#endif