#include <iostream>
#include <vector>
#include <stack>
#include <list>

using std::vector;
using std::stack;
using std::list;
using std::cin;
using std::cout;
using std::endl;

typedef int elem_t;

void InputPathToGraphByList(size_t amount_edges, vector<list<elem_t>>& graph, vector<bool>& is_isolated) 
{
    elem_t vertex_a = 0;
    elem_t vertex_b = 0;

    cin >> vertex_a;
    for (size_t i = 0; i < amount_edges; ++i) 
    {
        cin >> vertex_b;

        graph[vertex_a - 1].push_back(vertex_b - 1);
        is_isolated[vertex_a - 1] = false;
        is_isolated[vertex_b - 1] = false;

        vertex_a = vertex_b;
    }
}

elem_t GetNotIsolatedVertex(vector<bool>& is_isolated)
{
    for (elem_t i = 0; i < static_cast<elem_t>(is_isolated.size()); ++i)
    {
        if (!is_isolated[i])
        {
            return i;
        }
    }
    return 0;
}

void DFS(size_t start_vertex, vector<list<elem_t>>& graph, vector<bool>& visited)
{
    visited[start_vertex] = true;

    for (auto vertex_to : graph[start_vertex])
    {
        if (!visited[vertex_to])
        {
            DFS(vertex_to, graph, visited);
        }
    }
}


bool IsEuler(vector<list<elem_t>>& graph, vector<bool>& is_isolated)
{
    vector<bool> visited(graph.size(), false);

    DFS(GetNotIsolatedVertex(is_isolated), graph, visited);

    for (size_t i = 0; i < graph.size(); ++i)
    {
        if (!visited[i] && !is_isolated[i])
        {
            return false;
        }
    }
    return true;
}

void MakeEulerPath(elem_t start_vertex, vector<list<elem_t>>& graph, stack<elem_t>& euler_path)
{
    size_t vertex_to = 0;
    auto edge = graph[start_vertex].begin();
    while (!graph[start_vertex].empty())
    {
        vertex_to = *edge;
        edge = graph[start_vertex].erase(edge);
        MakeEulerPath(vertex_to, graph, euler_path);
    }
    euler_path.push(start_vertex);
}

void Processing()
{
    size_t amount_paths = 0;
    size_t amount_vertices = 0;
    cin >> amount_paths >> amount_vertices;

    if (amount_paths == 0)
    {
        cout << 0 << endl;
        return;
    }

    size_t amount_edges = 0;
    vector<list<elem_t>> graph(amount_vertices);
    vector<bool> is_isolated(amount_vertices, true);
    for (size_t i = 0; i < amount_paths; ++i)
    {
        cin >> amount_edges;
        InputPathToGraphByList(amount_edges, graph, is_isolated);
    }

    if (!IsEuler(graph, is_isolated))
    {
        cout << 0 << endl;
        return;
    }

    stack<elem_t> euler_path;
    MakeEulerPath(GetNotIsolatedVertex(is_isolated), graph, euler_path);

    cout << euler_path.size();
    while(!euler_path.empty())
    {   
        cout << ' ' << euler_path.top() + 1;
        euler_path.pop();
    }
    cout << endl;
}

int main()
{
    Processing();

    return 0;
}