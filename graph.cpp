#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"
#include "graph.hpp"

void Graph::populate_adjacency_matrix() //function to populate adjacency matrix
{
    for(int i = 0; i < edge_list.size(); i++)
    {
        adjacency_matrix[edge_list[i].get_vertex1()][edge_list[i].get_vertex2()] = true;
        adjacency_matrix[edge_list[i].get_vertex2()][edge_list[i].get_vertex1()] = true;
    }
}

Graph::Graph(int new_vertex_count, vector<Edge> new_edge_list) //constructor
{
    this->vertex_count = new_vertex_count;
    this->edge_list = new_edge_list;
    vector<vector<bool>> new_adjacency_matrix(new_vertex_count, vector<bool>(new_vertex_count, false));
    this->adjacency_matrix = new_adjacency_matrix;
    populate_adjacency_matrix();
}

void Graph::find_vertex_cover() //function to find the vertex cover
{
    int k = 1;  // start with a vertex cover of size 1
    bool sat = false;   // assume satisfiability is false initially
    
    while (!sat && k < vertex_count)
    {
        using Minisat::mkLit;
        using Minisat::lbool;
        
        Minisat::Solver solver;
        
        Minisat::Var variables[k][vertex_count];    //create variables
        
        for(int i = 0; i < k; i++)  //initialize variables
        {
            for (int j = 0; j < vertex_count; j++)
            {
                variables[i][j] = solver.newVar();
            }
        }
        
        for(int i = 0; i < k; i++)  //at least one vertex is the ith vertex in the vertex cover
        {
            Minisat::vec<Minisat::Lit> literals;
            for (int j = 0; j < vertex_count; j++)
            {
                literals.push(Minisat::mkLit(variables[i][j]));
            }
            solver.addClause(literals);
        }
        
        for(int m = 0; m < vertex_count; m++)   //no one vertex can appear twice in the vertex cover
        {
            for(int p = 0; p < k-1; p++)
            {
                for(int q = p+1; q < k; q++)
                {
                    solver.addClause(~mkLit(variables[p][m]), ~mkLit(variables[q][m]));
                }
            }
        }
        
        for(int m = 0; m < k; m++)  //no more than one vertex appears in the mth position of the vertex cover
        {
            for(int p = 0; p < vertex_count-1; p++)
            {
                for(int q = p+1; q < vertex_count; q++)
                {
                    solver.addClause(~mkLit(variables[m][p]), ~mkLit(variables[m][q]));
                }
            }
        }
        
        for(int edge_index = 0; edge_index < edge_list.size(); edge_index++)    //every edge is incident to at least one vertex in the vertex cover
        {
            int vertex1 = edge_list.at(edge_index).get_vertex1();
            int vertex2 = edge_list.at(edge_index).get_vertex2();
            
            Minisat::vec<Minisat::Lit> literals;

            for(int i = 0; i < k; i++)
            {
                literals.push(Minisat::mkLit(variables[i][vertex1]));
            }
            
            for(int i = 0; i < k; i++)
            {
                literals.push(Minisat::mkLit(variables[i][vertex2]));
            }
            solver.addClause(literals);
        }
        
        sat = solver.solve();   //find a possible solution if there is one
        
        if(sat)
        {
            vector<int> vertex_cover;
            bool sat_result[k][vertex_count];
            for(int i = 0; i < k; i++)
            {
                for(int j = 0; j < vertex_count; j++)
                {
                    sat_result[i][j] = (solver.modelValue(variables[i][j]) == Minisat::l_True);
                }
            }
            
            for(int i = 0; i < k; i++)  //create vertex cover
            {
                for(int j = 0; j < vertex_count; j++)
                {
                    if(sat_result[i][j])
                    {
                        vertex_cover.push_back(j);
                        break;
                    }
                }
            }
            
            sort(vertex_cover.begin(), vertex_cover.end()); //sort vertex cover

            for(int i = 0; i < vertex_cover.size(); i++)    //output vertex cover
            {
                cout<<vertex_cover.at(i);
                if(i != vertex_cover.size()-1)
                {
                    cout<<" ";
                }
            }
            cout<<endl;
            break;
        }
        k++;
    }
}