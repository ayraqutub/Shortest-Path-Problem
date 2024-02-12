/*----------------------------------------------------------------------------------------------------
    Name: Ayra Qutub
    SID: 1708104
    CCID: aqutub
    CMPUT 275, Winter 2023
    Assignment 1
----------------------------------------------------------------------------------------------------*/
#include "dijkstra.h"
#include "wdigraph.h"
#include <list>
#include <string>
#include <climits>
#include <cmath>
#include <fstream>
using namespace std;

struct Point {
    long long lat; // latitude of the point
    long long lon; // longitude of the point
};

long long manhattan(const Point& pt1, const Point& pt2){
    long long distance = abs(pt1.lat - pt2.lat) + abs(pt1.lon - pt2.lon);
    return distance;
}
/*----------------------------------------------------------------------------------------------------
    Description: 
        Reads a file containing graph data in the following format:
        V, vertex_id, latitude, longitude
        E, start_vertex_id, end_vertex_id
        and populates a WDigraph and an unordered_map of Points with the data.

    Arguments:
        - filename: a string containing the name of the file to be read
        - graph: a reference to a WDigraph object to be populated with vertex and edge data
        - points: a reference to an unordered_map object to be populated with Points associated with vertex IDs
    Returns: 
        N/A
----------------------------------------------------------------------------------------------------*/
void read_graph(const string& filename, WDigraph& graph, unordered_map<int, Point>& points) {
    ifstream file(filename); // open file for input
    string line; // string to hold each line of file
    while (getline(file, line)) { // loop over lines in file
        if (line.empty()) continue; // skip empty lines

        char cmd = line[0]; // get command character ('V' or 'E')
        if (cmd == 'E') { // if command is to add an edge
            vector<string> parts; // vector to hold each part of the line
            string part;
            // loop over each character in line starting from index 2 (skipping 'E,')
            for (size_t i = 2; i < line.size(); ++i) {
                if (line[i] == ',') {// if character is a comma, add current part to vector and start new part
                    parts.push_back(part);
                    part.clear();
                } else {
                    part += line[i]; // add character to current part
                }
            }
            parts.push_back(part); // add final part to vector

            int start = stoi(parts[0]); // convert start vertex ID from string to integer
            int end = stoi(parts[1]); // convert end vertex ID from string to integer

            graph.addEdge(start, end, manhattan(points[start], points[end])); // add edge to WDigraph with distance calculated using manhattan function
        }
        else if (cmd == 'V') { // if command is to add a vertex
            vector<string> parts; // vector to hold each part of the line
            string part;
            // loop over each character in line starting from index 2 (skipping 'V,')
            for (size_t i = 2; i < line.size(); ++i) {
                if (line[i] == ',') { // if character is a comma, add current part to vector and start new part
                    parts.push_back(part);
                    part.clear();
                } else {
                    part += line[i]; // add character to current part
                }
            }
            parts.push_back(part); // add final part to vector

            int id = stoll(parts[0]); // convert ID from string to integer
            double lat = stof(parts[1]); // convert latitude from string to long long
            double lon = stof(parts[2]); // convert longitude from string to long long

            long long ll_lat = static_cast<long long>(lat * 100000); // convert latitude from double to long long
            long long ll_lon = static_cast<long long>(lon * 100000); // convert longitude from double to long long

            Point point; // create Point object
            point.lat = ll_lat; // set latitude of Point object
            point.lon = ll_lon; // set longitude of Point object

            points[id] = point;// add Point object to unordered_map with ID as key
            graph.addVertex(id); // add vertex to WDigraph
        } 
    }
}

/*----------------------------------------------------------------------------------------------------
    Description: 
        Finds the ID of the point in the unordered map that is closest to a given point.

    Arguments:
        - pt1: A constant reference to a Point object representing the point for which we want to 
        find the closest point in the unordered map.
        - points: A constant reference to an unordered_map object with integer keys and Point values 
        representing the collection of points among which we want to find the closest one.
    Returns: 
        - An integer representing the ID of the closest point in the unordered map to the given point. 
        If there are no points in the unordered map, the function returns -1 indicating failure.
----------------------------------------------------------------------------------------------------*/
int closest(const Point& pt1, const std::unordered_map<int, Point>& points) {
    int id = -1; // initialize closest id to a value indicating failure
    long long closest_distance = LLONG_MAX; // initialize closest_distance to the largest possible distance a long long can hold

    for (auto pair : points) { // range-based for loop over the unordered_map
        int current_id = pair.first;
        Point point = pair.second;
        long long distance = manhattan(point, pt1);
        if (distance < closest_distance) { // check if the distance from the current point and the user point is smaller than the closest distance
            closest_distance = distance; // update the closest distance
            id = current_id; // set the closest id to the id of the current point
        }
    }

    return id;
}

/*----------------------------------------------------------------------------------------------------
    Description: 
        This is the main function of a program that reads a graph from a file, reads input from the user, 
        finds the closest vertices to start and end points, runs Dijkstra's algorithm to find the shortest 
        path, and prints the results.

    Arguments:
        N/A
    Returns: 
        The number of waypoints and the coordinates of each waypoint
----------------------------------------------------------------------------------------------------*/
int main() {
    // Initialize variables
    WDigraph graph;
    string request, input;
    long long lat1, lon1, lat2, lon2;
    unordered_map<int, PIL> searchTree;
    unordered_map<int, Point> points;

    // Read graph from file
    read_graph("edmonton-roads-2.0.1.txt", graph, points);
    // Read input
    cin >> request >> lat1 >> lon1 >> lat2 >> lon2;

    // Find closest vertices to start and end points
    Point start_point {lat1, lon1};
    Point end_point {lat2, lon2};
    int closest_vertex1 = closest(start_point, points);
    int closest_vertex2 = closest(end_point, points);
    // Run Dijkstra's algorithm to find shortest path
    dijkstra(graph, closest_vertex1, searchTree);



    // Print results
    list<int> path;
    if (searchTree.find(closest_vertex2) != searchTree.end()) {
        // Construct path and print nodes
        int stepping = closest_vertex2;
        while (stepping != closest_vertex1) {
            path.push_front(stepping);
            stepping = searchTree[stepping].first;
        }
        path.push_front(closest_vertex1);

        cout << "N " << path.size() << endl;
        for (auto it : path) {
            cin >> input;
            if (input == "A"){
                cout << "W " << points[it].lat << " " << points[it].lon << endl;
            }
        }
        cout << "E" << endl;
    } 
    else {
        // No path found
        cout << "N 0" << endl;
        cout << "E" << endl;
    }

    return 0;
}

