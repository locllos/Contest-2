#include <iostream>
#include <vector>
#include <queue>

using std::vector;
using std::queue;
using std::cin;
using std::cout;
using std::endl;

typedef int Vertex;

struct Edge
{   
    Vertex from;
    Vertex to;
    int distance;
};

const int kInf = (int)1e9;
const Vertex kStart = -3802;

//=======IGraph=======//

class IGraph
{
public:

    virtual void addEdge(const Vertex& from, const Vertex& to, int distance) = 0;
    virtual vector<Edge> getNeighbors(const Vertex& vertex) const = 0;
    virtual size_t getAmountVertices() const = 0;
    virtual size_t getAmountEdges() const = 0;
};

//=======ListGraph=======//

class ListGraph : public IGraph
{
private:

    size_t amount_vertices_;
    size_t amount_edges_;

    vector<vector<Edge>> graph_;

public:

    explicit ListGraph(size_t amount_vertices, size_t amount_edges)
        : amount_vertices_(amount_vertices)
        , amount_edges_(amount_edges)
        , graph_(amount_vertices_, vector<Edge>())
        {}
    
    void addEdge(const Vertex& from, const Vertex& to, int distance) override
    {
        graph_[from].push_back({from, to, distance});
    }

    vector<Edge> getNeighbors(const Vertex& vertex) const override
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
        , graph_(amount_vertices, vector<Vertex>(amount_vertices, kInf))
        {}

    void addEdge(const Vertex& from, const Vertex& to, int distance) override
    {   
        graph_[from][to] = distance;
    }

    vector<Edge> getNeighbors(const Vertex& vertex) const override
    {   
        vector<Edge> neighbors;

        for (Vertex to = 0; to < amount_vertices_; ++to)
        {   
            if (graph_[vertex][to] < kInf)
            {
                neighbors.push_back({vertex, to, graph_[vertex][to]});
            }
        }
        return neighbors;
    }

    size_t getAmountVertices() const {return amount_vertices_;};
    size_t getAmountEdges() const {return amount_edges_;};
};

//=======GraphAlgorithms=======//

void Input(IGraph& graph)
{
    int distance = 0;
    int vertex_to = 0;
    int vertex_from = 0;

    for (size_t i = 0; i < graph.getAmountEdges(); ++i)
    {   
        cin >> vertex_from >> vertex_to >> distance;

        graph.addEdge(vertex_from - 1, vertex_to - 1, distance);
    }
}

int getMaxDistance(IGraph& graph)
{
    int max_distance = -1;

    for (Vertex current = 0; current < graph.getAmountVertices(); ++current)
    {   
        for (auto& edge : graph.getNeighbors(current))
        {
            max_distance = std::max(edge.distance, max_distance);
        }
    }

    return max_distance;
}

int getDistance(IGraph& graph, Vertex start, Vertex end)
{   
    int max_distance = getMaxDistance(graph);

    vector<int> distances(graph.getAmountVertices(), kInf);
    vector<queue<int>> vector_queue(max_distance + 1);
    
    vector_queue[0].push(start);
    distances[start] = 0;

    int amount_vertices = graph.getAmountVertices();
    for (int queue_idx = 0; 
         queue_idx < amount_vertices * max_distance + 1; 
         ++queue_idx)
    {   
        queue<Vertex>& current = vector_queue[queue_idx % (max_distance + 1)];
        while (!current.empty())
        {
            Vertex front_vertex = current.front();
            current.pop();

            if (front_vertex == end) return distances[end];

            for (Edge& edge : graph.getNeighbors(front_vertex))
            {
                if (distances[edge.from] + edge.distance < distances[edge.to])
                {
                    distances[edge.to] = distances[edge.from] + edge.distance;
                    vector_queue[(queue_idx + edge.distance) % (max_distance + 1)].push(edge.to);
                }
            }
        }
    }
    return -1;
}

int main()
{   
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    size_t amount_vertices = 0;
    size_t amount_edges = 0;
    cin >> amount_vertices >> amount_edges;       
    
    Vertex start = 0;
    Vertex end = 0;
    cin >> start >> end;

    int max_distance = 0;
    ListGraph graph(amount_vertices, amount_edges);

    Input(graph);

    cout << getDistance(graph, start - 1, end - 1);

    return 0;
}