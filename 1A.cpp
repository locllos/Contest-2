#include <iostream>
#include <vector>
#include <queue>
#include <stack>

using std::vector;
using std::queue;
using std::stack;

using std::cin;
using std::cout;

typedef int elem_t;

const elem_t start = -3802;

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

void makeUpPaths(elem_t start_vertex, 
                 vector<vector<elem_t>>& graph,
                 vector<bool>& visited, 
                 vector<elem_t>& parents)
{
    queue<elem_t> vertex_queue;

    visited[start_vertex] = true;
    parents[start_vertex] = start;
    vertex_queue.push(start_vertex);
    while (!vertex_queue.empty())
    {
        size_t current_vertex = vertex_queue.front();
        vertex_queue.pop();
        for (size_t i = 0; i < graph[current_vertex].size(); ++i)
        {
            size_t vertex_to = graph[current_vertex][i];
            if (!visited[vertex_to])
            {
                visited[vertex_to] = true;
                vertex_queue.push(vertex_to);
                parents[vertex_to] = current_vertex;
            }
        }
    }
}

//note: path must be exist!
void theNearestPathToTarget(elem_t target_vertex,
                            stack<elem_t>& path,
                            vector<elem_t>& parents)
{   
    elem_t current_vertex = target_vertex;
    while (current_vertex != start)
    {
        path.push(current_vertex);
        current_vertex = parents[current_vertex];
    }
}

void printPath(stack<elem_t> path)
{   
    cout << "\n" << path.size() - 1 << "\n";

    while (!path.empty())
    {   
        cout << path.top() + 1 << "\n";

        path.pop();
    }
    cout << "\n";
}

void Processing()
{
    size_t amount_vertices = 0;
    size_t amount_edges = 0;
    elem_t start_vertex = 0;
    elem_t target_vertex = 0;

    cin >> amount_vertices >> amount_edges;
    cin >> start_vertex >> target_vertex;

    vector<vector<elem_t>> graph(amount_vertices);
    vector<bool> visited(amount_vertices);
    vector<elem_t> parents(amount_vertices);
    stack<elem_t> path;

    inputGraphByList(amount_edges, graph);

    makeUpPaths(start_vertex - 1, graph, visited, parents);

    if (!visited[target_vertex - 1])
    {
        cout << "\n-1\n";
    }
    else
    {
        theNearestPathToTarget(target_vertex - 1, path, parents);
        printPath(path);
    }

}

int main()
{
    Processing();

    return 0;
}