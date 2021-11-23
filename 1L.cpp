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


void inputGraphByList(const IGraph& graph)
{
    int vertex = 0;
    int amount_vertices = graph.getAmountVertices();
    for (Vertex current = 0; current < amount_vertices; ++current)
    {
        cin >> vertex;    

        graph[current].push_back(vertex - 1);
    }
}

int Indicator(bool bool_expr)
{
    return bool_expr == true;
}

bool isNewWeakComponent(Graph& graph, vector<TraverseInfo>& traverse_information, 
                        int current_vertex, int weak_component_number)
{   
    traverse_information[current_vertex].visited = true;
    
    int old_weak_component = 0;
    bool is_new_weak_component = true;
    for (int next : graph[current_vertex])
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
         traverse_information[current_vertex].weak_component = old_weak_component;
    }
    else
    {
        traverse_information[current_vertex].weak_component = 
                                            weak_component_number + 1;
    }
    return is_new_weak_component;
}   

int main()
{   
    size_t amount_vertices = 0;
    cin >> amount_vertices;

    Graph graph(amount_vertices);
    inputGraphByList(graph);

    int amount_weak_components = 0;
    vector<TraverseInfo> traverse_information(amount_vertices, {false, kNone});
    for (size_t i = 0; i < amount_vertices; ++i)
    {
        if(!traverse_information[i].visited)
        {
            if (isNewWeakComponent(graph, traverse_information, i, amount_weak_components))
            {
                ++amount_weak_components;
            } 
        }
    }
    cout << amount_weak_components << endl;

    return 0;
}