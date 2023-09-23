# Shortest-Path-Problem
Reads a directed graph from a file and receives requests for paths between two points. Then shows the path on map.

## Included Files:
	in ./server
		server.cpp
		dijkstra.cpp
		dijkstra.h
		digraph.cpp
		digraph.h
		wdigraph.h
		edmonton-roads-2.0.1.txt
		Makefile

	in ./soln
		README
		Makefile

## Acknowledgements:
    Created as part of the CMPUT275 course at University of Alberta
    Portions of the code are written by Tianyu Zhang
    Help from TA: G. Price

## Running Instructions: 
    Run "make run" in the ./soln terminal. Then, gnome-terminal will be ran and the server will be executed, then the client. Below are the instructions for the plotter:

		- press W/S/A/D to move the patch displayed in the window up/down/left/right respectively.
		- press R to remove all routes and selected points on the map.
		- press Q and E to respectively zoom in and out on the map, keeping the mouse cursor at its previous position.
		- click left mouse button to select current point as the start or end point of a trip.
		- drag left moues button to scroll around on the map.
