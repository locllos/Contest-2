#include <iostream>
#include <vector>
#include <stack>
#include <queue>

using std::queue;
using std::stack;
using std::vector;
using std::cin;
using std::cout;
using std::endl;

typedef int elem_t;

void inputGraphByMatrix(size_t amount_edges,
                        vector<vector<elem_t>> &graph)
{
    elem_t vertex_a = 0;
    elem_t vertex_b = 0;
    for (size_t i = 0; i < amount_edges; ++i)
    {   
        cin >> vertex_a >> vertex_b;

        graph[vertex_a - 1][vertex_b - 1] = 1;
    }
}

void inputGraphByList(size_t amount_edges,
                      vector<vector<elem_t>> &graph)
{
    elem_t vertex_a = 0;
    elem_t vertex_b = 0;

    for (size_t i = 0; i < amount_edges; ++i)
    {   
        cin >> vertex_a >> vertex_b;

        graph[vertex_a - 1].push_back(vertex_b - 1);
    }
}

bool DFS(elem_t start_vertex, vector<elem_t>& color, vector<vector<elem_t>>& graph, stack<elem_t>& sorted)
{
    color[start_vertex] = 1;
    for (elem_t current_vertex : graph[start_vertex])
    {
            if (!color[current_vertex])
            {   
                if (DFS(current_vertex, color, graph, sorted))
                {
                    return true;
                }
            }
            if (color[current_vertex] == 1)
            {   
                return true;
            }
    }
    color[start_vertex] = 2;
    sorted.push(start_vertex);

    return false;
}
void Processing()
{
    size_t amount_vertices = 0;
    size_t amount_edges = 0;
    cin >> amount_vertices >> amount_edges;

    vector<vector<elem_t>> graph(amount_vertices);
    inputGraphByList(amount_edges, graph);

    bool is_acyclic = true;
    stack<elem_t> sorted;
    vector<elem_t> color(amount_vertices);
    for (size_t i = 0; i < amount_vertices; ++i)
    {
        if (!color[i] && DFS(i, color, graph, sorted))
        {
            is_acyclic = false;
            break;
        }
    }
    
    if (!is_acyclic)
    {   
        cout << -1 << endl;
        return;
    }
    while (!sorted.empty())
    {   
        cout << sorted.top() + 1 << ' ';
        sorted.pop();
    }
    cout << endl;
}

int main()
{
    Processing();

    return 0;
}