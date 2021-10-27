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

struct ExtentedVertex 
{
    int number;
    bool is_sub = false;
};

typedef ExtentedVertex elem_t;

const int start = -1;

void InputStrangeGraph(int height, int length, 
                       vector<vector<elem_t>>& graph,
                       queue<int>& vertex_queue,
                       vector<bool>& visited)
{      
    bool is_sub = false;
    int current_x = 0;
    int current_y = 0;
    ExtentedVertex current_vertex = {};
    while (current_y < height)
    {
        cin >> is_sub;
        current_vertex.number = current_y * length + current_x;
        current_vertex.is_sub = is_sub;
        if (is_sub)
        {
            vertex_queue.push(current_vertex.number);
            visited[current_vertex.number] = true;
        }
        if (current_x != 0)
        {
            graph[current_y * length + current_x - 1].push_back(current_vertex);
        }
        if (current_x != length - 1)
        {
            graph[current_y * length + current_x + 1].push_back(current_vertex);
        }
        if (current_y != 0)
        {
            graph[(current_y - 1) * length + current_x].push_back(current_vertex);
        }
        if (current_y != height - 1)
        {
            graph[(current_y + 1) * length + current_x].push_back(current_vertex);
        }
        ++current_x;
        if (current_x == length)
        {
            current_x = 0;
            ++current_y;
        }
    }
}

void initializeDistances(queue<int>& vertex_queue, 
                         vector<vector<elem_t>>& graph,
                         vector<bool>& visited, vector<int>& distances)
{   
    while (!vertex_queue.empty())
    {
        int vertex_from = vertex_queue.front();

        vertex_queue.pop();
        for (size_t i = 0; i < graph[vertex_from].size(); ++i)
        {
            elem_t vertex_to = graph[vertex_from][i];
            if (!visited[vertex_to.number])
            {
                visited[vertex_to.number] = true;
                vertex_queue.push(vertex_to.number);

                distances[vertex_to.number] = distances[vertex_from] + 1;
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
    vector<bool> are_sub(amount_vertices, false);
    vector<vector<elem_t>> graph(amount_vertices);
    vector<bool> visited(amount_vertices, false);

    queue<int> vertex_queue;
    InputStrangeGraph(height, length, graph, vertex_queue, visited);

    vector<int> distances(amount_vertices, 0);
    initializeDistances(vertex_queue, graph, visited, distances);

    for (size_t i = 0; i < amount_vertices; ++i)
    {   
        cout << distances[i] << " ";
        if (i != 0 && (i + 1) % length == 0)
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