#include <iostream>
#include <vector>
#include <queue>

using std::vector;
using std::queue;
using std::cin;
using std::cout;
using std::endl;

struct Edge
{   
    int from;
    int to;
    int distance;
};

using Graph = vector<vector<Edge>>;

const int INF = (int)1e9;

void InputGraph(Graph& graph, size_t amount_edges, int& max_distance)
{
    int distance = 0;
    int vertex_to = 0;
    int vertex_from = 0;

    for (size_t i = 0; i < amount_edges; ++i)
    {   
        cin >> vertex_from >> vertex_to >> distance;

        if (max_distance < distance) max_distance = distance;

        graph[vertex_from - 1].push_back({vertex_from - 1, vertex_to - 1, distance});
    }
}

int getDistance(Graph& graph, int start, int end, int max_distance)
{   
    vector<int> distances(graph.size(), INF);
    vector<queue<int>> vector_queue(max_distance + 1);
    
    vector_queue[0].push(start);
    distances[start] = 0;

    int amount_vertices = graph.size();
    for (int queue_idx = 0; 
         queue_idx < amount_vertices * max_distance + 1; 
         ++queue_idx)
    {   
        queue<int>& current = vector_queue[queue_idx % (max_distance + 1)];
        while (!current.empty())
        {
            int front_vertex = current.front();
            current.pop();

            if (front_vertex == end) return distances[end];

            for (Edge& edge : graph[front_vertex])
            {
                if (distances[edge.from] + edge.distance < distances[edge.to])
                {
                    distances[edge.to] = distances[edge.from] + edge.distance;
                    vector_queue[(queue_idx + edge.distance) % (max_distance + 1)].push(edge.to);
                }
            }
        }
    }
    return -1;
}

int main()
{   
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    size_t amount_vertices = 0;
    size_t amount_edges = 0;
    cin >> amount_vertices >> amount_edges;       
    
    int start = 0;
    int end = 0;
    cin >> start >> end;

    int max_distance = 0;
    Graph graph(amount_vertices, vector<Edge>());
    InputGraph(graph, amount_edges, max_distance);

    cout << getDistance(graph, start - 1, end - 1, max_distance);

    return 0;
}