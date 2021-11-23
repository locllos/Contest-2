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

using Vertex = int;

enum Color
{
    WHITE,
    GRAY,
    BLACK,
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

void inputGraph(IGraph& graph)
{
    Vertex vertex_a = 0;
    Vertex vertex_b = 0;

    for (size_t i = 0; i < graph.getAmountEdges(); ++i)
    {   
        cin >> vertex_a >> vertex_b;

        graph.addEdge(vertex_a - 1, vertex_b - 1);
    }
}

bool DFS(const IGraph& graph, vector<Color>& color, vector<Vertex>& sorted, const Vertex& start)
{
    color[start] = GRAY;
    for (Vertex current : graph.getNeighbors(start))
    {
            if (!color[current])
            {   
                if (DFS(graph, color, sorted, current))
                {
                    return true;
                }
            }
            if (color[current] == GRAY)
            {   
                return true;
            }
    }
    color[start] = BLACK;
    sorted.push_back(start);

    return false;
}

vector<Vertex> getSortedVertices(const IGraph& graph)
{
    bool is_acyclic = true;

    vector<Vertex> sorted;
    vector<Color> color(graph.getAmountVertices(), WHITE);
    for (Vertex current = 0; current < graph.getAmountVertices(); ++current)
    {
        if (!color[current] && DFS(graph, color, sorted, current))
        {
            is_acyclic = false;
            break;
        }
    }

    if (!is_acyclic) return vector<Vertex>();

    return sorted;
}

void printSortedVertices(const vector<Vertex>& sorted)
{
    for (int i = sorted.size() - 1; i > -1; --i)
    {
        cout << sorted[i] + 1 << ' ';
    }
}

void Processing()
{
    size_t amount_vertices = 0;
    size_t amount_edges = 0;
    cin >> amount_vertices >> amount_edges;

    ListGraph graph(amount_vertices, amount_edges);
    inputGraph(graph);

    vector<Vertex> sorted = getSortedVertices(graph);
    
    if (sorted.size() == 0) cout << -1 << endl;
    else printSortedVertices(sorted);
    
}

int main()
{
    Processing();

    return 0;
}