#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>

using std::cin;
using std::cout;
using std::queue;
using std::vector;

using Vertex = int;

const size_t kInf = -1;
const Vertex kStart = -3802;

struct ChessPosition
{
    int x;
    int y;

    bool operator==(const ChessPosition &other)
    {
        return (other.x == x && other.y == y);
    }

    ChessPosition operator+(const ChessPosition &other)
    {
        return {x + other.x, y + other.y};
    }

    ChessPosition operator-(const ChessPosition &other)
    {
        return {x - other.x, y - other.y};
    }

    int getUniqueNumber(int row_length)
    {
        return row_length * y + x;
    }
};

ChessPosition extractChessPosition(int unique_number, int row_length)
{
    return {unique_number % row_length, unique_number / row_length};
}


const ChessPosition kHorseMoves[] = {{2, 1}, {1, 2}, {2, -1}, {1, -2}, {-1, 2}, {-2, 1}, {-2, -1}, {-1, -2}};

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
    {
    }

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

void inputGraph(IGraph &graph, int chess_size)
{   
    ChessPosition pos = {0, 0};
    for (; pos.x < chess_size; ++pos.x)
    {
        for (; pos.y < chess_size; ++pos.y)
        {
            for (auto &move : kHorseMoves)
            {
                if (pos.x + move.x < chess_size && pos.x + move.x > -1 &&
                    pos.y + move.y < chess_size && pos.y + move.y > -1)
                {
                    ChessPosition result = pos + move;
                    graph.addEdge(pos.getUniqueNumber(chess_size),
                                  result.getUniqueNumber(chess_size));
                }
            }
        }
    }
}

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

int main()
{
    size_t chess_size;

    ChessPosition start;
    ChessPosition end;

    cin >> chess_size;
    cin >> start.x >> start.y;
    cin >> end.x >> end.y;

    start.x -= 1;
    start.y -= 1;

    end.x -= 1;
    end.y -= 1;

    ListGraph graph(chess_size * chess_size, 0);
    inputGraph(graph, chess_size);

    vector<Vertex> path = getOptimalPath(graph, start.getUniqueNumber(chess_size),
                                                end.getUniqueNumber(chess_size));
    cout << path.size() << '\n';
    for (auto& number : path)
    {
        ChessPosition pos = extractChessPosition(number, chess_size);

        cout << pos.x + 1 << ' ' << pos.y + 1 << '\n';
    }

    return 0;
}