#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include <list>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <cstring>

#include "wdigraph.h"
#include "dijkstra.h"

struct Point {
    long long lat, lon;
};

// return the manhattan distance between the two points
long long manhattan(const Point& pt1, const Point& pt2) {
  long long dLat = pt1.lat - pt2.lat, dLon = pt1.lon - pt2.lon;
  return abs(dLat) + abs(dLon);
}

// find the ID of the point that is closest to the given point "pt"
int findClosest(const Point& pt, const unordered_map<int, Point>& points) {
  pair<int, Point> best = *points.begin();

  for (const auto& check : points) {
    if (manhattan(pt, check.second) < manhattan(pt, best.second)) {
      best = check;
    }
  }
  return best.first;
}

// read the graph from the file that has the same format as the "Edmonton graph" file
void readGraph(const string& filename, WDigraph& g, unordered_map<int, Point>& points) {
  ifstream fin(filename);
  string line;
  // read each line from the file
  while (getline(fin, line)) {
    // split the string around the commas, there will be 4 substrings either way
    string p[4];
    int at = 0;
    for (auto c : line) {
      if (c == ',') {
        // start new string
        ++at;
      }
      else {
        // append character to the string we are building
        p[at] += c;
      }
    }
    // check if there are exactly 3 substrings
    if (at != 3) {
      // empty line
      break;
    }

    // process the line based on the first substring
    if (p[0] == "V") {
      // new Point
      int id = stoi(p[1]);
      assert(id == stoll(p[1])); // sanity check: asserts if some id is not 32-bit
      points[id].lat = static_cast<long long>(stod(p[2])*100000);
      points[id].lon = static_cast<long long>(stod(p[3])*100000);
      g.addVertex(id);
    }
    else {
      // new directed edge
      int u = stoi(p[1]), v = stoi(p[2]);
      g.addEdge(u, v, manhattan(points[u], points[v]));
    }
  }
}

// create and open a fifo special file for communication with the plotter app
int create_and_open_fifo(const char * pname, int mode) {
  // create a fifo special file in the current working directory with
  // read-write permissions for communication with the plotter app
  // both proecsses must open the fifo before they perform I/O operations
  // Note: pipe can't be created in a directory shared between 
  // the host OS and VM. Move your code outside the shared directory
  if (mkfifo(pname, 0666) == -1) {
    cout << "Unable to make a fifo. Make sure the pipe does not exist already!" << endl;
    cout << errno << ": " << strerror(errno) << endl << flush;
    exit(-1);
  }

  // opening the fifo for read-only or write-only access
  // a file descriptor that refers to the open file description is
  // returned
  int fd = open(pname, mode);

  if (fd == -1) {
    cout << "Error: failed on opening named pipe." << endl;
    cout << errno << ": " << strerror(errno) << endl << flush;
    exit(-1);
  }

  return fd;
}

// keep in mind that in part 1, the program should only handle 1 request
// in part 2, you need to listen for a new request the moment you are done
// handling one request
int main() {
  WDigraph graph;
  unordered_map<int, Point> points;

  const char *inpipe = "inpipe";
  const char *outpipe = "outpipe";

  // Open the two pipes
  int in = create_and_open_fifo(inpipe, O_RDONLY);
  cout << "inpipe opened..." << endl;
  int out = create_and_open_fifo(outpipe, O_WRONLY);
  cout << "outpipe opened..." << endl;  

  // build the graph
  readGraph("server/edmonton-roads-2.0.1.txt", graph, points);

  // read a request 
  // Help from TA: G. Price , code adapted from ReadGraph function
  //char c; 
  Point sPoint, ePoint;
  while (true){
    char buffer[22]; 
    read(in, buffer, 22);
    if (buffer[0] == 'Q'){
      break;
    }
    string p[2];
      int at = 0;
      for (auto c : buffer) {
        if (c == ' ') {
          // start new string
          ++at;
        }
        else {
          // append character to the string we are building
          p[at] += c;
        }
      }
      sPoint.lat = static_cast<long long>(stod(p[0]) * 100000);
      sPoint.lon =  static_cast<long long>(stod(p[1]) * 100000);
    read(in, buffer, 22);
    if (buffer[0] == 'Q'){
      break;
    }
    string q[2];
      at = 0;
      for (auto c : buffer) {
        if (c == ' ') {
          // start new string
          ++at;
        }
        else {
          // append character to the string we are building
          q[at] += c;
        }
      }
      ePoint.lat = static_cast<long long>(stod(q[0]) * 100000);
      ePoint.lon =  static_cast<long long>(stod(q[1]) * 100000);
  
    // get the points closest to the two points we read
    int start = findClosest(sPoint, points), end = findClosest(ePoint, points);
  
    // run dijkstra's algorithm, this is the unoptimized version that
    // does not stop when the end is reached but it is still fast enough
    unordered_map<int, PIL> tree;
    dijkstra(graph, start, tree);
  
    // NOTE: in Part II you will use a different communication protocol than Part I
    // So edit the code below to implement this protocol
  
    
    if (tree.find(end) != tree.end()) {
      // read off the path by stepping back through the search tree
      list<int> path;
      while (end != start) {
        path.push_front(end);
        end = tree[end].first;
      }
      path.push_front(start);
  
      // output the path **
      for (int v : path) {
         double lat = (double)points[v].lat / 100000;
         double lon = (double)points[v].lon / 100000;
         string output = to_string(lat) + " " + to_string(lon) + "\n"; //TA: G. Price
         cout << output <<endl;
         write(out, output.c_str(), output.size());
      }
    }
    string E = "E\n";
    write(out, E.c_str(), E.size());
  }
  close(in);
  close(out);

  unlink(inpipe);
  unlink(outpipe);
  return 0;
}
