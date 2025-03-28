#include "set_graph.hpp"

SetGraph::SetGraph(int vertexCount) {
    assert(vertexCount > 0);
    adjHashTables.resize(vertexCount);
    prevAdjHashTables.resize(vertexCount);
}

SetGraph::SetGraph(const IGraph &graph) {
    adjHashTables.resize(graph.VerticesCount());
    prevAdjHashTables.resize(graph.VerticesCount());
    for (int i = 0; i < graph.VerticesCount(); ++i) {
        for (auto j : graph.GetNextVertices(i)) {
            adjHashTables[i].Add(j);
        }
        for (auto j : graph.GetPrevVertices(i)) {
            prevAdjHashTables[i].Add(j);
        }
    }
}

void SetGraph::AddEdge(int from, int to) {
    assert(from >= 0 && from < adjHashTables.size());
    assert(to >= 0 && to < adjHashTables.size());
    adjHashTables[from].Add(to);
    prevAdjHashTables[to].Add(from);
}

int SetGraph::VerticesCount() const {
    return adjHashTables.size();
}

std::vector<int> SetGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjHashTables.size());
    std::vector<int> nextVertices;
    for (int j = 0; j < adjHashTables.size(); ++j) {
        if (adjHashTables[vertex].Has(j)) {
            nextVertices.push_back(j);
        }
    }
    return nextVertices;
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < prevAdjHashTables.size());
    std::vector<int> prevVertices;
    for (int j = 0; j < adjHashTables.size(); ++j) {
        if (prevAdjHashTables[vertex].Has(j)) {
            prevVertices.push_back(j);
        }
    }
    return prevVertices;
}