#ifndef EDGE_H
#define EDGE_H


#include <unordered_map>
#include "node.h"


using std::unordered_map;



class Edge {
    public:

    Node* inode;
    Node* jnode;
    float cost;
    unordered_map<int, float> savings;

    Edge(Node* inode, Node* jnode, float cost): inode(inode), jnode(jnode), cost(cost) {}
    ~Edge() = default;

};

#endif