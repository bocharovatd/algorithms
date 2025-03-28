#include "i_graph.hpp"

#include <functional>
#include <queue>

void bfs(const IGraph &graph, const std::function<void(int)>& visit, std::vector<bool>& visited);
void BFS(const IGraph &graph, const std::function<void(int)>& visit);