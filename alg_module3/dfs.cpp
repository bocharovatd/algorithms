#include "dfs.hpp"

void dfs(const IGraph &graph, int vertex, const std::function<void(int)>& visit, std::vector<bool>& visited) {
	std::stack<int> dfsS;
	dfsS.push(vertex);
	while (!dfsS.empty()) {
		int current = dfsS.top();
		dfsS.pop();
		if (visited[current]) {
            continue;
        }
		visit(current);
		visited[current] = true;
		std::vector<int> adjList = graph.GetNextVertices(current);
		for (int i = 0; i < adjList.size(); ++i) {
			if (!visited[adjList[i]]) {
				dfsS.push(adjList[i]);
			}
		}
	}
}

void DFS(const IGraph &graph, const std::function<void(int)>& visit) {
    std::vector<bool> visited(graph.VerticesCount(), false);
    for (int i = 0; i < graph.VerticesCount(); ++i) {
        if (!visited[i]) {
            dfs(graph, i, visit, visited);
        }
    }
}
