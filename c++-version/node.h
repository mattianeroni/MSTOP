#ifndef NODE_H
#define NODE_H


#include <string>
#include <sstream>
#include <vector>
#include <utility>
#include <unordered_map>


using std::pair;
using std::vector;
using std::unordered_map;


class Route;



class Node {


    public:

    // generic attributes of the node
    int id, revenue, n_vehicles;
    float x, y;
    bool issource;
    bool isdepot;

    // attributes used by the Mapper
    unordered_map<int, Node*> nodes;
    vector<pair<float, Node*>>  preferences;
    bool assigned;


    // attributes used by the PJS
    bool link_source, link_depot;
    float from_source, to_depot;
    Route* route;

    
    
    public:

    // constructors 
    Node () = default;

    Node(int id, float x, float y, int revenue, bool issource, bool isdepot, int n_vehicles) {
      this->id = id;
      this->x = x; 
      this->y = y;
      this->revenue = revenue;
      this->issource = issource;
      this->isdepot = isdepot;
      this->n_vehicles = n_vehicles;
    };

    // descructor
    ~Node() = default;

    // representation
    std::string __repr__ () {
      std::stringstream ss;
      ss << "Node " << id;
      return ss.str();
    };


};

#endif