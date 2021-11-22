#include <stdlib.h>
#include <iostream>
#include <string.h>

#include <vector>
#include <stack>
#include <queue>
#include <iostream>

using std::cin;
using std::cout;
using std::vector;
using std::stack;
using std::queue;
using std::fill;


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

void Input(int height, int length, 
           IGraph& graph,
           vector<bool>& is_sub)
{      
    int current_x = 0;
    int current_y = 0;
    while (current_y < height)
    {
        Vertex current = current_y * length + current_x;
        
        bool sub_indicator = false;
        cin >> sub_indicator; 

        is_sub[current] = sub_indicator;

        if (current_x != 0)
        {   
            graph.addEdge(current_y * length + current_x - 1, current);
        }
        if (current_x != length - 1)
        {   
            graph.addEdge(current_y * length + current_x + 1, current);
        }
        if (current_y != 0)
        {
            graph.addEdge((current_y - 1) * length + current_x, current);
        }
        if (current_y != height - 1)
        {
            graph.addEdge((current_y + 1) * length + current_x, current);
        }
        ++current_x;
        if (current_x == length)
        {
            current_x = 0;
            ++current_y;
        }
    }
}

void initializeDistances(const IGraph& graph,
                         queue<Vertex>& queue, 
                         vector<bool>& visited, vector<int>& distances)
{
    while (!queue.empty())
    {
        Vertex from = queue.front();

        queue.pop();
        for (auto& to : graph.getNeighbors(from))
        {
            if (!visited[to])
            {
                visited[to] = true;
                
                queue.push(to);

                distances[to] = distances[from] + 1;
            }
        }
    }
}

void Processing()
{
    int height = 0;
    int length = 0;
    cin >> height >> length;

    size_t amount_vertices = height * length;
    
    ListGraph graph(amount_vertices, amount_vertices + 1);
    
    queue<Vertex> queue;
    vector<bool> visited(amount_vertices, false);
    vector<bool> is_sub(amount_vertices, false);
    Input(height, length, graph, is_sub);

    for (Vertex current = 0; current < amount_vertices; ++current)
    {
        if (is_sub[current])
        {
            queue.push(current);
            visited[current] = true;
        }
    }

    vector<int> distances(amount_vertices, 0);
    initializeDistances(graph, queue, visited, distances);

    for (Vertex current = 0; current < amount_vertices; ++current)
    {   
        cout << distances[current] << " ";
        if (current != 0 && (current + 1) % length == 0)
        {
            cout << std::endl;
        }
    }
}

int main()
{   
    Processing();

    return 0;
}