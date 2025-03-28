#include "i_graph.hpp"

#include <functional>
#include <stack>

void dfs(const IGraph &graph, int vertex, void (*visit)(int), std::vector<bool>& visited);
void DFS(const IGraph &graph, void (*visit)(int));