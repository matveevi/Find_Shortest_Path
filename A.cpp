#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

enum { UNDEFINED_DIST = 1000000000LL, THERE_IS_EDGE = 1, THERE_IS_NOT_EDGE = 0 };

class Graph {
protected:
    bool isDirected;
    size_t num_of_vertex;
    size_t num_of_edges;

public:
    Graph() {
        isDirected = false;
        num_of_vertex = 0;
        num_of_edges = 0;
    }

    typedef size_t vertex_t;

    void addEdge(const vertex_t& from, const vertex_t& to) { num_of_edges++; }

    size_t getNumberOfVertex() const { return num_of_vertex; }

    size_t getNumberOfEdges() const { return num_of_edges; }

    virtual std::vector<vertex_t> getNeighbors(const vertex_t& vertex) const = 0;

    virtual void PrintGraph() const = 0;
};

class GraphAdjList : public Graph {
    std::vector<std::vector<vertex_t>> adj_list;

public:
    void IsDirected() { isDirected = true; }

    void IsNotDirected() { isDirected = false; }

    void PrintGraph() const {
        for (int i = 0; i < adj_list.size(); i++) {
            std::cout << i << " - ";
            for (int j = 0; j < adj_list[i].size(); j++) {
                std::cout << adj_list[i][j];
            }
            std::cout << '\n';
        }
    }

    GraphAdjList(size_t number_vertex, size_t number_edges) {
        num_of_vertex = number_vertex;
        num_of_edges = number_edges;
        adj_list.resize(number_vertex + 1);
    }

    std::vector<vertex_t> getNeighbors(const vertex_t& vertex) const { return adj_list[vertex]; }

    void addEdge(const vertex_t& from, const vertex_t& to) {
        adj_list[from].push_back(to);
        if (!isDirected) {
            adj_list[to].push_back(from);
        }
    }
};

class GraphAdjMatrix : public Graph {
    std::vector<std::vector<bool>> Matrix;

public:
    GraphAdjMatrix(size_t number_vertex, size_t number_edges) {
        num_of_vertex = number_vertex;
        num_of_edges = number_edges;
        Matrix.resize(number_vertex + 1);
        for (vertex_t vertex = 0; vertex < number_vertex + 1; vertex++) {
            Matrix[vertex].resize(number_vertex + 1);
        }
    }

    void PrintGraph() const {
        for (int y_ver = 0; y_ver < Matrix.size(); y_ver++) {
            for (int x_ver = 0; x_ver < Matrix.size(); x_ver++) {
                std::cout << Matrix[y_ver][x_ver];
            }
            std::cout << '\n';
        }
    }

    std::vector<vertex_t> getNeighbors(const vertex_t& vertex) const {
        std::vector<vertex_t> Neighbors;
        for (vertex_t neighbor = 0; neighbor < Matrix.size(); neighbor++) {
            if (Matrix[vertex][neighbor] == THERE_IS_EDGE) {
                Neighbors.push_back(neighbor);
            }
        }
        return Neighbors;
    }

    void addEdge(const vertex_t& from, const vertex_t& to) {
        Matrix[from][to] = THERE_IS_EDGE;
        if (!isDirected) {
            Matrix[to][from] = THERE_IS_EDGE;
        }
    }
};

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
    std::cin >> number_vertices >> number_edges;
    GraphAdjList graph(number_vertices, number_edges);

    Graph::vertex_t start_ver;
    Graph::vertex_t finish_ver;
    std::cin >> start_ver >> finish_ver;

    for (int i = 0; i < number_edges; i++) {
        Graph::vertex_t from;
        Graph::vertex_t to;
        std::cin >> from >> to;
        graph.addEdge(from, to);
    }

    ShortestPath shortest_path = findTheShortestPath(graph, start_ver, finish_ver);

    if (shortest_path.dist == UNDEFINED_DIST) {
        std::cout << "-1" << '\n';
    } else {
        std::cout << shortest_path.dist << '\n';
    }

    for (Graph::vertex_t now_vertex : shortest_path.path) {
        std::cout << now_vertex << " ";
    }

    return 0;
}