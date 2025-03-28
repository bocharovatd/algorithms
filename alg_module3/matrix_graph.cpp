#include "matrix_graph.hpp"

MatrixGraph::MatrixGraph(int vertexCount) {
    assert(vertexCount > 0);
    adjMatrix.resize(vertexCount);
    prevAdjMatrix.resize(vertexCount);
    for (int i = 0; i < adjMatrix.size(); ++i) {
        adjMatrix[i].resize(vertexCount);
        prevAdjMatrix[i].resize(vertexCount);
        std::fill(adjMatrix[i].begin(), adjMatrix[i].end(), 0);
        std::fill(prevAdjMatrix[i].begin(), prevAdjMatrix[i].end(), 0);
    }
}

MatrixGraph::MatrixGraph(const IGraph &graph) {
    MatrixGraph(graph.VerticesCount());
    for (int i = 0; i < graph.VerticesCount(); ++i) {
        for (auto j : graph.GetNextVertices(i)) {
            adjMatrix[i][j] = 1;
        }
        for (auto j : graph.GetPrevVertices(i)) {
            prevAdjMatrix[i][j] = 1;
        }
    }
}

void MatrixGraph::AddEdge(int from, int to) {
    assert(from >= 0 && from < adjMatrix.size());
    assert(to >= 0 && to < adjMatrix.size());
    adjMatrix[from][to] = 1;
    prevAdjMatrix[to][from] = 1;
}

int MatrixGraph::VerticesCount() const {
    return adjMatrix.size();
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjMatrix.size());
    std::vector<int> nextVertices;
    for (int j = 0; j < adjMatrix.size(); ++j) {
        if (adjMatrix[vertex][j]) {
            nextVertices.push_back(j);
        }
    }
    return nextVertices;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjMatrix.size());
    std::vector<int> prevVertices;
    for (int j = 0; j < prevAdjMatrix.size(); ++j) {
        if (prevAdjMatrix[vertex][j]) {
            prevVertices.push_back(j);
        }
    }
    return prevVertices;
}