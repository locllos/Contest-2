#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <queue>

using std::string;
using std::queue;
using std::stack;
using std::vector;
using std::cin;
using std::cout;
using std::endl;

typedef int elem_t;

struct ExtentedVertex
{
    size_t condensation_num;
    elem_t vertex;
};

using Graph = vector<vector<elem_t>>;

void inputGraphByList(size_t amount_edges,
                      Graph& graph,
                      Graph& transpose_graph)
{
    elem_t vertex_a = 0;
    elem_t vertex_b = 0;
    for (size_t i = 0; i < amount_edges; ++i)
    {
        cin >> vertex_a >> vertex_b;

        graph[vertex_a - 1].push_back(vertex_b - 1);
        transpose_graph[vertex_b - 1].push_back(vertex_a - 1);
    }
}

void FillExitTimes(elem_t start_vertex, 
                   vector<bool>& visited, Graph& graph, stack<elem_t>& vertex_exits)
{
    visited[start_vertex] = true;
    for (elem_t current_vertex : graph[start_vertex])
    {
            if (!visited[current_vertex])
            {   
                FillExitTimes(current_vertex, visited, graph, vertex_exits);
            }
    }
    vertex_exits.push(start_vertex);
}

void FillCondensation(elem_t start_vertex, size_t condensation_num, 
                      vector<bool>& visited, Graph& graph, vector<size_t>& condensation)
{
    visited[start_vertex] = true;
    condensation[start_vertex] = condensation_num;

    for (elem_t current_vertex : graph[start_vertex])
    {
        if (!visited[current_vertex])
        {
            FillCondensation(current_vertex, 
                             condensation_num, visited, graph, condensation);
        }
    }
}

void Processing()
{
    size_t amount_vertices = 0;
    size_t amount_edges = 0;
    cin >> amount_vertices >> amount_edges;

    Graph graph(amount_vertices);
    Graph transpose_graph(amount_vertices);
    inputGraphByList(amount_edges, graph, transpose_graph);

    stack<elem_t> vertex_exits;
    vector<bool> visited(amount_vertices);
    for (size_t i = 0; i < amount_vertices; ++i)
    {
        if (!visited[i])
        {
            FillExitTimes(i, visited, graph, vertex_exits);
        }
    }
    
    size_t condensation_num = 1;
    vector<size_t> condensation(amount_vertices, 0);
    fill(visited.begin(), visited.end(), false);
    while (!vertex_exits.empty())
    {
        elem_t current_vertex = vertex_exits.top();
        vertex_exits.pop();
        if (!visited[current_vertex])
        {
            FillCondensation(current_vertex, condensation_num, visited, transpose_graph, condensation);
            ++condensation_num;
        }
    }

    cout << condensation_num - 1 << endl;
    for (elem_t vertex : condensation)
    {   
        cout << vertex << ' ';
    }
    cout << endl;
}

int main()
{
    Processing();

    return 0;
}