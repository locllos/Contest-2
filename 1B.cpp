#include <iostream>
#include <vector>
#include <stack>

using std::vector;
using std::stack;

using std::cin;
using std::cout;


class IGraph
{
    virtual void Input() = 0;
};

class Graph : public IGraph
{
private:

    size_t amount_edges_;
    size_t amount_vertices_;

    vector<vector<int>> graph_;

    enum Color
    {
        WHITE,
        GRAY,
        BLACK,

        AMOUNT_COLORS,
    };

private:

    bool isSplitable(int start_vertex, vector<Color>& color)
    {
        size_t is_splitable = true;
        for (int current_vertex : graph_[start_vertex])
        {
            if (color[current_vertex] == WHITE)
            {
                color[current_vertex] = static_cast<Color>(AMOUNT_COLORS - color[start_vertex]);
                is_splitable = isSplitable(current_vertex, color); 
            }
            else if (color[current_vertex] ==
                     color[start_vertex])
            {
                is_splitable = false;
            }
        }
        return is_splitable;
    }

public:

    explicit Graph(size_t amount_edges, size_t amount_vertices)
        : amount_edges_(amount_edges)
        , amount_vertices_(amount_vertices)
        , graph_(amount_vertices_, vector<int>())
        {}

    void Input() override
    {
        int vertex_a = 0;
        int vertex_b = 0;
        for (size_t i = 0; i < amount_edges_; ++i)
        {   
            cin >> vertex_a >> vertex_b;

            graph_[vertex_a - 1].push_back(vertex_b - 1);
            graph_[vertex_b - 1].push_back(vertex_a - 1);
        }
    }

    bool isSplitable()
    {
        vector<Color> color(amount_vertices_, WHITE);

        bool is_splitable = true;
        for (size_t v = 0; v < amount_vertices_; ++v)
        {
            if (color[v] == WHITE) 
            {
                color[v] = GRAY;
                is_splitable = is_splitable && isSplitable(v, color);
            }
        }
        if (amount_vertices_ == 1)
        {
            is_splitable = false;
        }

        return is_splitable;
    }

};


void Processing()
{
    size_t amount_vertices = 0;
    size_t amount_edges = 0;
    cin >> amount_vertices >> amount_edges;

    Graph graph(amount_vertices, amount_edges);

    graph.Input();

    cout << (graph.isSplitable() ? "YES" : "NO");
}

int main()
{
    Processing();

    return 0;
}