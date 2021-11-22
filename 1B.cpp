#include <iostream>
#include <vector>
#include <stack>

using std::vector;
using std::stack;

using std::cin;
using std::cout;

using Vertex = int;

enum Color
{
    WHITE,
    GRAY,
    BLACK,

    AMOUNT_COLORS,
};


//=======IGraph=======//

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
        , graph_(amount_vertices, vector<Vertex>(amount_vertices))
    {   
        // благодаря этому, мне не нужно создавать новый вектор и делать лишние копирования
        // для того, чтобы bfs работал корректно
        Vertex vertex = 0;
        for (auto& edges : graph_)
        {
            std::fill(begin(edges), end(edges), vertex++);
        }
    }

    void addEdge(const Vertex& from, const Vertex& to) override
    {   
        // благодаря этому, мне не нужно создавать новый вектор и делать много копий вектора
        // для того, чтобы bfs работал корректно
        // но, да, мы не можем теперь хранить расстояние в этой ячейке, за полиморфизм нужно платить.
        // В данном случае это память.
        graph_[from][to] = to;
    }

    const vector<Vertex>& getNeighbors(const Vertex& vertex) const override
    {
        return graph_[vertex];
    }

    size_t getAmountVertices() const {return amount_vertices_;};
    size_t getAmountEdges() const {return amount_edges_;};
};

//=======GraphAlgorithms=======//

void inputGraphByEdgeList(IGraph& graph)
{
    Vertex vertex_a = 0;
    Vertex vertex_b = 0;
    for (size_t i = 0; i < graph.getAmountEdges(); ++i)
    {   
        cin >> vertex_a >> vertex_b;

        graph.addEdge(vertex_a - 1, vertex_b - 1);
        graph.addEdge(vertex_b - 1, vertex_a - 1);
    }
}

bool isSplitable(Vertex start, const IGraph& graph, vector<Color>& color)
{
    size_t is_splitable = true;
    for (auto& current : graph.getNeighbors(start))
    {
        if (color[current] == WHITE)
        {
            color[current] = static_cast<Color>(AMOUNT_COLORS - color[start]);
            is_splitable = isSplitable(current, graph, color); 
        }
        else if (color[current] == color[start])
        {
            is_splitable = false;
        }
    }
    return is_splitable;
}

bool isSplitable(const IGraph& graph)
{
    vector<Color> color(graph.getAmountVertices(), WHITE);

    bool is_splitable = true;
    for (Vertex current = 0; current < graph.getAmountVertices(); ++current)
    {
        if (color[current] == WHITE) 
        {
            color[current] = GRAY;
            is_splitable = is_splitable && isSplitable(current, graph, color);
        }
    }
    if (graph.getAmountVertices() == 1)
    {
        is_splitable = false;
    }
    return is_splitable;
}

void Processing()
{
    size_t amount_vertices = 0;
    size_t amount_edges = 0;
    cin >> amount_vertices >> amount_edges;

    ListGraph graph(amount_vertices, amount_edges);

    inputGraphByEdgeList(graph);

    cout << (isSplitable(graph) ? "YES" : "NO");
}

int main()
{
    Processing();

    return 0;
}