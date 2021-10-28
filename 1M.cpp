#include <iostream>

#include <vector>
#include <stack>
#include <queue>

using std::vector;
using std::stack;
using std::queue;
using std::cin;
using std::cout;
using std::endl;

typedef int elem_t;

const elem_t threshold = -37; 



void inputGraphByMatrix(size_t amount_edges, 
                        vector<vector<elem_t>>& graph)
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

void DFS(elem_t start_vertex, 
         vector<bool>& visited, vector<vector<elem_t>>& graph, stack<elem_t>& connected_vertices)
{
    visited[start_vertex] = true;
    for (elem_t current_vertex : graph[start_vertex])
    {
        if (!visited[current_vertex])
        {
            DFS(current_vertex, visited, graph, connected_vertices);
        }
    }
    connected_vertices.push(start_vertex);
}


void printAnswer(queue<stack<elem_t>>& components) 
{
    cout << components.size() << endl;

    while (!components.empty()) 
    {
        stack<elem_t>& connected_vertices = components.front();

        cout << connected_vertices.size() << endl;
        while (!connected_vertices.empty()) 
        {
            cout << connected_vertices.top() + 1 << ' ';

            connected_vertices.pop();
        }
        cout << endl;
        components.pop();
    }
}

void Processing()
{
    size_t amount_vertices = 0;
    size_t amount_edges = 0;
    cin >> amount_vertices >> amount_edges;

    vector<vector<elem_t>> graph(amount_vertices);

    inputGraphByList(amount_edges, graph);

    vector<bool> visited(amount_vertices, false);
    queue<stack<elem_t>> components;
    for (size_t i = 0; i < amount_vertices; ++i)
    {
        if (!visited[i])
        {
            components.push(stack<elem_t>());

            DFS(i, visited, graph, components.back());
        }
    }
    printAnswer(components);

}

int main()
{
    Processing();

    return 0;
}