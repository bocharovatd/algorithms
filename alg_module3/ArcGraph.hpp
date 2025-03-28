#pragma once 

#include <cassert>
#include <cmath>
#include <vector>

struct Point {
    double x;
    double y;
    bool operator<(const Point &point) {
        if (this->x == point.x) {
            return this->y < point.y;
        }
        return this->x < point.x;
    }
};

struct Vertice {
    int num;
    Point point;
};

struct Arc {
    Vertice from;
    Vertice to;
    double weight;
};

class ArcGraph {
public:
    ArcGraph(int arcsCount_, int verticesCount_);
    ArcGraph(const ArcGraph &graph);
    ~ArcGraph();
    void AddEdge(const Vertice &from, const Vertice &to);
    int ArcsCount() const;
    int VerticesCount() const;
    int ArcsWeight() const;
    Arc* GetArcs();
    std::vector<Vertice> GetNextVertices(int num) const;


private:
    Arc* arcs;
    int arcsCount;
    int verticesCount;
    int arcsWeight;
    int currentEdge;
};
