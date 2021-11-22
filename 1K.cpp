#include <iostream>
#include <vector>
#include <stack>

using std::vector;
using std::stack;
using std::cin;
using std::cout;
using std::endl;

using Vertex = int;

//=======IGraph=======//

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
    virtual const vector<Vertex>& getNeighbors(const Vertex& vertex) const = 0;
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

    const vector<Vertex>& getNeighbors(const Vertex& vertex) const override
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
    {   
        Vertex vertex = 0;
        for (auto& edges : graph_)
        {
            std::fill(begin(edges), end(edges), vertex++);
        }
    }

    void addEdge(const Vertex& from, const Vertex& to) override
    {   
        graph_[from][to] = 1;
    }

    const vector<Vertex>& getNeighbors(const Vertex& vertex) const override
    {
        return graph_[vertex];
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

bool DFS(const IGraph& graph, const Vertex& start, vector<Color>& color, vector<Vertex>& cycle)
{
    color[start] = GRAY;
    for (Vertex current : graph.getNeighbors(start))
    {
        if (color[current] == WHITE)
        {   
            if (DFS(graph, current, color, cycle))
            {
                cycle[current] = start;
                return true;
            }
        }
        if (color[current] == GRAY)
        {   
            cycle.push_back(start);
            cycle.push_back(current);
            return true;
        }
    }
    color[start] = BLACK;
    
    return false;
}

void printAnswer(vector<Vertex>& cycle)
{   
    for (int i = cycle.size() - 1; i > -1; --i)
    {
        cout << cycle[i] << ' ';
    }
    cout << endl;
}

vector<Vertex> getCorrectCycle(vector<Vertex>& cycle)
{
    vector<Vertex> correct_cycle;    

    Vertex start = cycle.back();
    cycle.erase(cycle.end() - 1);
    Vertex end = cycle.back();
    cycle.erase(cycle.end() - 1);

    correct_cycle.push_back(start + 1);
    for (Vertex current = end; 
         current != start && current > 0; 
         current = cycle[current])
    {     
        correct_cycle.push_back(current + 1);
    }
    return correct_cycle;
}

bool isCycled(const IGraph& graph,
              vector<Vertex>& cycle)
{   
    vector<Color> color(graph.getAmountVertices(), WHITE);

    bool is_cycled = false;
    for (Vertex current = 0; current < graph.getAmountVertices(); ++current)
    {
        if ((color[current] == WHITE) && DFS(graph, current, color, cycle))
        {                
            is_cycled = true;
            
            cycle = getCorrectCycle(cycle);

            break;
        }
    }
    return is_cycled;
}

void Processing()
{
    size_t amount_edges = 0;
    size_t amount_vertices = 0;
    cin >> amount_vertices >> amount_edges;

    ListGraph graph(amount_vertices, amount_edges);
    inputGraph(graph);

    vector<Vertex> cycle;  
    if (isCycled(graph, cycle))
    {
        cout << "YES" << endl;
        printAnswer(cycle);
    }
    else
    {   
        cout << "NO" << endl;
    } 
}

int main()
{
    Processing();

    return 0;
}