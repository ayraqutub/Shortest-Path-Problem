TARGET = server
CC = g++
CFLAGS = -std=c++11 -g
OBJ = dijkstra.o server.o digraph.o

all: server

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ)

server.o: server.cpp
	$(CC) $(CFLAGS) -c server.cpp

digraph.o: digraph.cpp
	$(CC) $(CFLAGS) -c digraph.cpp 

dijkstra.o: dijkstra.cpp
	$(CC) $(CFLAGS) -c dijkstra.cpp

clean: 
	rm -f server *.o