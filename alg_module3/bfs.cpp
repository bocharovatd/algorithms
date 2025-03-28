#include "bfs.hpp"

void bfs(const IGraph &graph, int vertex, const std::function<void(int)>& visit, std::vector<bool>& visited) {
	std::queue<int> bfsQ;
	bfsQ.push(vertex);
	while (!bfsQ.empty()) {
		int current = bfsQ.front();
		bfsQ.pop();
		if (visited[current]) {
            continue;
        }
		visit(current);
		visited[current] = true;
		std::vector<int> adjList = graph.GetNextVertices(current);
		for (int i = 0; i < adjList.size(); ++i) {
			if (!visited[adjList[i]]) {
				bfsQ.push(adjList[i]);
			}
		}
	}
}

void BFS(const IGraph &graph, const std::function<void(int)>& visit) {
    std::vector<bool> visited(graph.VerticesCount(), false);
    for (int i = 0; i < graph.VerticesCount(); ++i) {
        if (!visited[i]) {
            bfs(graph, i, visit, visited);
        }
    }
}
