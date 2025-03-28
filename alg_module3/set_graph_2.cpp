#include "set_graph_2.hpp"

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
            adjHashTables[i].insert(j);
        }
        for (auto j : graph.GetPrevVertices(i)) {
            prevAdjHashTables[i].insert(j);
        }
    }
}

void SetGraph::AddEdge(int from, int to) {
    assert(from >= 0 && from < adjHashTables.size());
    assert(to >= 0 && to < adjHashTables.size());
    adjHashTables[from].insert(to);
    prevAdjHashTables[to].insert(from);
}

int SetGraph::VerticesCount() const {
    return adjHashTables.size();
}

std::vector<int> SetGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjHashTables.size());
    std::vector<int> nextVertices;
    for (int j = 0; j < adjHashTables.size(); ++j) {
        if (adjHashTables[vertex].find(j) != adjHashTables[vertex].end()) {
            nextVertices.push_back(j);
        }
    }
    return nextVertices;
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < prevAdjHashTables.size());
    std::vector<int> prevVertices;
    for (int j = 0; j < adjHashTables.size(); ++j) {
        if (prevAdjHashTables[vertex].find(j) != prevAdjHashTables[vertex].end()) {
            prevVertices.push_back(j);
        }
    }
    return prevVertices;
}