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

class IGraph
{
public:

    virtual void Input() = 0;
};

class Graph : public IGraph
{
private:

    size_t amount_vertices_;
    size_t amount_edges_;

    vector<vector<elem_t>> graph;

    void makeUpPaths(elem_t start_vertex, 
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
                                vector<elem_t>& path,
                                vector<elem_t>& parents)
    {   
        elem_t current_vertex = target_vertex;
        while (current_vertex != start)
        {
            path.push_back(current_vertex);
            current_vertex = parents[current_vertex];
        }
    }

public:

    explicit Graph(size_t amount_vertices, size_t amount_edges) 
        : amount_vertices_(amount_vertices)
        , amount_edges_(amount_edges)
        , graph(amount_vertices, vector<elem_t>())
        {}
    
    void Input() override 
    {
        elem_t vertex_a = 0;
        elem_t vertex_b = 0;
        for (size_t i = 0; i < amount_edges_; ++i)
        {   
            cin >> vertex_a >> vertex_b;

            graph[vertex_a - 1].push_back(vertex_b - 1);
            graph[vertex_b - 1].push_back(vertex_a - 1);
        }
    }

    vector<elem_t> getTheNearestPath(elem_t start_vertex, elem_t end_vertex)
    {
        vector<bool> visited(amount_vertices_);
        vector<elem_t> parents(amount_vertices_);
        vector<elem_t> path;

        makeUpPaths(start_vertex, visited, parents);

        if (visited[end_vertex - 1])
        {
            theNearestPathToTarget(end_vertex, path, parents);
        }
        return path;
    }
};


void printPath(vector<elem_t> path)
{   
    cout << '\n' << path.size() - 1 << '\n';

    for (int i = path.size() - 1; i > - 1; ++i)
    {
        cout << path[i] + 1 << '\n';
    }
    cout << '\n';
}

void Processing()
{
    size_t amount_vertices = 0;
    size_t amount_edges = 0;
    elem_t start_vertex = 0;
    elem_t target_vertex = 0;

    cin >> amount_vertices >> amount_edges;
    cin >> start_vertex >> target_vertex;

    Graph graph(amount_vertices, amount_edges);
    graph.Input();

    vector<elem_t> path = graph.getTheNearestPath(start_vertex - 1, target_vertex - 1);

    printPath(path);
}

int main()
{
    Processing();

    return 0;
}