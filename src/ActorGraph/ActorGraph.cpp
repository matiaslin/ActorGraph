#include "ActorGraph.hpp"

#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <stack>
#include <string>

#include "Node.hpp"

using namespace std;

// Initializing an empty graph
ActorGraph::ActorGraph() { this->graph; }

/* Build the actor graph from dataset file.
 * Each line of the dataset file must be formatted as:
 * ActorName <tab> MovieName <tab> Year
 * Two actors are connected by an undirected edge if they have worked in a movie
 * before.
 */
bool ActorGraph::buildGraph(istream& is) {
    bool readHeader = false;
    unordered_map<string, vector<Node*>> movies;

    while (is) {
        string s;
        if (!getline(is, s)) break;
        // skip the header of the file
        if (!readHeader) {
            readHeader = true;
            continue;
        }
        // read each line of the dataset to get the movie actor relation
        istringstream ss(s);
        vector<string> record;
        while (ss) {
            string str;
            if (!getline(ss, str, '\t')) break;
            record.push_back(str);
        }
        // if format is wrong, skip current line
        if (record.size() != 3) {
            continue;
        }
        // extract the information
        string actor(record[0]);
        string title(record[1]);
        int year = stoi(record[2]);
        // Append title with year
        string completeTitle = title + "#@" + record[2];
        // Helper u_map
        if (movies.find(completeTitle) == movies.end()) {
            Node* curr;
            if (this->graph.find(actor) == this->graph.end()) {
                curr = new Node(actor);
            } else {
                curr = this->graph.find(actor)->second;
            }

            vector<Node*> list;
            list.push_back(curr);
            unordered_map<string, Node*> map;
            movies.insert({completeTitle, list});
            curr->outgoing.insert({completeTitle, map});
            // Found the movie title and year
        } else {
            vector<Node*> list = movies[completeTitle];
            // Check if actor is already in the list
            bool found = false;
            for (Node* curr : list) {
                if (curr->name == actor) {
                    found = true;
                }
            }
            // Actor is not in the list
            if (!found) {
                Node* curr;
                if (this->graph.find(actor) == this->graph.end()) {
                    curr = new Node(actor);
                } else {
                    curr = this->graph.find(actor)->second;
                }

                for (Node* tmp : list) {
                    // If movie is not in this actor's node
                    if (curr->outgoing.find(completeTitle) ==
                        curr->outgoing.end()) {
                        unordered_map<string, Node*> tmpMap;
                        tmpMap.insert({tmp->name, tmp});
                        curr->outgoing.insert({completeTitle, tmpMap});
                    } else {
                        curr->outgoing[completeTitle].insert({tmp->name, tmp});
                    }
                    curr->neighbors.push_back(tmp);
                    tmp->outgoing[completeTitle].insert({curr->name, curr});
                    tmp->neighbors.push_back(curr);
                }
                movies[completeTitle].push_back(curr);
            }
        }
        // If it does not exist
        if (this->graph.find(actor) == this->graph.end()) {
            // Check movie title
            if (movies.find(completeTitle) != movies.end()) {
                for (Node* curr : movies[completeTitle]) {
                    if (curr->name == actor) {
                        // insert into graph
                        this->graph.insert({actor, curr});
                    }
                }
            }
        }
    }
    // if failed to read the file, clear the graph and return
    if (!is.eof()) {
        this->~ActorGraph();
        return false;
    }
    return true;
}

/* TODO */
void ActorGraph::BFS(const string& fromActor, const string& toActor,
                     string& shortestPath) {
    // If the actors don't exist
    if (this->graph.find(toActor) == this->graph.end() ||
        this->graph.find(fromActor) == this->graph.end()) {
        shortestPath = "";
        return;
    }
    // reset distance and visited
    for (pair<string, Node*> it : graph) {
        it.second->distance = 0;
        it.second->visited = false;
    }
    // initialize empty queue
    queue<pair<int, Node*>> path;
    // push the current node with zero distance
    pair<int, Node*> root(0, this->graph[fromActor]);
    path.push(root);
    while (!path.empty()) {
        int distance = path.front().first;
        Node* curr = path.front().second;
        path.pop();
        if (!curr->visited) {
            curr->visited = true;
            curr->distance = distance;
            // Loopping through neighbors
            for (auto nodes : curr->neighbors) {
                if (!nodes->visited) {
                    path.push({distance + 1, nodes});
                }
            }
        }
    }
    // Finding the shortest path
    stack<string> shortest;
    Node* curr = this->graph[toActor];
    int isBroken = false;
    // if not the first actor yet
    while (curr->name != fromActor) {
        if (curr->distance == 0 && curr->name != fromActor) {
            shortestPath = "";
            return;
        }
        // loop through movies
        for (auto movies : curr->outgoing) {
            // loop through coworkers
            for (auto cw : movies.second) {
                if (cw.second->distance == curr->distance - 1) {
                    // push to stack
                    shortest.push(movies.first);
                    shortest.push(cw.first);
                    curr = cw.second;

                    // break
                    isBroken = true;
                    break;
                }
            }

            if (isBroken) {
                isBroken = false;
                break;
            }
        }
    }
    // Building the shortest path
    while (!shortest.empty()) {
        string tmp = "(" + shortest.top() + ")";
        shortest.pop();
        tmp += "--[" + shortest.top() + "]-->";
        shortest.pop();
        shortestPath += tmp;
    }
    shortestPath += "(" + toActor + ")";
}

// Destroy the list
ActorGraph::~ActorGraph() {
    for (auto it = this->graph.begin(); it != this->graph.end(); it++) {
        delete (it->second);
    }
}
