#pragma once 

#include <vector>

class DSU {
public:
    DSU(int count);
    int FindParent(int v);
    void Union(int vi, int v2);

private:
    std::vector<int> parent;
    std::vector<int> rank;
};