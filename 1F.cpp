#include <iostream>
#include <vector>
#include <queue>

using std::vector;
using std::queue;
using std::cin;
using std::cout;
using std::endl;


struct Coordinate
{
    int x;
    int y;

    bool operator==(const Coordinate& other)
    {
        return (x == other.x && y == other.y);
    }
};

enum BARRIER_TYPE
{
    WALL = '#',
    NOTHING = '.',

    START = 'S',
    END = 'T',
};

Coordinate moveUp(const vector<vector<char>>& race_field, const Coordinate& current)
{
    int i = 1;
    for (; current.y - i > -1; ++i)
    {
        if (race_field[current.y - i][current.x] == WALL) 
        {
            break;
        }
    }
    i = current.y - i < 0 ? (current.y + 1) : i;

    return {current.x, current.y - i / 2};
}

Coordinate moveDown(const vector<vector<char>>& race_field, const Coordinate& current)
{
    int i = 1;
    for (; current.y + i < race_field.size(); ++i)
    {
        if (race_field[current.y + i][current.x] == WALL) 
        {
            break;
        }
    }
    return {current.x, current.y + i / 2}; 
}

Coordinate moveRight(const vector<vector<char>>& race_field, const Coordinate& current)
{
    int i = 1;
    for (; current.x + i < race_field.size(); ++i)
    {
        if (race_field[current.y][current.x + i] == WALL) 
        {
            break;
        }
    }
    return {current.x + i / 2, current.y}; 
}

Coordinate moveLeft(const vector<vector<char>>& race_field, const Coordinate& current)
{
    int i = 1;
    for (; current.x - i > -1; ++i)
    {
        if (race_field[current.y][current.x - i] == WALL) 
        {
            break;
        }
    }
    i = i < 0 ? (current.x + 1) : i;

    return {current.x - i / 2, current.y};
}

using Move = Coordinate (*)(const vector<vector<char>>&, const Coordinate&);
const Move MOVE[] = {moveUp, moveDown, moveRight, moveLeft};

void addNew(queue<Coordinate>& queue,
            vector<vector<bool>>& visited,
            vector<vector<Coordinate>>& prev, 
            const Coordinate& parent, const Coordinate& next)
{
    prev[next.y][next.x] = parent;
    visited[next.y][next.x] = true;
    queue.push(next);
}

void processMovement(const vector<vector<char>>& race_field, 
                     queue<Coordinate>& queue,
                     vector<vector<bool>>& visited,
                     vector<vector<Coordinate>>& prev, 
                     const Coordinate& current)
{
    Coordinate next = {};
    for (int i = 0; i < sizeof(MOVE) / sizeof(Move); ++i)
    {
        next = MOVE[i](race_field, current);
        if (!visited[next.y][next.x]) 
        {
            addNew(queue, visited, prev, current, next);
        }
    }
}

bool isEnd(const vector<vector<char>>& race_field, 
           const Coordinate& current)
{
    return race_field[current.y][current.x] == END;
}

bool isStart(const vector<vector<char>>& race_field, 
             const Coordinate& current)
{
    return race_field[current.y][current.x] == START;
}

Coordinate Processing(const vector<vector<char>>& race_field, 
                      vector<vector<Coordinate>>& prev,
                      Coordinate start)
{
    queue<Coordinate> queue;
    vector<vector<bool>> visited(race_field.size(), vector<bool>(race_field[0].size()));

    prev[start.y][start.x] = {-1, -1};
    visited[start.y][start.x] = true;
    
    queue.push(start);
    while (!queue.empty())
    {
        Coordinate current = queue.front();
        
        queue.pop();

        if (isEnd(race_field, current)) return current;

        processMovement(race_field, queue, visited, prev, current);
    }
    return start;
}



void Input(vector<vector<char>>& race_field, Coordinate& start)
{
    size_t width = race_field[0].size();
    size_t height = race_field.size();

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            cin >> race_field[y][x];

            if (isStart(race_field, {x, y})) 
                start = {x, y};
        }
    }
}

int getAmountMoves(const vector<vector<Coordinate>>& prev, const Coordinate& end)
{
    Coordinate current = end;
    int amount_moves = 0;

    while (prev[current.y][current.x].x != -1)
    {
        ++amount_moves;

        current = prev[current.y][current.x];
    }
    return amount_moves;
}

int main()
{   
    size_t width = 0;
    size_t height = 0;
    cin >> height >> width;
    
    Coordinate start = {};
    vector<vector<char>> race_field(height, vector<char>(width));
    Input(race_field, start);

    vector<vector<Coordinate>> prev(height, vector<Coordinate>(width, {0, 0}));
    Coordinate end = Processing(race_field, prev, start);
    
    cout << ((end == start) ? -1 : getAmountMoves(prev, end)) << endl;

    return 0;
}
