#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <queue>

using std::string;
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
    virtual IGraph* getTransposed() const = 0;
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

    IGraph* getTransposed() const override
    {
        ListGraph* transposed = new ListGraph(amount_vertices_, amount_edges_);

        for (Vertex from = 0; from < amount_vertices_; ++from)
        {
            for (auto& to : graph_[from])
            {
                transposed->addEdge(to, from);
            }
        }

        return transposed;
    }
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

    IGraph* getTransposed() const override
    {
        MatrixGraph* transposed = new MatrixGraph(amount_vertices_, amount_edges_);

        for (Vertex from = 0; from < amount_vertices_; ++from)
        {
            for (Vertex to = 0; to < amount_vertices_; ++to)
            {   
                if (graph_[from][to] != 0)
                {
                    transposed->addEdge(to, from);
                }
            }
        }

        return transposed;
    }
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

void FillExitTimes(const IGraph& graph, vector<bool>& visited, vector<Vertex>& exits, Vertex start)
{
    visited[start] = true;
    for (Vertex current : graph.getNeighbors(start))
    {
            if (!visited[current])
            {   
                FillExitTimes(graph, visited, exits, current);
            }
    }
    exits.push_back(start);
}

void FillCondensation(const IGraph& graph, vector<bool>& visited, vector<Vertex>& condensation,
                      Vertex start, size_t condensation_num)
{
    visited[start] = true;
    condensation[start] = condensation_num;

    for (Vertex current : graph.getNeighbors(start))
    {
        if (!visited[current])
        {
            FillCondensation(graph, visited,  condensation, current, condensation_num);
        }
    }
}

vector<Vertex> getCondensation(const IGraph& graph, size_t& condensation_num)
{   
    IGraph* transposed = graph.getTransposed();
    vector<Vertex> exits;
    vector<bool> visited(graph.getAmountVertices());
    for (Vertex current = 0; current < graph.getAmountVertices(); ++current)
    {
        if (!visited[current])
        {
            FillExitTimes(graph, visited, exits, current);
        }
    }
    
    condensation_num = 1;
    vector<Vertex> condensation(graph.getAmountVertices(), 0);
    fill(visited.begin(), visited.end(), false);
    while (!exits.empty())
    {
        Vertex current = exits.back();
        exits.erase(exits.end() - 1);
        if (!visited[current])
        {
            FillCondensation(*transposed, visited,  condensation, current, condensation_num);
            ++condensation_num;
        }
    }

    return condensation;
}

void Processing()
{
    size_t amount_vertices = 0;
    size_t amount_edges = 0;
    cin >> amount_vertices >> amount_edges;

    ListGraph graph(amount_vertices, amount_edges);
    inputGraph(graph);

    size_t condensation_num = 0;
    vector<Vertex> condensation = getCondensation(graph, condensation_num);
    
    cout << condensation_num - 1 << endl;
    for (Vertex vertex : condensation)
    {   
        cout << vertex << ' ';
    }
    cout << endl;
}

int main()
{
    Processing();

    return 0;
}