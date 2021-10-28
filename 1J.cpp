#include <iostream>
#include <vector>
#include <algorithm>
#include <deque>

using std::vector;
using std::deque;
using std::cin;
using std::cout;
using std::endl;

struct Edge
{   
    int from;
    int to;
    
    int fine;
};

using Graph = vector<vector<Edge>>;

void inputGraph(Graph& graph, int amount_edges)
{
    int vertex_to = 0;
    int vertex_from = 0;
    for (int i = 0; i < amount_edges; ++i)
    {
        cin >> vertex_from >> vertex_to;

        graph[vertex_from - 1].push_back({vertex_from - 1, vertex_to - 1, 0});
        
        graph[vertex_to - 1].push_back({vertex_to - 1, vertex_from - 1, 1});
    }
}

void calculateFines(Graph& graph, vector<int>& fines,
                    int start_vertex, int end_vertex)
{
    deque<int> bi_queue;
    fines[start_vertex] = 0;
    
    bi_queue.push_back(start_vertex);
    while(bi_queue.size() > 0)
    {
        int current_vertex = bi_queue.front();
        bi_queue.pop_front();

        if (current_vertex == end_vertex) return;

        for (Edge& edge : graph[current_vertex])
        {
            if (fines[edge.from] + edge.fine < fines[edge.to])
            {
                fines[edge.to] = fines[edge.from] + edge.fine;

                if (edge.fine == 0) bi_queue.push_front(edge.to);
                else                bi_queue.push_back(edge.to);
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

    Graph graph(amount_vertices);
    inputGraph(graph, amount_edges);

    int amount_requests = 0;
    cin >> amount_requests;

    vector<int> fines(amount_vertices, amount_vertices + 1);
    for (int i = 0; i < amount_requests; ++i)
    {
        int start = 0;
        int end = 0;

        cin >> start >> end;

        calculateFines(graph, fines, start - 1, end - 1);

        cout << ((fines[end - 1] == amount_vertices + 1) ? -1 : fines[end - 1]) << endl;

        std::fill(fines.begin(), fines.end(), amount_vertices + 1);
    }

    return 0;
}
