#include "i_graph.hpp"

#include <list>
#include <stack>

void topologicalSort(const IGraph &graph, int vertex, std::vector<bool>& visited, std::list<int>& sorted);
std::list<int> TopologicalSort(const IGraph &graph);