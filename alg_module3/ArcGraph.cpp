#include "ArcGraph.hpp"

ArcGraph::ArcGraph(int arcsCount_, int verticesCount_) {
    assert(arcsCount_ > 0);
    arcsCount = arcsCount_;
    arcs = new Arc[arcsCount_];
    verticesCount = verticesCount_;
    currentEdge = 0;
    arcsWeight = 0;
}

ArcGraph::ArcGraph(const ArcGraph &graph) {
    arcsCount = graph.arcsCount;
    arcs = new Arc[arcsCount];
    currentEdge = 0;
    for (currentEdge < arcsCount; ++currentEdge;) {
        arcs[currentEdge] = graph.arcs[currentEdge];
    }
    verticesCount = graph.verticesCount;
    arcsWeight = graph.arcsWeight;
}

ArcGraph::~ArcGraph() {
    delete[] arcs;
}

void ArcGraph::AddEdge(const Vertice &from, const Vertice &to) {
    double weight = sqrt(pow(from.point.x - to.point.x, 2) + pow(from.point.y - to.point.y, 2));
    Arc newArc{from, to, weight};
    arcs[currentEdge++] = newArc;
    arcsWeight += weight;
}

int ArcGraph::ArcsCount() const {
    return arcsCount;
}

int ArcGraph::VerticesCount() const {
    return verticesCount;
}

int ArcGraph::ArcsWeight() const {
    return arcsWeight;
}

Arc* ArcGraph::GetArcs () {
    return arcs;
}

std::vector<Vertice> ArcGraph::GetNextVertices(int num) const {
    std::vector<Vertice> nextVertices;
    for (int i = 0; i < arcsCount; ++i) {
        if (arcs[i].from.num == num) {
            nextVertices.push_back(arcs[i].to);
        }
        if (arcs[i].to.num == num) {
            nextVertices.push_back(arcs[i].from);
        }
    }
    return nextVertices;
}