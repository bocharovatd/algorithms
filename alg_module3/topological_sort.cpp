#include "topological_sort.hpp"

void topologicalSort(const IGraph &graph, int vertex, std::vector<bool>& visited, std::list<int>& sorted) {
	std::stack<std::pair<bool, int>> dfsS;
    // bool параметр нужен, чтобы обозначать, где для каждой вершины начались потомки
    // После true идут только потомки этой вершины.
    // И, если они все извлечены из стека (мы наткнулись на true в стеке), 
    // то дальше зависимостей нет и узел можно помещать в ответ
	dfsS.push(std::make_pair(false, vertex));
	while (!dfsS.empty()) {
        int current_flag = dfsS.top().first;
		int current = dfsS.top().second;
		dfsS.pop();
        if (current_flag) {
            // Уже обработаны все потомки
            sorted.push_front(current);
            continue;
        }
        if (visited[current]) {
            continue;
        }
        visited[current] = true;
        dfsS.push(std::make_pair(true, current));
        // Начало потомков текущей вершины в стеке
		std::vector<int> adjList = graph.GetNextVertices(current);
		for (int i = 0; i < adjList.size(); ++i) {
			if (!visited[adjList[i]]) {
				dfsS.push(std::make_pair(false, adjList[i]));
			}
		}
    }
}

std::list<int> TopologicalSort(const IGraph &graph) {
    std::vector<bool> visited(graph.VerticesCount(), false);
    std::list<int> sorted;
    for (int i = 0; i < graph.VerticesCount(); ++i) {
        if (!visited[i]) {
            topologicalSort(graph, i, visited, sorted);
        }
    }
    return sorted;
}

