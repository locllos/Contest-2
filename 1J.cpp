#include <iostream>
#include <vector>
#include <algorithm>
#include <deque>

using std::vector;
using std::deque;
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


void inputGraph(IGraph& graph)
{
    Vertex to = 0;
    Vertex from = 0;

    for (int i = 0; i < graph.getAmountEdges(); ++i)
    {
        cin >> from >> to;

        graph.addEdge(from - 1, to - 1, 0);
        graph.addEdge(to - 1, from - 1, 1);
    }
}

void initializeDistances(IGraph& graph, vector<int>& distances,
                         const Vertex& start, const Vertex& end_vertex)
{
    deque<int> bi_queue;
    distances[start] = 0;
    
    bi_queue.push_back(start);
    while(bi_queue.size() > 0)
    {
        Vertex current = bi_queue.front();
        bi_queue.pop_front();

        if (current == end_vertex) return;

        for (Edge& edge : graph.getNeighbors(current))
        {
            if (distances[edge.from] + edge.distance < distances[edge.to])
            {
                distances[edge.to] = distances[edge.from] + edge.distance;

                if (edge.distance == 0) bi_queue.push_front(edge.to);
                else                    bi_queue.push_back(edge.to);
            }
        }
    }
}

int main()
{   
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int amount_vertices = 0;
    int amount_edges = 0;
    cin >> amount_vertices >> amount_edges;

    ListGraph graph(amount_vertices, amount_edges);
    inputGraph(graph);

    int amount_requests = 0;
    cin >> amount_requests;

    vector<int> fines(amount_vertices, amount_vertices + 1);
    for (int i = 0; i < amount_requests; ++i)
    {
        Vertex start = 0;
        Vertex end = 0;

        cin >> start >> end;

        initializeDistances(graph, fines, start - 1, end - 1);

        cout << ((fines[end - 1] == amount_vertices + 1) ? -1 : fines[end - 1]) << endl;

        std::fill(fines.begin(), fines.end(), amount_vertices + 1);
    }

    return 0;
}
