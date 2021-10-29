#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <queue>
#include <algorithm>

using std::string;
using std::queue;
using std::stack;
using std::vector;
using std::cin;
using std::cout;
using std::endl;
using std::min;
using std::sort;

typedef int elem_t;

const elem_t inf = (elem_t)1e9;

struct Vertex
{
    elem_t number;
    elem_t parent;
};

struct Time
{   
    size_t& timer;
    vector<elem_t>& entry;
    vector<elem_t>& min_entry;
};

using Graph = vector<vector<elem_t>>;

void inputGraphByList(size_t amount_edges,
                      Graph &graph)
{
    elem_t vertex_a = 0;
    elem_t vertex_b = 0;
    bool skip = false;
    for (size_t i = 0; i < amount_edges; ++i)
    {
        cin >> vertex_a >> vertex_b;

        graph[vertex_a - 1].push_back(vertex_b - 1);
        graph[vertex_b - 1].push_back(vertex_a - 1);
    }
}


void DFS(Vertex vertex, Time time, vector<bool>& visited, Graph& graph, vector<bool>& cut_points)
{   
    visited[vertex.number] = true;
    time.entry[vertex.number] = time.timer;
    time.min_entry[vertex.number] = time.timer;
    ++time.timer;
    size_t unconnected_children = 0;
    for (elem_t vertex_to : graph[vertex.number])
    {   
        if (vertex_to == vertex.parent)
        {
            continue;
        }
        if (visited[vertex_to])
        {
            time.min_entry[vertex.number] = min(time.min_entry[vertex.number], time.entry[vertex_to]);
        }
        else
        {
            DFS({vertex_to, vertex.number}, time, visited, graph, cut_points);
            time.min_entry[vertex.number] = min(time.min_entry[vertex.number], time.min_entry[vertex_to]);
            if (vertex.parent != -1 && time.min_entry[vertex_to] >= time.entry[vertex.number])
            {
                cut_points[vertex.number] = true;
            }
            ++unconnected_children;
        }
    }
    if (vertex.parent == -1 && unconnected_children > 1)
    {
        cut_points[vertex.number] = true;
    }
}
void Processing()
{
    size_t amount_vertices = 0;
    size_t amount_edges = 0;
    cin >> amount_vertices >> amount_edges;

    Graph graph(amount_vertices);
    inputGraphByList(amount_edges, graph);

    size_t timer = 0;
    vector<bool> cut_points(amount_vertices);
    vector<bool> visited(amount_vertices, false);
    vector<elem_t> min_entry_time(amount_vertices, inf);
    vector<elem_t> entry_time(amount_vertices, inf);

    for (int i = 0; i < amount_vertices; ++i)
    {
        if (!visited[i])
        {
            DFS({i, -1}, {timer, entry_time, min_entry_time}, visited, graph, cut_points);
            timer = 0;
        }
    }

    size_t amount_cut_point = 0;
    for (size_t i = 0; i < amount_vertices; ++i)
    {
        if (cut_points[i])
        {
            ++amount_cut_point;
        }
    }

    cout << amount_cut_point << endl;
    for (size_t i = 0; i < amount_vertices; ++i)
    {
        if (cut_points[i])
        {   
            cout << i + 1 << endl;
        }
    }
}

int main()
{
    Processing();

    return 0;
}