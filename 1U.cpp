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

struct Time
{   
    size_t timer;

    vector<Vertex> entry;
    vector<Vertex> min_entry;
};

class IGraph
{
public:

    virtual void addEdge(const Vertex& from, const Vertex& to) = 0;
    virtual vector<Vertex> getNeighbors(const Vertex& vertex) const = 0;
    virtual size_t getAmountVertices() const = 0;
    virtual size_t getAmountEdges() const = 0;
};

//=======ListGraph=======//

class ListGraph : public IGraph
{
private:

    size_t amount_vertices_;
    size_t amount_edges_;

    vector<vector<Vertex>> graph_;

public:

    explicit ListGraph(size_t amount_vertices, size_t amount_edges)
        : amount_vertices_(amount_vertices)
        , amount_edges_(amount_edges)
        , graph_(amount_vertices_, vector<Vertex>())
        {}
    
    void addEdge(const Vertex& from, const Vertex& to) override
    {
        graph_[from].push_back(to);
        graph_[to].push_back(from);

        ++amount_edges_;
    }

    vector<Vertex> getNeighbors(const Vertex& vertex) const override
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
        , graph_(amount_vertices, vector<Vertex>(amount_vertices, 0))
        {}

    void addEdge(const Vertex& from, const Vertex& to) override
    {   
        graph_[from][to] = 1;
        graph_[to][from] = 1;
        ++amount_edges_;
    }

    vector<Vertex> getNeighbors(const Vertex& vertex) const override
    {
        vector<Vertex> neighbors;

        for (Vertex to = 0; to < amount_vertices_; ++to)
        {   
            if (graph_[vertex][to] != 0)
            {
                neighbors.push_back(to);
            }
        }
        return neighbors;
    }

    size_t getAmountVertices() const {return amount_vertices_;};
    size_t getAmountEdges() const {return amount_edges_;};
};

//=======GraphAlgorithms=======//

void inputGraph(IGraph &graph, size_t amount_triangles, size_t amount_vertices)
{
    Vertex vertex_a = 0;
    Vertex vertex_b = 0;
    Vertex vertex_c = 0;

    for (size_t i = 0; i < amount_triangles; ++i)
    {
        cin >> vertex_a >> vertex_b >> vertex_c;
    
        graph.addEdge(i + amount_vertices, vertex_a - 1);
        graph.addEdge(i + amount_vertices, vertex_b - 1);
        graph.addEdge(i + amount_vertices, vertex_c - 1);

        graph.addEdge(vertex_a - 1, i + amount_vertices);
        graph.addEdge(vertex_b - 1, i + amount_vertices);
        graph.addEdge(vertex_c - 1, i + amount_vertices);
    }
}


void DFS(const IGraph& graph, Time& time, 
         vector<bool>& visited, vector<bool>& cut_points, 
         Vertex parent, Vertex start)
{   
    visited[start] = true;
    time.entry[start] = time.timer;
    time.min_entry[start] = time.timer;

    ++time.timer;
    
    size_t unconnected_children = 0;
    for (Vertex to : graph.getNeighbors(start))
    {   
        if (to == parent)
        {
            continue;
        }
        if (visited[to])
        {
            time.min_entry[start] = min(time.min_entry[start], time.entry[to]);
        }
        else
        {
            DFS(graph, time, visited, cut_points, start, to);

            time.min_entry[start] = min(time.min_entry[start], time.min_entry[to]);
            
            if (parent != -1 && time.min_entry[to] >= time.entry[start])
            {
                cut_points[start] = true;
            }
            ++unconnected_children;
        }
    }
    if (parent == -1 && unconnected_children > 1)
    {
        cut_points[start] = true;
    }
}

vector<Vertex> getCutPoints(const IGraph& graph)
{
    vector<bool> is_cut_point(graph.getAmountVertices());
    vector<bool> visited(graph.getAmountVertices(), false);

    Time time = {0, vector<Vertex>(graph.getAmountVertices(), kInf), 
                    vector<Vertex>(graph.getAmountVertices(), kInf)};
    
    for (Vertex current = 0; current < graph.getAmountVertices(); ++current)
    {
        if (!visited[current])
        {   
            DFS(graph, time, visited, is_cut_point, -1, current);
            time.timer = 0;
        }
    }

    vector<Vertex> cut_points;
    for (Vertex current = 0; current < graph.getAmountVertices(); ++current)
    {
        if (is_cut_point[current])
        {   
            cut_points.push_back(current);
        }
    }
    return cut_points;
}

void Processing()
{
    size_t amount_magnets = 0;
    size_t amount_triangles = 0;
    cin >> amount_magnets >> amount_triangles;

    ListGraph graph(amount_magnets + amount_triangles, 0);
    inputGraph(graph, amount_triangles, amount_magnets);

    vector<Vertex> cut_points = getCutPoints(graph);

    cout << cut_points.size() << endl;
    for (auto& cut_point : cut_points)
    {
        cout << cut_point + 1 - amount_magnets << '\n';
    }
}

int main()
{
    Processing();

    return 0;
}