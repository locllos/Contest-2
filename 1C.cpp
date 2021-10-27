#include <iostream>
#include <queue>
#include <vector>

using std::vector;
using std::queue;
using std::cin;
using std::cout;

const size_t INF = -1;

struct ChessPosition
{
    size_t x;
    size_t y;

    void setChessPathLength(vector<vector<size_t>>& data, size_t length)
    {
        data[x][y] = length;
    }

    size_t getChessPathLength(const vector<vector<size_t>>& data)
    {
        return data[x][y];
    }

    bool operator==(const ChessPosition& other)
    {
        return (other.x == x && other.y == y);
    }

    const ChessPosition operator+(const ChessPosition& other)
    {
        return {x + other.x, y + other.y};
    }

    const ChessPosition operator-(const ChessPosition& other)
    {
        return {x - other.x, y - other.y};
    }
};

using chess_queue = queue<ChessPosition>;

void Processing(vector<vector<chess_queue>>& paths,
                const ChessPosition& from, const ChessPosition& to)
{
    chess_queue queue;
    size_t chess_size = paths.size();

    queue.push(from);

    while (!queue.empty())
    {
        ChessPosition current = queue.front();

        queue.pop();

        if (current == to) break;

        if (current.x < chess_size - 2 && 
            current.y < chess_size - 1 &&
            paths[current.x + 2][current.y + 1].size() == 0) 
        {
            paths[current.x + 2][current.y + 1] = paths[current.x][current.y];
            paths[current.x + 2][current.y + 1].push(current);
            queue.push(ChessPosition({current.x + 2, current.y + 1}));
        }
        if (current.x < chess_size - 1 && 
            current.y < chess_size - 2 &&
            paths[current.x + 1][current.y + 2].size() == 0) 
        {
            paths[current.x + 1][current.y + 2] = paths[current.x][current.y];
            paths[current.x + 1][current.y + 2].push(current);
            queue.push(ChessPosition({current.x + 1, current.y + 2}));
        }
        if (current.x < chess_size - 2 && 
            current.y > 0 &&
            paths[current.x + 2][current.y - 1].size() == 0) 
        {
            paths[current.x + 2][current.y - 1] = paths[current.x][current.y];
            paths[current.x + 2][current.y - 1].push(current);
            queue.push(ChessPosition({current.x + 2, current.y - 1}));
        }
        if (current.x > 0 && 
            current.y < chess_size - 2 &&
            paths[current.x - 1][current.y + 2].size() == 0) 
        {
            paths[current.x - 1][current.y + 2] = paths[current.x][current.y];
            paths[current.x - 1][current.y + 2].push(current);
            queue.push(ChessPosition({current.x - 1, current.y + 2}));
        }
        if (current.x > 1 && 
            current.y < chess_size - 1 &&
            paths[current.x - 2][current.y + 1].size() == 0) 
        {
            paths[current.x - 2][current.y + 1] = paths[current.x][current.y];
            paths[current.x - 2][current.y + 1].push(current);
            queue.push(ChessPosition({current.x - 2, current.y + 1}));
        }
        if (current.x < chess_size - 1 && 
            current.y > 1 &&
            paths[current.x + 1][current.y - 2].size() == 0) 
        {
            paths[current.x + 1][current.y - 2] = paths[current.x][current.y];
            paths[current.x + 1][current.y - 2].push(current);
            queue.push(ChessPosition({current.x + 1, current.y - 2}));
        }
        if (current.x > 0 && 
            current.y > 2 &&
            paths[current.x - 1][current.y - 2].size() == 0) 
        {
            paths[current.x - 1][current.y - 2] = paths[current.x][current.y];
            paths[current.x - 1][current.y - 2].push(current);
            queue.push(ChessPosition({current.x - 1, current.y - 2}));
        }
        if (current.x > 2 && 
            current.y > 1 &&
            paths[current.x - 2][current.y - 1].size() == 0) 
        {
            paths[current.x - 2][current.y - 1] = paths[current.x][current.y];
            paths[current.x - 2][current.y - 1].push(current);
            queue.push(ChessPosition({current.x - 2, current.y - 1}));
        }
    }
}   

int main()
{   
    size_t chess_size;

    ChessPosition pos_from;
    ChessPosition pos_to;

    cin >> chess_size;
    cin >> pos_from.x >> pos_from.y;
    cin >> pos_to.x >> pos_to.y;

    pos_from.x -= 1;
    pos_from.y -= 1;

    pos_to.x -= 1;
    pos_to.y -= 1;

    vector<vector<chess_queue>> paths(chess_size, vector<chess_queue>(chess_size));
    Processing(paths, pos_from, pos_to);

    cout << paths[pos_to.x][pos_to.y].size() << std::endl;
    while (!paths[pos_to.x][pos_to.y].empty())
    {   
        ChessPosition current = paths[pos_to.x][pos_to.y].front();
        
        cout << current.x + 1 << " " << current.y + 1 << std::endl;

        paths[pos_to.x][pos_to.y].pop();
    }
    cout << pos_to.x + 1 << " " << pos_to.y + 1 << std::endl;

    return 0;
}