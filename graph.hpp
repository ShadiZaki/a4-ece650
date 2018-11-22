#ifndef graph_hpp
#define graph_hpp

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include "edge.hpp"
using namespace std;

class Graph
{
private:
    int vertex_count;   //number of vertices in graph
    vector<Edge> edge_list; //edges in graph
    vector<vector<bool>> adjacency_matrix;  //adjacency matrix
    void populate_adjacency_matrix();   //function to populate adjacency matrix
public:
    Graph(int new_vertex_count, vector<Edge> new_edge_list); //constructor
    void find_vertex_cover();   //function to find the vertex cover
};

#endif
