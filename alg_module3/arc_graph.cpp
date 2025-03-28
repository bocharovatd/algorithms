#include "arc_graph.hpp"

ArcGraph::ArcGraph(int vertexCount) {
    assert(vertexCount > 0);
    verticesCount = vertexCount;
}

ArcGraph::ArcGraph(const IGraph &graph) {
    verticesCount = graph.VerticesCount();
    for (int i = 0; i < verticesCount; ++i) {
        for (auto j : graph.GetNextVertices(i)) {
            arcs.push_back(std::make_pair(i, j));
        }
    }
}

void ArcGraph::AddEdge(int from, int to) {
    assert(from >= 0 && from < verticesCount);
    assert(to >= 0 && to < verticesCount);
    arcs.push_back(std::make_pair(from, to));
}

int ArcGraph::VerticesCount() const {
    return verticesCount;
}

std::vector<int> ArcGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < verticesCount);
    std::vector<int> nextVertices;
    for (int i = 0; i < arcs.size(); ++i) {
        if (arcs[i].first == vertex) {
            nextVertices.push_back(arcs[i].second);
        }
    }
    return nextVertices;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < verticesCount);
    std::vector<int> prevVertices;
    for (int i = 0; i < arcs.size(); ++i) {
        if (arcs[i].second == vertex) {
            prevVertices.push_back(arcs[i].first);
        }
    }
    return prevVertices;
}