#ifndef ACTORGRAPH_HPP
#define ACTORGRAPH_HPP

#include <iostream>
#include <unordered_map>
#include <vector>

#include "Node.hpp"

using namespace std;

class ActorGraph {
  protected:
    // Title, Node*
    unordered_map<string, Node*> graph;

  public:
    /* TODO */
    ActorGraph();

    /* TODO */
    bool buildGraph(istream& is);

    /* TODO */
    void BFS(const string& fromActor, const string& toActor,
             string& shortestPath);

    /* TODO */
    ~ActorGraph();

    // Check if a path exists
    // bool pathExists(string from, string to, string movie);
};

#endif  // ACTORGRAPH_HPP
