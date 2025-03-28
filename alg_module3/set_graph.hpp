#pragma once 

#include "i_graph.hpp"
#include "hash_table.cpp"

class SetGraph : public IGraph {
public:
    SetGraph(int vertexCount);
    SetGraph(const IGraph &graph);

    // IGraph
    virtual void AddEdge(int from, int to) override;
    virtual int VerticesCount() const override;
    virtual std::vector<int> GetNextVertices(int vertex) const override;
    virtual std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<HashTable> adjHashTables;
    std::vector<HashTable> prevAdjHashTables;
};
