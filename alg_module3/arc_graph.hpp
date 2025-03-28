#pragma once 

#include "i_graph.hpp"

class ArcGraph : public IGraph {
public:
    ArcGraph(int vertexCount);
    ArcGraph(const IGraph &graph);

    // IGraph
    virtual void AddEdge(int from, int to) override;
    virtual int VerticesCount() const override;
    virtual std::vector<int> GetNextVertices(int vertex) const override;
    virtual std::vector<int> GetPrevVertices(int vertex) const override;

private:
    int verticesCount;
    std::vector<std::pair<int, int>> arcs;
};
