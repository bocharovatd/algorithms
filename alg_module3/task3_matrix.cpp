// Общие требования для всех задач
// Ввод/вывод отделены от решения.
// Не должно быть утечек памяти.
// Задача 2. Количество различных путей
// Дан невзвешенный неориентированный граф. 
// В графе может быть несколько кратчайших путей между какими-то вершинами. Найдите количество различных кратчайших путей между заданными вершинами. 
// Требования: сложность O(V+E). 
// Формат ввода.
// v: кол-во вершин (макс. 50000),
// n: кол-во ребер (макс. 200000),
// n пар реберных вершин,
// пара вершин u, w для запроса.
// Формат вывода.
// Количество кратчайших путей от u к w.

#include <cassert>
#include <functional>
#include <iostream>
#include <limits>
#include <set>
#include <sstream>
#include <vector>

int maxInt = std::numeric_limits<int>::max();

class MatrixGraph {
public:
    MatrixGraph(int vertexCount);
    void AddEdge(int from, int to, int weight);
    int GetWeight(int from, int to) const;
    int VerticesCount() const;
    std::vector<int> GetNextVertices(int vertex) const;

private:
    std::vector<std::vector<int>> adjMatrix;
};

MatrixGraph::MatrixGraph(int vertexCount) {
    assert(vertexCount > 0);
    adjMatrix.resize(vertexCount);
    for (int i = 0; i < adjMatrix.size(); ++i) {
        adjMatrix[i].resize(vertexCount);
        std::fill(adjMatrix[i].begin(), adjMatrix[i].end(), 0);
    }
}

void MatrixGraph::AddEdge(int from, int to, int weight) {
    assert(from >= 0 && from < adjMatrix.size());
    assert(to >= 0 && to < adjMatrix.size());
    if (adjMatrix[from][to] != 0) {
        if (adjMatrix[from][to] > weight) {
            adjMatrix[from][to] = weight;
        }
    }
    else {
        adjMatrix[from][to] = weight;
    }
}

int MatrixGraph::GetWeight(int from, int to) const {
    assert(from >= 0 && from < adjMatrix.size());
    assert(to >= 0 && to < adjMatrix.size());
    return adjMatrix[from][to];
}

int MatrixGraph::VerticesCount() const {
    return adjMatrix.size();
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjMatrix.size());
    std::vector<int> nextVertices;
    for (int j = 0; j < adjMatrix.size(); ++j) {
        if (adjMatrix[vertex][j]) {
            nextVertices.push_back(j);
        }
    }
    return nextVertices;
}

int lengthShortestWay(const MatrixGraph &graph, int from, int to) {
    std::vector<int> dist(graph.VerticesCount(), maxInt);
    dist[from] = 0;
    std::set<std::pair<int, int>> priorityQueue;
	priorityQueue.insert(std::make_pair(dist[from], from));
	while (!priorityQueue.empty()) {
		int current = priorityQueue.begin()->second;
		priorityQueue.erase(priorityQueue.begin());
		std::vector<int> adjList = graph.GetNextVertices(current);
		for (int i = 0; i < adjList.size(); ++i) {
            if (dist[adjList[i]] == maxInt) {
                dist[adjList[i]] = dist[current] + graph.GetWeight(adjList[i], current);
                priorityQueue.insert(std::make_pair(dist[adjList[i]], adjList[i]));
            }
            else if (dist[adjList[i]] > dist[current] + graph.GetWeight(adjList[i], current)) {
                priorityQueue.erase(std::make_pair(dist[adjList[i]], adjList[i]));
                dist[adjList[i]] = dist[current] + graph.GetWeight(adjList[i], current);
                priorityQueue.insert(std::make_pair(dist[adjList[i]], adjList[i]));
            }
        }
    }
    return dist[to];
}

void Run(std::istream &input, std::ostream &output) {
    int n = 0;
    int m = 0;
    input >> n >> m;
    MatrixGraph graph(n);
    for (int i = 0; i < m; ++i) {
        int vertexA, vertexB;
        int weight;
        input >> vertexA >> vertexB >> weight;
        graph.AddEdge(vertexA, vertexB, weight);
        graph.AddEdge(vertexB, vertexA, weight);
    }
    int from, to;
    input >> from >> to;
    output << lengthShortestWay(graph, from, to);
}

void TestCountDifferentWays() {
        {
            std::stringstream input;
            std::stringstream output;
            input << "6 9 0 3 1 0 4 2 1 2 7 1 3 2 1 4 3 1 5 3 2 5 3 3 4 4 3 5 6 0 2";
            Run(input, output);
            assert(output.str() == "9");
        }
        {
            std::stringstream input;
            std::stringstream output;
            input << "3 3 0 1 5 1 1 2 1 2 3 0 2";
            Run(input, output);
            assert(output.str() == "8");
        }
}

int main() {
    TestCountDifferentWays();
    //Run(std::cin, std::cout);
    return 0;
}
