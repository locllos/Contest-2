#include <iostream>
#include <vector>
#include <queue>
#include <stack>

using std::vector;
using std::queue;
using std::stack;

using std::cin;
using std::cout;

typedef int Vertex;

const Vertex kStart = -3802;

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

void BFS(const IGraph& graph, Vertex start, vector<bool>& visited, vector<Vertex>& parents)
{
    queue<Vertex> queue;

    visited[start] = true;
    parents[start] = kStart;
    
    queue.push(start);
    while (!queue.empty())
    {
        Vertex current = queue.front();
        queue.pop();

        for (Vertex to : graph.getNeighbors(current))
        {
            if (!visited[to])
            {
                visited[to] = true;

                queue.push(to);
                
                parents[to] = current;
            }
        }
    }
}

void initOptimalPath(Vertex target_vertex,
                     vector<Vertex>& path,
                     const vector<Vertex>& parents)
{   
    Vertex current_vertex = target_vertex;
    while (current_vertex != kStart)
    {
        path.push_back(current_vertex);
        current_vertex = parents[current_vertex];
    }
}

vector<Vertex> getOptimalPath(const IGraph& graph, Vertex start, Vertex end)
{
    vector<bool> visited(graph.getAmountVertices());
    vector<Vertex> parents(graph.getAmountVertices());
    vector<Vertex> path;

    BFS(graph, start, visited, parents);

    if (visited[end])
    {
        initOptimalPath(end, path, parents);
    }
    return path;
}

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

void printPath(const vector<Vertex>& path)
{   
    cout << '\n' << path.size() - 1 << '\n';

    for (int i = path.size() - 1; i > - 1; --i)
    {
        cout << path[i] + 1 << ' ';
    }
    cout << '\n';
}

void Processing()
{
    size_t amount_vertices = 0;
    size_t amount_edges = 0;
    Vertex start_vertex = 0;
    Vertex target_vertex = 0;

    cin >> amount_vertices >> amount_edges;
    cin >> start_vertex >> target_vertex;

    // MatrixGraph graph(amount_vertices, amount_edges);
    ListGraph graph(amount_vertices, amount_edges);

    inputGraphByEdgeList(graph);

    vector<Vertex> path = getOptimalPath(graph, start_vertex - 1, target_vertex - 1);

    if (path.size() == 0) cout << "-1\n";
    else printPath(path);
}

int main()
{
    Processing();

    return 0;
}