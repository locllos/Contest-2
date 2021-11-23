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

enum ArrowColor
{
    RED = 'R', 
    BLUE = 'B',
};

// color is too boring
enum VisitType
{
    VIRGIN,
    TOUCHED,
    USED,
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

// reverse directions of red arrows while inputing graph
void specialInput(IGraph& graph)
{   
    string color_arrows({});
    for (Vertex current = 0; current < static_cast<int>(graph.getAmountVertices()) - 1; ++current)
    {
        cin >> color_arrows;
        
        int amount_arrows = color_arrows.size();
        for (Vertex next = current + 1; next < current + amount_arrows + 1; ++next)
        {
            switch (color_arrows[next - current - 1])
            {
                case RED:
                    graph.addEdge(next, current);
                    break;

                case BLUE:
                    graph.addEdge(current, next);
                    break;
            }
        }
    }
}

bool DFS(const IGraph& graph, vector<VisitType>& visit_log, Vertex start)
{
    visit_log[start] = TOUCHED;
    for (Vertex vertex_to : graph.getNeighbors(start))
    {
            if (visit_log[vertex_to] == VIRGIN)
            {   
                if (DFS(graph, visit_log, vertex_to))
                {
                    return true;
                }
            }
            if (visit_log[vertex_to] == TOUCHED)
            {   
                return true;
            }
    }
    visit_log[start] = USED;

    return false;
}

bool isCyclic(const IGraph& graph)
{
    vector<VisitType> visit_log(graph.getAmountVertices(), VIRGIN);

    bool is_cyclic = false;
    for (Vertex current = 0; current < graph.getAmountVertices(); ++current)
    {
        if (visit_log[current] == VIRGIN)
        {
            is_cyclic = is_cyclic || DFS(graph, visit_log, current);
        }
    }

    return is_cyclic;
}

void Proccesing()
{
    size_t amount_numbers = 0;
    cin >> amount_numbers;

    ListGraph graph(amount_numbers, 0);
    specialInput(graph);


    cout << (isCyclic(graph) ? "NO" : "YES") << endl;
}

int main()
{   
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    Proccesing();

    return 0;
}