// Общие требования для всех задач
// Ввод/вывод отделены от решения.
// Не должно быть утечек памяти.
// Задача 3. Города
// Требуется отыскать самый короткий маршрут между городами. 
// Из города может выходить дорога, которая возвращается в этот же город.
// Требуемое время работы O((N + M)log N), где N – количество городов, M – известных дорог между ними.
// Формат ввода
// Первая строка содержит число N – количество городов, 3 ≤ N ≤ 10000.
// Вторая строка содержит число M - количество дорог, 1 ≤ M ≤ 250000.
// Каждая следующая строка содержит три числа u, v, w – описание дороги: 
// откуда, куда, время в пути. 0 ≤ u < N, 0 ≤ v < N, 1 ≤ w ≤ 10000. 
// Между любыми двумя городами может быть больше одной дороги.
// Последняя строка содержит два числа: s и t – маршрут (откуда и куда нужно доехать). 
// 0 ≤ s < N, 0 ≤ t < N.
// Формат вывода
// Вывести длину самого короткого маршрута.w.

#include <cassert>
#include <functional>
#include <iostream>
#include <limits>
#include <set>
#include <sstream>
#include <vector>

int maxInt = std::numeric_limits<int>::max();

class ListGraph {
public:
    ListGraph(int vertexCount);
    void AddEdge(int from, int to, int weight);
    int GetWeight(int from, int to) const;
    int VerticesCount() const;
    std::vector<int> GetNextVertices(int vertex) const;

private:
    std::vector<std::vector<int>> adjLists;
    std::vector<std::vector<int>> weightsLists;
};

ListGraph::ListGraph(int vertexCount) {
    assert(vertexCount > 0);
    adjLists.resize(vertexCount);
    weightsLists.resize(vertexCount);
}

void ListGraph::AddEdge(int from, int to, int weight) {
    assert(from >= 0 && from < adjLists.size());
    assert(to >= 0 && to < adjLists.size());
    for (int i = 0; i < adjLists[from].size(); ++i) {
        if (adjLists[from][i] == to) {
            if (weightsLists[from][i] > weight) {
                weightsLists[from][i] = weight;
            }
            return;
        }
    }
    adjLists[from].push_back(to);
    weightsLists[from].push_back(weight);
}

int ListGraph::GetWeight(int from, int to) const {
    for (int i = 0; i < adjLists[from].size(); ++i) {
        if (adjLists[from][i] == to) {
            return weightsLists[from][i];
        }
    }
    assert(false);
}

int ListGraph::VerticesCount() const {
    return adjLists.size();
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjLists.size());
    return adjLists[vertex];
}

int lengthShortestWay(const ListGraph &graph, int from, int to) {
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
    ListGraph graph(n);
    for (int i = 0; i < m; ++i) {
        int vertexA = 0;
        int vertexB = 0;
        int weight = 0;
        input >> vertexA >> vertexB >> weight;
        graph.AddEdge(vertexA, vertexB, weight);
        graph.AddEdge(vertexB, vertexA, weight);
    }
    int from = 0;
    int to = 0;
    input >> from >> to;
    output << lengthShortestWay(graph, from, to);
}

void TestLengthShortestWay() {
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
    TestLengthShortestWay();
    //Run(std::cin, std::cout);
    return 0;
}
