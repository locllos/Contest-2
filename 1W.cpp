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

    IGraph() = default;

    virtual void addEdge(const Vertex& from, const Vertex& to) = 0;
    virtual bool hasEdges(const Vertex& vertex) = 0;
    virtual std::_List_iterator<Vertex> deleteEdge(const Vertex& from, const Vertex& to) = 0;
    virtual std::_List_iterator<Vertex> deleteEdge(const Vertex& from, std::_List_iterator<Vertex> edge) = 0;
    virtual list<Vertex> getNeighbors(const Vertex& vertex) const = 0;
    virtual size_t getAmountVertices() const = 0;
    virtual size_t getAmountEdges() const = 0;
};

//=======ListGraph=======//

class ListGraph : public IGraph
{
private:

    size_t amount_vertices_;
    size_t amount_edges_;

    vector<list<Vertex>> graph_;

public:

    explicit ListGraph(size_t amount_vertices, size_t amount_edges)
        : IGraph::IGraph()
        , amount_vertices_(amount_vertices)
        , amount_edges_(amount_edges)
        , graph_(amount_vertices_, list<Vertex>())
        {}
    
    std::_List_iterator<Vertex> deleteEdge(const Vertex& from, const Vertex& to) override
    {   
        auto iter = graph_[from].begin();
        while (iter != graph_[from].end())
        {
            if (*iter == to)
            {
                --amount_edges_;
                return graph_[from].erase(iter);
            }
            ++iter;
        }
        return graph_[from].end();
    }

    std::_List_iterator<Vertex> deleteEdge(const Vertex& from, std::_List_iterator<Vertex> edge) override
    {   
        return graph_[from].erase(edge);
    }

    void addEdge(const Vertex& from, const Vertex& to) override
    {
        graph_[from].push_back(to);
        ++amount_edges_;
    }

    list<Vertex> getNeighbors(const Vertex& vertex) const override
    {
        return graph_[vertex];
    }

    bool hasEdges(const Vertex& vertex)
    {
        return !graph_[vertex].empty();
    }

    size_t getAmountVertices() const override {return amount_vertices_;};
    size_t getAmountEdges() const override {return amount_edges_;};
};

//=======GraphAlgorithms=======//

Vertex GetNotIsolatedVertex(vector<bool>& is_isolated)
{
    for (Vertex i = 0; i < static_cast<Vertex>(is_isolated.size()); ++i)
    {
        if (!is_isolated[i])
        {
            return i;
        }
    }
    return 0;
}

void DFS(Vertex start, IGraph& graph, vector<bool>& visited)
{
    visited[start] = true;

    for (auto vertex_to : graph.getNeighbors(start))
    {
        if (!visited[vertex_to])
        {
            DFS(vertex_to, graph, visited);
        }
    }
}


bool IsEuler(IGraph& graph, vector<bool>& is_isolated)
{
    vector<bool> visited(graph.getAmountVertices(), false);

    DFS(GetNotIsolatedVertex(is_isolated), graph, visited);

    for (size_t i = 0; i < graph.getAmountVertices(); ++i)
    {
        if (!visited[i] && !is_isolated[i])
        {
            return false;
        }
    }
    return true;
}

void MakeEulerPath(Vertex start, IGraph& graph, vector<Vertex>& euler_path)
{
    Vertex vertex_to = 0;
    auto edge = graph.getNeighbors(start).begin();
    while (graph.hasEdges(start))
    {
        vertex_to = *edge;
        edge = graph.deleteEdge(start, edge);
        MakeEulerPath(vertex_to, graph, euler_path);
    }
    euler_path.push_back(start);
}

void InputPathToGraphByList(size_t amount_edges, IGraph& graph, vector<bool>& is_isolated) 
{
    Vertex vertex_a = 0;
    Vertex vertex_b = 0;

    cin >> vertex_a;
    for (size_t i = 0; i < amount_edges; ++i) 
    {
        cin >> vertex_b;

        graph.addEdge(vertex_a - 1, vertex_b - 1);
        is_isolated[vertex_a - 1] = false;
        is_isolated[vertex_b - 1] = false;

        vertex_a = vertex_b;
    }
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

    ListGraph graph(amount_vertices, 0);
    vector<bool> is_isolated(amount_vertices, true);
    for (size_t i = 0; i < amount_paths; ++i)
    {
        size_t amount_edges = 0;
        cin >> amount_edges;
        InputPathToGraphByList(amount_edges, graph, is_isolated);
    }

    if (!IsEuler(graph, is_isolated))
    {
        cout << 0 << endl;
        return;
    }

    vector<Vertex> euler_path;
    MakeEulerPath(GetNotIsolatedVertex(is_isolated), graph, euler_path);

    cout << euler_path.size();
    for (int i = 0; i < euler_path.size(); ++i)
    {   
        cout << ' ' << euler_path[i] + 1;
    }
    cout << endl;
}

int main()
{
    Processing();

    return 0;
}