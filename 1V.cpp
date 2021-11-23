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

const int kInf = (int)1e9;

using Vertex = int;

enum BridgeType
{
    SIMPLE_EDGE,
    BRIDGE,
    MULTI_EDGE,
};

struct Time
{   
    size_t timer;

    vector<Vertex> entry;
    vector<Vertex> min_entry;
};

struct Edge
{
    Vertex from;
    Vertex to;

    int number;
};

class IGraph
{
public:

    virtual void addEdge(const Vertex& from, const Vertex& to, int number) = 0;
    virtual vector<Edge> getNeighbors(const Vertex& vertex) const = 0;
    virtual size_t getAmountVertices() const = 0;
    virtual size_t getAmountEdges() const = 0;
};

//=======ListGraph=======//

class ListGraph : public IGraph
{
private:

    size_t amount_vertices_;
    size_t amount_edges_;

    vector<vector<Edge>> graph_;

public:

    explicit ListGraph(size_t amount_vertices, size_t amount_edges)
        : amount_vertices_(amount_vertices)
        , amount_edges_(amount_edges)
        , graph_(amount_vertices_, vector<Edge>())
        {}
    
    void addEdge(const Vertex& from, const Vertex& to, int number) override
    {
        graph_[from].push_back({from, to, number});
    }

    vector<Edge> getNeighbors(const Vertex& vertex) const override
    {
        return graph_[vertex];
    }

    size_t getAmountVertices() const {return amount_vertices_;};
    size_t getAmountEdges() const {return amount_edges_;};
};

//=======MatrixGraph=======//

class MatrixGraph : public IGraph
{
private:

    vector<vector<Vertex>> graph_;

    size_t amount_vertices_;
    size_t amount_edges_;

public:

    explicit MatrixGraph(size_t amount_vertices, size_t amount_edges)
        : amount_vertices_(amount_vertices)
        , amount_edges_(amount_edges)
        , graph_(amount_vertices, vector<Vertex>(amount_vertices, -1))
        {}

    void addEdge(const Vertex& from, const Vertex& to, int number) override
    {   
        graph_[from][to] = number;
    }

    vector<Edge> getNeighbors(const Vertex& vertex) const override
    {
        vector<Edge> neighbors;

        for (Vertex to = 0; to < amount_vertices_; ++to)
        {   
            if (graph_[vertex][to] != -1)
            {
                neighbors.push_back({vertex, to, graph_[vertex][to]});
            }
        }
        return neighbors;
    }

    size_t getAmountVertices() const {return amount_vertices_;};
    size_t getAmountEdges() const {return amount_edges_;};
};

//=======GraphAlgorithms=======//

void inputGraph(IGraph& graph)
{
    Vertex vertex_a = 0;
    Vertex vertex_b = 0;

    for (int i = 0; i < graph.getAmountEdges(); ++i)
    {   
        cin >> vertex_a >> vertex_b;

        graph.addEdge(vertex_a - 1, vertex_b - 1, i);
        graph.addEdge(vertex_b - 1, vertex_a - 1, i);
    }
}


void DFS(const IGraph& graph, Time& time, 
         vector<bool>& visited, vector<Edge>& bridges, 
         Vertex parent, Vertex start)
{   
    visited[start] = true;

    time.entry[start] = time.timer;
    time.min_entry[start] = time.timer;
    ++time.timer;

    for (auto& next : graph.getNeighbors(start))
    {   
        if (next.to == parent)
        {
            continue;
        }
        if (visited[next.to])
        {
            time.min_entry[start] = min(time.min_entry[start], time.entry[next.to]);
        }
        else
        {
            DFS(graph, time, visited, bridges, start, next.to);
            time.min_entry[start] = min(time.min_entry[start], time.min_entry[next.to]);
            if (time.min_entry[next.to] > time.entry[start])
            {
                bridges.push_back(next);
            }
        }
    }
}

vector<Edge> getBridges(const IGraph& graph)
{
    vector<bool> is_bridge(graph.getAmountVertices());
    vector<bool> visited(graph.getAmountVertices(), false);
    Time time = {0, vector<Vertex>(graph.getAmountVertices(), kInf), 
                    vector<Vertex>(graph.getAmountVertices(), kInf)};
    
    vector<Edge> bridges;
    for (Vertex current = 0; current < static_cast<int>(graph.getAmountVertices()); ++current)
    {
        if (!visited[current])
        {
            DFS(graph, time, visited, bridges, -1, current);
            time.timer = 0;
        }
    }

    vector<Edge> correct_bridges;
    vector<BridgeType> bridge_type(graph.getAmountVertices(), SIMPLE_EDGE);
    for (auto& bridge : bridges)
    {
        for (auto& edge : graph.getNeighbors(bridge.from))
        {
            if (edge.to == bridge.to && bridge_type[bridge.number] == SIMPLE_EDGE)
            {  
                correct_bridges.push_back(edge);
                bridge_type[edge.number] = BRIDGE;

            }
            else if (edge.to == bridge.to && bridge_type[bridge.number] == BRIDGE)
            {
                bridge_type[bridge.number] = MULTI_EDGE;
            }
        }
    }
    return correct_bridges;
}

void Processing()
{
    size_t amount_vertices = 0;
    size_t amount_edges = 0;
    cin >> amount_vertices >> amount_edges;

    ListGraph graph(amount_vertices, amount_edges);
    inputGraph(graph);

    vector<Edge> bridges = getBridges(graph);

    cout << bridges.size() << endl;
    for (auto& bridge : bridges)
    {
        cout << bridge.number + 1 << '\n';
    }
}

int main()
{
    Processing();

    return 0;
}