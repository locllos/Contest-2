#include <iostream>
#include <vector>
#include <stack>

using std::vector;
using std::stack;

using std::cin;
using std::cout;

typedef int elem_t;
 
void inputGraphByMatrix(size_t amount_edges, 
                        vector<vector<elem_t>>& graph)
{
    elem_t vertex_a = 0;
    elem_t vertex_b = 0;
    for (size_t i = 0; i < amount_edges; ++i)
    {   
        cin >> vertex_a >> vertex_b;

        graph[vertex_a - 1][vertex_b - 1] = 1;
        graph[vertex_b - 1][vertex_a - 1] = 1;
    }
}

void inputGraphByList(size_t amount_edges, 
                      vector<vector<elem_t>>& graph)
{
    elem_t vertex_a = 0;
    elem_t vertex_b = 0;
    for (size_t i = 0; i < amount_edges; ++i)
    {   
        cin >> vertex_a >> vertex_b;

        graph[vertex_a - 1].push_back(vertex_b - 1);
        graph[vertex_b - 1].push_back(vertex_a - 1);
    }
}

bool isSplitable(elem_t start_vertex, vector<elem_t>& color, vector<vector<elem_t>>& graph)
{
    size_t is_splitable = true;
    for (elem_t current_vertex : graph[start_vertex])
    {
        if (color[current_vertex] == 0)
        {
            color[current_vertex] = 3 - color[start_vertex];
            is_splitable = isSplitable(current_vertex, color, graph); 
        }
        else if (color[current_vertex] ==
                 color[start_vertex])
        {
            is_splitable = false;
        }
    }
    return is_splitable;
}

void Processing()
{
    size_t amount_vertices = 0;
    size_t amount_edges = 0;
    cin >> amount_vertices >> amount_edges;

    vector<vector<elem_t>> graph(amount_vertices);
    vector<elem_t> color(amount_vertices);

    inputGraphByList(amount_edges, graph);

    bool is_splitable = true;
    for (size_t v = 0; v < amount_vertices; ++v)
    {
        if (color[v] == 0) 
        {
            color[v] = 1;
            is_splitable = is_splitable && isSplitable(v, color, graph);
        }
    }
    if (amount_vertices == 1)
    {
        is_splitable = false;
    }
    cout << (is_splitable ? "YES" : "NO");
}

int main()
{
    Processing();

    return 0;
}