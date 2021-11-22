#include <iostream>
#include <vector>

using std::vector;
using std::cin;
using std::cout;
using std::endl;

const int kNone = (int)10e9;

using Vertex = int;

struct TraverseInfo
{
    bool visited;

    int weak_component;
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
    Vertex vertex = 0;
    int amount_vertices = graph.getAmountVertices();
    for (Vertex current = 0; current < amount_vertices; ++current)
    {
        cin >> vertex;    

        graph.addEdge(current, vertex - 1);
    }
}

int Indicator(bool bool_expr)
{
    return bool_expr == true;
}


bool isNewWeakComponent(const IGraph& graph, vector<TraverseInfo>& traverse_information, 
                        Vertex current, int weak_component_number)
{   
    traverse_information[current].visited = true;
    
    int old_weak_component = 0;
    bool is_new_weak_component = true;
    for (Vertex next : graph.getNeighbors(current))
    {
        if (!traverse_information[next].visited) 
        {   
            is_new_weak_component = isNewWeakComponent(graph, traverse_information, 
                                                       next, weak_component_number);
        }
        else if (traverse_information[next].weak_component <=
                                            weak_component_number)
        {   
            old_weak_component = traverse_information[next].weak_component;
            is_new_weak_component = false;
        }
    }
    if (!is_new_weak_component)
    {
         traverse_information[current].weak_component = old_weak_component;
    }
    else
    {
        traverse_information[current].weak_component = weak_component_number + 1;
    }
    return is_new_weak_component;
}   


size_t getAmountWeakComponents(const IGraph& graph)
{
    size_t amount_weak_components = 0;
    vector<TraverseInfo> traverse_information(graph.getAmountVertices(), {false, kNone});
    for (size_t i = 0; i < graph.getAmountVertices(); ++i)
    {
        if(!traverse_information[i].visited)
        {
            if (isNewWeakComponent(graph, traverse_information, i, amount_weak_components))
            {
                ++amount_weak_components;
            } 
        }
    }

    return amount_weak_components;
}

int main()
{   
    size_t amount_vertices = 0;
    cin >> amount_vertices;

    ListGraph graph(amount_vertices, amount_vertices);
    inputGraph(graph);

    cout << getAmountWeakComponents(graph) << endl;

    return 0;
}