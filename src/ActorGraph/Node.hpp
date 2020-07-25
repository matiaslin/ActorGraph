#ifndef NODE_HPP
#define NODE_HPP
#include <string>
#include <unordered_map>

using namespace std;

class Node {
  public:
    string name;                                                   // Actor name
    int distance;                                                  // Distance
    unordered_map<string, unordered_map<string, Node*>> outgoing;  // Edges
    bool visited;                                                  // BFS

    Node(string name) : name(name), visited(false) { this->distance = 0; }
};
#endif