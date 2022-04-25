#include <algorithm>
#include <queue>

#include "graph.h"

struct ShortestPath {
    size_t dist;
    std::vector<Graph::vertex_t> parent;
    std::vector<Graph::vertex_t> path;
};

void bFS(const Graph& graph, Graph::vertex_t start_ver, std::vector<Graph::vertex_t>& parent, std::vector<size_t>& dist) {
    std::queue<Graph::vertex_t> queue_of_vertex;
    queue_of_vertex.push(start_ver);

    while (!queue_of_vertex.empty()) {
        Graph::vertex_t now_vertex = queue_of_vertex.front();
        queue_of_vertex.pop();

        for (Graph::vertex_t neighbor : graph.getNeighbors(now_vertex)) {
            if (dist[neighbor] == UNDEFINED_DIST) {
                dist[neighbor] = dist[now_vertex] + 1;
                parent[neighbor] = now_vertex;
                queue_of_vertex.push(neighbor);
            }
        }
    }
}

void pathRestoration(ShortestPath& shortest_path, const Graph::vertex_t& to) {
    if (shortest_path.dist == UNDEFINED_DIST) {
        return;
    }

    shortest_path.path.push_back(to);
    Graph::vertex_t now_child = to;
    for (int i = 0; i < shortest_path.dist; i++) {
        shortest_path.path.push_back(shortest_path.parent[now_child]);
        now_child = shortest_path.parent[now_child];
    }

    std::reverse(shortest_path.path.begin(), shortest_path.path.end());
}

ShortestPath findTheShortestPath(const Graph& graph, Graph::vertex_t from, Graph::vertex_t to) {
    std::vector<Graph::vertex_t> parent(graph.getNumberOfVertex() + 1);
    std::vector<size_t> dist(graph.getNumberOfVertex() + 1, UNDEFINED_DIST);
    dist[from] = 0;
    parent.resize(graph.getNumberOfVertex() + 1);
    parent[from] = 0;

    bFS(graph, from, parent, dist);

    std::vector<Graph::vertex_t> path;

    ShortestPath result = {dist[to], parent, path};

    pathRestoration(result, to);

    return result;
}

int main() {
    int number_vertices = 0;
    int number_edges = 0;

    std::cout << "Get: number of vertices and number of edges\n";
    std::cin >> number_vertices >> number_edges;
    GraphAdjList graph(number_vertices, number_edges);

    Graph::vertex_t start_ver;
    Graph::vertex_t finish_ver;

    std::cout << "Get: start point and finish point between path\n";
    std::cin >> start_ver >> finish_ver;

    std::cout << "Get: full graph [from; to]\n";
    for (int i = 0; i < number_edges; i++) {
        Graph::vertex_t from;
        Graph::vertex_t to;
        std::cin >> from >> to;
        graph.addEdge(from, to);
    }

    ShortestPath shortest_path = findTheShortestPath(graph, start_ver, finish_ver);

    if (shortest_path.dist == UNDEFINED_DIST) {
        std::cout << "UNDEFINED DIST\n" << '\n';
    } else {
        std::cout << "Shortest path len: " <<  shortest_path.dist << '\n';
    }


    std::cout << "Path:\n";
    for (Graph::vertex_t now_vertex : shortest_path.path) {
        std::cout << now_vertex << " ";
    }

    return 0;
}