#include <iostream>
#include <queue>
#include <vector>
#include <stack>
#include <algorithm>
#include <vector>

using std::vector;
using std::queue;
using std::stack;
using std::cin;
using std::cout;
using std::endl;
using std::vector;


using Vertex = int;

const Vertex kStart = -3802;
const size_t kInf = -1;
const int kAmountNumbers = 10000;
const int kStartNumber = 1000;
const int kNone = 0;
const int kInit = 1;


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

    virtual void addEdge(const Vertex &from, const Vertex &to) = 0;
    virtual vector<Vertex> getNeighbors(const Vertex &vertex) const = 0;
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
        : amount_vertices_(amount_vertices), amount_edges_(amount_edges), graph_(amount_vertices_, vector<Vertex>())
    {}

    void addEdge(const Vertex &from, const Vertex &to) override
    {
        graph_[from].push_back(to);
        ++amount_edges_;
    }

    vector<Vertex> getNeighbors(const Vertex &vertex) const override
    {
        return graph_[vertex];
    }

    size_t getAmountVertices() const { return amount_vertices_; };
    size_t getAmountEdges() const { return amount_edges_; };
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
        : amount_vertices_(amount_vertices), amount_edges_(amount_edges), graph_(amount_vertices, vector<Vertex>(amount_vertices, 0))
    {}

    void addEdge(const Vertex &from, const Vertex &to) override
    {
        graph_[from][to] = 1;
        ++amount_edges_;
    }

    vector<Vertex> getNeighbors(const Vertex &vertex) const override
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

    size_t getAmountVertices() const { return amount_vertices_; };
    size_t getAmountEdges() const { return amount_edges_; };
};

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

void initOptimalPath(Vertex target,
                     vector<Vertex>& path,
                     const vector<Vertex>& parents)
{   
    Vertex current = target;
    while (current != kStart)
    {
        path.push_back(current);
        current = parents[current];
    }

    std::reverse(path.begin(), path.end());
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


int increaseFirstDigit(int number)
{   
    if (number / 1000 == 9) return number;

    return number + 1000; 
}

int decreaseLastDigit(int number)
{   
    if (number % 10 == 1) return number;

    return number - 1;
}

int decimalShiftRight(int number)
{
    return ((number % 10) * 1000 + number / 10);
}

int decimalShiftLeft(int number)
{
    return ((number % 1000) * 10 + number / 1000);
}

bool hasZero(int number)
{
    while (number != 0)
    {
        if (number % 10 == 0) return true;

        number /= 10;
    }
    return false;
}

void inputGraph(IGraph& graph)
{   
    int next = 0;
    for (int number = kStartNumber; number < kAmountNumbers; ++number)
    {
        if (hasZero(number)) continue;

        next = increaseFirstDigit(number);
        if (next != number) graph.addEdge(number, next);

        next = decreaseLastDigit(number);
        if (next != number) graph.addEdge(number, next);

        next = decimalShiftRight(number);
        if (next != number) graph.addEdge(number, next);

        next = decimalShiftLeft(number);
        if (next != number) graph.addEdge(number, next);
    }
}


int main()
{   
    Vertex start;
    Vertex end;
    cin >> start >> end;

    ListGraph graph(kAmountNumbers, 0);

    inputGraph(graph);

    vector<Vertex> path = getOptimalPath(graph, start, end);

    cout << path.size() << '\n';
    for (auto& number : path)
    {
        cout << number << '\n';
    }

    return 0;
}