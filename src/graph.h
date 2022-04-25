#include <vector>
#include <iostream>

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