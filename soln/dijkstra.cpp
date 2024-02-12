/*----------------------------------------------------------------------------------------------------
    Name: Ayra Qutub
    SID: 1708104
    CCID: aqutub
    CMPUT 275, Winter 2023
    Assignment 1
----------------------------------------------------------------------------------------------------*/
#include <iostream>
#include <unordered_map>
#include <list>
#include <utility> 
#include <queue>
#include "wdigraph.h"
#include "dijkstra.h"

using namespace std;
// Defined new data types using C++ typedef
typedef pair<int, long long> PIL; 
typedef pair<int, PIL> PIPIL;

// Defined a custom comparison function for priority_queue
struct compare { 
    bool operator()(const PIPIL& left, const PIPIL& right) const {
        return (left.second.second > right.second.second);
    }
};

/*----------------------------------------------------------------------------------------------------
    Description: 
        Implements Dijkstra's algorithm to find the shortest path from a given starting vertex to 
        all other vertices in a weighted directed graph. The function updates an unordered map, 'tree', with the shortest distances and corresponding parent vertices.

    Arguments:
        const WDigraph& graph: a weighted directed graph represented using an adjacency list.
        int startVertex: the vertex from which the shortest paths are to be computed.
        unordered_map<int, PIL>& tree: a reference to an unordered map where the computed shortest distances and parent vertices will be stored.
    Returns: 
        N/A
----------------------------------------------------------------------------------------------------*/
void dijkstra(const WDigraph& graph, int startVertex, unordered_map<int, PIL>& tree) {
    // Create a priority queue for PIPIL values
    priority_queue<PIPIL, vector<PIPIL>, compare> queue;
    // Push the starting vertex and a self-pair (startVertex, 0) to the queue
    queue.push(PIPIL(startVertex, PIL(startVertex, 0)));
    // Run the loop while queue is not empty
    while (queue.size() != 0){
        // Get the top element from the priority queue
        PIPIL queue_top = queue.top();
        // Extract the vertex, parent vertex, and distance values
        int u = queue_top.first; 
        int v = queue_top.second.first; 
        long long d = queue_top.second.second;
        // Remove this element from the queue
        queue.pop();
        // If the vertex is already visited, skip it
        if (tree.find(v) == tree.end()) {
            // Update the shortest distance from startVertex to this vertex, and the corresponding parent vertex
            if (u != v){
                tree[v] = PIL(u, graph.getCost(u, v));
            }
            else{
                tree[v] = PIL(u, 0);
            }
            // For each neighbour of the current vertex, calculate the time of burn of the edge (v, neighbour) and push it to the priority queue
            for(auto neighbour = graph.neighbours(v); neighbour != graph.endIterator(v); neighbour++) {
                // Calculate the time when this neighbour will be reached by fire
                long long time = d + graph.getCost(v, *neighbour);
                // Push the (neighbour, (v, time)) pair to the priority queue
                queue.push(PIPIL(v, PIL(*neighbour, time)));
            }       
        }
    }
}
