#ifndef UTILS_H
#define UTILS_H


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>

#include "node.h"
#include "edge.h"

using namespace std;




class Problem {

  public:
  int n_nodes, n_vehicles;
  float Tmax;
  vector<vector<float>> dists;
  vector<Node*> nodes, sources;
  Node* depot;
  vector<Edge*> edges;

  Problem (int n_nodes, int n_vehicles, float Tmax, vector<vector<float>>& dists,
          vector<Node*>& sources, vector<Node*>& nodes, Node* depot, vector<Edge*>& edges){
      this->n_nodes = n_nodes;
	    this->n_vehicles = n_vehicles;
      this->Tmax = Tmax;
      this->dists = dists;
      this->nodes = nodes;
      this->sources = sources;
      this->depot = depot;
      this->edges = edges;
  }

  ~Problem() {

    for (Node* i : sources)
      delete i;
    
    for (Node* i : nodes)
      delete i;

    for (Edge* i : edges)
      delete i;

    delete depot;

    nodes.clear();
    sources.clear();
    edges.clear();
    
  };
};



Problem read_file ( string filename) {

  // init useful variables
  string line, subline, word;
  int linecount = 0;

  // open file
  ifstream file (filename);



  // init problem characteristics
  int n_nodes, n_vehicles;
  float Tmax;
  vector<vector<float>> dists;
  vector<Node*> nodes;
  vector<Node*> sources;
  Node* depot;
  vector<Edge*> edges;




  if (file.is_open())
  {
    while ( getline (file, line) )
    {
      stringstream streamline(line);
      vector<string> info;
      int id, revenue, v;
      float x, y;
      bool issource, isdepot;

      while (getline(streamline, subline, '\t'))
      {
        stringstream streamsubline(subline);
        while (getline(streamsubline, word, ' ') )
        {
          info.push_back(word);
        }
      }

      if (linecount == 0) { n_nodes = stoi(info[1]);}
      else if (linecount == 1) { n_vehicles = stoi(info[1]);}
      else if (linecount == 2) { Tmax = stof(info[1]);}
      else if (linecount == n_nodes + 2) {
        depot = new Node(linecount - 3, stof(info[0]), stof(info[1]), stoi(info[2]), false, true, 0);
      }
      else {
        if (info[3] == "1") {
          sources.push_back(new Node(linecount - 3, stof(info[0]), stof(info[1]), stoi(info[2]), true, false, stoi(info[4])));
        } else {
          nodes.push_back(new Node(linecount - 3, stof(info[0]), stof(info[1]), stoi(info[2]), false, false, 0));
        }
      }

      linecount++;
    }
    file.close();
  }

  
  // make the vector of all the nodes
  vector<Node*> allnodes (sources);
  allnodes.insert(allnodes.end(), nodes.begin(), nodes.end());
  allnodes.push_back(depot);

  // make matrix of distances
  for (int i = 0; i < allnodes.size(); i++) {
    Node* inode = allnodes[i];

    vector<float> row (allnodes.size());
    dists.push_back(row);

    for (int j = 0; j < allnodes.size(); j++) {
      Node* jnode = allnodes[j];

      dists[i][j] = sqrt( pow( inode->x - jnode->x , 2 )   +    pow( inode->y - jnode->y , 2));

      if (inode->issource || inode->isdepot || jnode->issource || jnode->isdepot)
          continue;
      
      Edge* edge = new Edge(inode, jnode, dists[i][j]);
      edges.push_back(edge);
    }
  }

  return Problem(n_nodes, n_vehicles, Tmax, dists, sources, nodes, depot, edges);

}








Problem read_single_source (string filename ) {


  // init useful variables
  string line, subline, word;
  int linecount = 0;

  // open file
  ifstream file (filename);



  // init problem characteristics
  int n_nodes, n_vehicles;
  float Tmax;
  vector<vector<float>> dists;
  vector<Node*> nodes;
  vector<Node*> sources;
  Node* depot;
  vector<Edge*> edges;




  if (file.is_open())
  {
    while ( getline (file, line) )
    {
      stringstream streamline(line);
      vector<string> info;
      int id, revenue, v;
      float x, y;
      bool issource, isdepot;

      while (getline(streamline, subline, '\t'))
      {
        stringstream streamsubline(subline);
        while (getline(streamsubline, word, ' ') )
        {
          info.push_back(word);
        }
      }

      if (linecount == 0) { n_nodes = stoi(info[1]);}
      else if (linecount == 1) { n_vehicles = stoi(info[1]);}
      else if (linecount == 2) { Tmax = stof(info[1]);}
      else if (linecount == n_nodes + 2) {
        depot = new Node(linecount - 3, stof(info[0]), stof(info[1]), stoi(info[2]), false, true, 0);
      } else if (linecount == 3) {
        sources.push_back(new Node(linecount - 3, stof(info[0]), stof(info[1]), stoi(info[2]), true, false, n_vehicles));
      } else {
          nodes.push_back(new Node(linecount - 3, stof(info[0]), stof(info[1]), stoi(info[2]), false, false, 0));  
      }

      linecount++;
    }
    file.close();
  }

  
  // make the vector of all the nodes
  vector<Node*> allnodes (sources);
  allnodes.insert(allnodes.end(), nodes.begin(), nodes.end());
  allnodes.push_back(depot);

  // make matrix of distances
  for (int i = 0; i < allnodes.size(); i++) {
    Node* inode = allnodes[i];

    vector<float> row (allnodes.size());
    dists.push_back(row);

    for (int j = 0; j < allnodes.size(); j++) {
      Node* jnode = allnodes[j];

      dists[i][j] = sqrt( pow( inode->x - jnode->x , 2 )   +    pow( inode->y - jnode->y , 2));

      if (inode->issource || inode->isdepot || jnode->issource || jnode->isdepot)
          continue;
      
      Edge* edge = new Edge(inode, jnode, dists[i][j]);
      edges.push_back(edge);
    }
  }

  return Problem(n_nodes, n_vehicles, Tmax, dists, sources, nodes, depot, edges);

}

#endif
