#include <iostream>
#include <vector>

using std::vector;
using std::cin;
using std::cout;
using std::endl;

const int NONE = (int)10e9;

struct TraverseInfo
{
    bool visited;

    int weak_component;
};

using Graph = vector<vector<int>>;

void inputGraphByList(Graph& graph)
{
    int vertex = 0;
    int amount_vertices = graph.size();
    for (int i = 0; i < amount_vertices; ++i)
    {
        cin >> vertex;    

        graph[i].push_back(vertex - 1);
    }
}

int Indicator(bool bool_expr)
{
    return bool_expr == true;
}

bool isNewWeakComponent(Graph& graph, vector<TraverseInfo>& traverse_information, 
                        int current_vertex, int weak_component_number)
{   
    traverse_information[current_vertex].visited = true;
    
    int old_weak_component = 0;
    bool is_new_weak_component = true;
    for (int next : graph[current_vertex])
    {
        if (!traverse_information[next].visited) 
        {   
            is_new_weak_component = isNewWeakComponent(graph, traverse_information, 
                                                       next, weak_component_number);
        }
        else if (traverse_information[next].weak_component <=
                                            weak_component_number)
        {   
            old_weak_component = traverse_information[next].weak_component;
            is_new_weak_component = false;
        }
    }
    if (!is_new_weak_component)
    {
         traverse_information[current_vertex].weak_component = old_weak_component;
    }
    else
    {
        traverse_information[current_vertex].weak_component = 
                                            weak_component_number + 1;
    }
    return is_new_weak_component;
}   

int main()
{   
    size_t amount_vertices = 0;
    cin >> amount_vertices;

    Graph graph(amount_vertices);
    inputGraphByList(graph);

    int amount_weak_components = 0;
    vector<TraverseInfo> traverse_information(amount_vertices, {false, NONE});
    for (size_t i = 0; i < amount_vertices; ++i)
    {
        if(!traverse_information[i].visited)
        {
            if (isNewWeakComponent(graph, traverse_information, i, amount_weak_components))
            {
                ++amount_weak_components;
            } 
        }
    }
    cout << amount_weak_components << endl;

    return 0;
}