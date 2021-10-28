#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <queue>

using std::string;
using std::queue;
using std::stack;
using std::vector;
using std::cin;
using std::cout;
using std::endl;

enum ArrowColor
{
    RED = 'R', 
    BLUE = 'B',
};

enum VisitType
{
    VIRGIN,
    TOUCHED,
    USED,
};

struct Edge
{
    int from;
    int to;

    ArrowColor color;
};

using Graph = vector<vector<int>>;

// reverse directions of red arrows while inputing graph
void specialInput(Graph& graph, size_t amount_numbers)
{   
    string color_arrows({});
    for (int current = 0; current < static_cast<int>(amount_numbers) - 1; ++current)
    {
        cin >> color_arrows;
        
        int amount_arrows = color_arrows.size();
        for (int next = current + 1; next < current + amount_arrows + 1; ++next)
        {
            switch (color_arrows[next - current - 1])
            {
                case RED:
                    graph[next].push_back(current);
                    break;

                case BLUE:
                    graph[current].push_back(next);
                    break;
            }
        }
    }
}

bool isCyclic(Graph& graph, vector<VisitType>& visit_log, int start_vertex)
{
    visit_log[start_vertex] = TOUCHED;
    for (int vertex_to : graph[start_vertex])
    {
            if (visit_log[vertex_to] == VIRGIN)
            {   
                if (isCyclic(graph, visit_log, vertex_to))
                {
                    return true;
                }
            }
            if (visit_log[vertex_to] == TOUCHED)
            {   
                return true;
            }
    }
    visit_log[start_vertex] = USED;

    return false;
}
void Proccesing()
{
    size_t amount_numbers = 0;
    cin >> amount_numbers;

    Graph graph(amount_numbers);
    specialInput(graph, amount_numbers);

    vector<VisitType> visit_log(amount_numbers, VIRGIN);

    bool is_cyclic = false;
    for (int current_number = 0; current_number < amount_numbers; ++current_number)
    {
        if (visit_log[current_number] == VIRGIN)
        {
            is_cyclic = is_cyclic || isCyclic(graph, visit_log, current_number);
        }
    }
    cout << (is_cyclic ? "NO" : "YES") << endl;
}

int main()
{   
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    Proccesing();

    return 0;
}