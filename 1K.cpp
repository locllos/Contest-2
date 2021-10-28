#include <iostream>
#include <vector>
#include <stack>

using std::vector;
using std::stack;
using std::cin;
using std::cout;
using std::endl;



typedef int elem_t;
 
void inputGraphByMatrix(size_t amount_edges, 
                        vector<vector<elem_t>>& graph)
{
    elem_t vertex_a = 0;
    elem_t vertex_b = 0;
    for (size_t i = 0; i < amount_edges; ++i)
    {
        cin >> vertex_a >> vertex_b;

        graph[vertex_a - 1][vertex_b - 1] = 1;
    }
}

void inputGraphByList(size_t amount_edges, 
                      vector<vector<elem_t>>& graph)
{
    elem_t vertex_a = 0;
    elem_t vertex_b = 0;
    for (size_t i = 0; i < amount_edges; ++i)
    {
        cin >> vertex_a >> vertex_b;    

        graph[vertex_a - 1].push_back(vertex_b - 1);
    }
}

bool DFS(elem_t start_vertex, vector<elem_t>& color, vector<vector<elem_t>>& graph, vector<elem_t>& answer)
{
    color[start_vertex] = 1;
    for (elem_t current_vertex : graph[start_vertex])
    {
            if (!color[current_vertex])
            {   
                if (DFS(current_vertex, color, graph, answer))
                {
                    answer[current_vertex] = start_vertex;
                    return true;
                }
            }
            if (color[current_vertex] == 1)
            {   
                answer.push_back(start_vertex);
                answer.push_back(current_vertex);
                return true;
            }
    }
    color[start_vertex] = 2;
    
    return false;
}

void printAnswer(vector<elem_t>& answer)
{   
    elem_t start_vertex = answer.back();
    answer.erase(answer.end() - 1);
    elem_t end_vertex = answer.back();
    answer.erase(answer.end() - 1);
    
    stack<elem_t> correct_cycle;    

    correct_cycle.push(start_vertex + 1);
    for (elem_t current_vertex = end_vertex; 
         current_vertex != start_vertex && current_vertex > 0; 
         current_vertex = answer[current_vertex])
    {     
        correct_cycle.push(current_vertex + 1);
    }
    while (!correct_cycle.empty())
    {
        cout << correct_cycle.top() << " ";

        correct_cycle.pop();
    }
    cout << endl;
}

bool isCycled(size_t amount_vertices,
              vector<elem_t>& color,
              vector<vector<elem_t>>& graph,
              vector<elem_t>& answer)
{
    for (size_t i = 0; i < amount_vertices; ++i)
    {
        if (!color[i] && DFS(i, color, graph, answer))
        {                
            return true;
        }
    }
    return false;
}

void Processing()
{
    size_t amount_edges = 0;
    size_t amount_vertices = 0;
    cin >> amount_vertices >> amount_edges;

    vector<vector<elem_t>> graph(amount_vertices);
    vector<elem_t> color(amount_vertices);
    vector<elem_t> answer(amount_vertices, -1);

    inputGraphByList(amount_edges, graph);

    if (isCycled(amount_vertices, color, graph, answer))
    {
        cout << "YES" << endl;
        printAnswer(answer);
    }
    else
    {   
        cout << "NO" << endl;
    } 
}

int main()
{
    Processing();

    return 0;
}