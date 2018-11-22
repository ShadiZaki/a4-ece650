#include "parser.hpp"
#include "edge.hpp"
#include "graph.hpp"
using namespace std;

int main(int argc, const char * argv[])
{
    int vertex_count = 0;   //number of vertices
    vector<Edge> edge_list; //vector to store edges
    string command = "    ";
    bool v_specified = false;   //determines if the number of vertices has been specified
    
    do{
        Parser parser;  //parser instance
        pair<string, vector<int>> parser_result;    //result of parser (command, parameters)
        vector<int> parameters;     //input parameters
        
        parser_result = parser.read_and_parse();    //read and parse input
        command = parser_result.first;  //splitting parser result into command and parameters
        parameters = parser_result.second;
        
        
        if(command == "V" || command == "v")    //check if command is a vertex number specification
        {
            if(parameters.size() == 1)  //check if there is only one parameter (vertex count)
            {
                vertex_count = parameters[0];
                v_specified = true;
            }
        }
        else if(command == "E" || command == "e")   //check if command is an edges specification
        {
            if(v_specified) //check if the number of vertices has been specified
            {
                if(parameters.size() % 2 == 0)  //check if the number of parameters is even (because edges are specified as vertex pairs)
                {
                    bool valid_edges = true;    //determines if edge specification is valid
                    
                    for(int i = 0; i < parameters.size(); i+=2)
                    {
                        int vertex1 = parameters[i];
                        int vertex2 = parameters[i+1];
                        if(vertex1 < 0 || vertex1 >= vertex_count || vertex2 < 0 || vertex2 >= vertex_count || vertex1 == vertex2) //check if the vertices are not within range or equal to each other
                        {
                            valid_edges = false;
                            break;
                        }
                    }
                    
                    edge_list.clear();  //clear previous edges
                    
                    if(valid_edges) //check if edges are valid
                    {
                        for(int i = 0; i < parameters.size(); i+=2) //push every edge into the edges vector
                        {
                            int vertex1 = parameters[i];
                            int vertex2 = parameters[i+1];
                            Edge new_edge(vertex1, vertex2);
                            edge_list.push_back(new_edge);
                        }
                        Graph graph(vertex_count, edge_list);
                        graph.find_vertex_cover();
                    }
                    else    //invalid edges
                    {
                        cerr<<"Error: invalid edges"<<endl;
                    }
                }
            }
            else   //number of vertices was not specified
            {
                cerr<<"Error: number of vertices was not specified"<<endl;
            }
        }
    }while(command != "");
    return 0;
}
