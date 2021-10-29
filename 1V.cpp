#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>

using std::set;
using std::vector;
using std::cin;
using std::cout;
using std::endl;
using std::min;
using std::sort;
using std::pair;

const int inf = (int)1e9;

struct Vertex
{
    int number;
    int parent;
};

enum BridgeType
{
    SIMPLE_EDGE,
    BRIDGE,
    MULTI_EDGE,
};

struct Edge
{
    int from;
    int to;

    int number;

    Edge(int init_from, int init_to, int init_number) 
        : from(init_from)
        , to(init_to)
        , number(init_number)
        {}

    Edge(const Edge& other)
        : from(other.from)
        , to(other.to)
        , number(other.number)
        {}

    bool operator==(const Edge& other)
    {
        return number == other.number && 
               from   == other.from;
    }

    bool operator<(const Edge& other)
    {
        return  number < other.number ||
               (number == other.number && from < other.from);
    }

    bool operator>(const Edge& other)
    {
        return  number > other.number ||
               (number == other.number && from > other.from);
    }
    bool operator<=(const Edge& other)
    {
        return  number <= other.number ||
               (number == other.number && from <= other.from);
    }

    bool operator>=(const Edge& other)
    {
        return  number >= other.number ||
               (number == other.number && from >= other.from);
    }

    bool operator!=(const Edge& other)
    {
        return !(*this == other);
    }
};

struct Time
{   
    size_t& timer;
    vector<int>& entry;
    vector<int>& min_entry;
};


using Graph = vector<vector<Edge>>;

void inputGraphByList(size_t amount_edges, Graph& graph)
{
    int vertex_a = 0;
    int vertex_b = 0;

    for (int i = 0; i < static_cast<int>(amount_edges); ++i)
    {
        cin >> vertex_a >> vertex_b;

        graph[vertex_a - 1].push_back({vertex_a - 1, vertex_b - 1, i});
        graph[vertex_b - 1].push_back({vertex_b - 1, vertex_a - 1, i});
    }
}

void DFS(Vertex vertex, Time time, vector<bool>& visited, Graph& graph, vector<Edge>& bridges)
{   
    visited[vertex.number] = true;
    time.entry[vertex.number] = time.timer;
    time.min_entry[vertex.number] = time.timer;
    ++time.timer;

    for (auto next : graph[vertex.number])
    {   
        if (next.to == vertex.parent)
        {
            continue;
        }
        if (visited[next.to])
        {
            time.min_entry[vertex.number] = min(time.min_entry[vertex.number], time.entry[next.to]);
        }
        else
        {
            DFS({next.to, vertex.number}, time, visited, graph, bridges);
            time.min_entry[vertex.number] = min(time.min_entry[vertex.number], time.min_entry[next.to]);
            if (time.min_entry[next.to] > time.entry[vertex.number])
            {
                bridges.push_back(next);
            }
        }
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
    vector<Edge> bridges;
    vector<bool> visited(amount_vertices, false);
    vector<int> min_entry_time(amount_vertices, inf);
    vector<int> entry_time(amount_vertices, inf);

    for (int i = 0; i < static_cast<int>(amount_vertices); ++i)
    {
        if (!visited[i])
        {
            DFS({i, -1}, {timer, entry_time, min_entry_time}, visited, graph, bridges);
            timer = 0;
        }
    }

    size_t amount_bridges = 0;
    vector<BridgeType> are_bridge(amount_edges, SIMPLE_EDGE);
    for (auto& bridge : bridges)
    {
        for (auto& edge : graph[bridge.from])
        {
            if (edge.to == bridge.to && are_bridge[bridge.number] == SIMPLE_EDGE)
            {   
                ++amount_bridges;
                are_bridge[edge.number] = BRIDGE;
            }
            else if (edge.to == bridge.to && are_bridge[bridge.number] == BRIDGE)
            {
                --amount_bridges;
                are_bridge[bridge.number] = MULTI_EDGE;
            }
        }
    }

    cout << amount_bridges << endl;
    for (size_t i = 0; i < amount_edges; ++i)
    {
        if (are_bridge[i] == BRIDGE)
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