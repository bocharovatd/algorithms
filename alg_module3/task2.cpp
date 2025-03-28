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
#include <sstream>
#include <queue>
#include <vector>

int maxInt = std::numeric_limits<int>::max();

struct IGraph {
	virtual ~IGraph() {}
	// Добавление ребра от from к to.
	virtual void AddEdge( int from, int to ) = 0;
	virtual int VerticesCount() const = 0;
	virtual std::vector<int> GetNextVertices( int vertex ) const = 0;
	virtual std::vector<int> GetPrevVertices( int vertex ) const = 0;
};

class ListGraph : public IGraph {
public:
    ListGraph(int vertexCount);
    ListGraph(const IGraph &graph);

    // IGraph
    virtual void AddEdge(int from, int to) override;
    virtual int VerticesCount() const override;
    virtual std::vector<int> GetNextVertices(int vertex) const override;
    virtual std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::vector<int>> adjLists;
    std::vector<std::vector<int>> prevAdjLists;
};

ListGraph::ListGraph(int vertexCount) {
    assert(vertexCount > 0);
    adjLists.resize(vertexCount);
    prevAdjLists.resize(vertexCount);
}

ListGraph::ListGraph(const IGraph &graph) {
    adjLists.resize(graph.VerticesCount());
    prevAdjLists.resize(graph.VerticesCount());
    for (int i = 0; i < VerticesCount(); ++i) {
        adjLists[i] = graph.GetNextVertices(i);
        prevAdjLists[i] = graph.GetPrevVertices(i);
    }
}

void ListGraph::AddEdge(int from, int to) {
    assert(from >= 0 && from < adjLists.size());
    assert(to >= 0 && to < adjLists.size());
    adjLists[from].push_back(to);
    prevAdjLists[to].push_back(from);
}

int ListGraph::VerticesCount() const {
    return adjLists.size();
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjLists.size());
    return adjLists[vertex];
}

std::vector<int> ListGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjLists.size());
    return prevAdjLists[vertex];
}

int countDifferentWays(const IGraph &graph, int from, int to) {
	std::vector<int> paths(graph.VerticesCount(), 0);
    std::vector<int> dist(graph.VerticesCount(), maxInt);
    dist[from] = 0;
    paths[from] = 1;
    std::vector<bool> visited(graph.VerticesCount(), false);
    std::queue<int> bfsQ;
	bfsQ.push(from);
	while (!bfsQ.empty()) {
		int current = bfsQ.front();
		bfsQ.pop();
        if (visited[current]) {
            continue;
        }
		visited[current] = true;
		std::vector<int> adjList = graph.GetNextVertices(current);
		for (int i = 0; i < adjList.size(); ++i) {
            if (dist[adjList[i]] > dist[current] + 1) {
                dist[adjList[i]] = dist[current] + 1;
                paths[adjList[i]] = paths[current];
            }
            else if (dist[adjList[i]] == dist[current] + 1) {
                paths[adjList[i]] += paths[current];
            }
            if (!visited[adjList[i]]) {
                bfsQ.push(adjList[i]);
            }
		}
	}
    return paths[to];
}

void Run(std::istream &input, std::ostream &output) {
    int v = 0;
    int n = 0;
    input >> v >> n;
    ListGraph graph(v);
    for (int i = 0; i < n; ++i) {
        int vertexA, vertexB;
        input >> vertexA >> vertexB;
        graph.AddEdge(vertexA, vertexB);
        graph.AddEdge(vertexB, vertexA);
    }
    int from, to;
    input >> from >> to;
    output << countDifferentWays(graph, from, to);
}

void TestCountDifferentWays() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "4 5 0 1 0 2 1 2 1 3 2 3 0 3";
        Run(input, output);
        assert(output.str() == "2");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "7 9 0 1 0 2 0 3 1 2 1 4 3 2 3 4 2 4 5 6 0 4";
        Run(input, output);
        assert(output.str() == "3");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "9 12 0 1 0 2 1 3 1 4 2 3 2 5 3 6 4 6 5 6 5 7 6 7 7 8 0 8";
        Run(input, output);
        assert(output.str() == "1");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "5 5 0 1 0 2 1 3 2 4 3 4 1 0";
        Run(input, output);
        assert(output.str() == "1");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "5 10 0 1 0 2 0 3 0 4 1 2 1 3 1 4 2 3 2 4 3 4 0 4";
        Run(input, output);
        assert(output.str() == "1");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "5 10 0 1 0 2 0 3 0 4 1 2 1 3 1 4 2 3 2 4 3 4 0 4";
        Run(input, output);
        assert(output.str() == "1");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "13 18 0 1 0 2 0 3 1 4 2 4 3 4 4 5 4 6 4 7 5 8 6 8 7 8 8 9 8 10 8 11 9 12 10 12 11 12 0 12";
        Run(input, output);
        assert(output.str() == "27");
    }
}

int main() {
    TestCountDifferentWays();
    //Run(std::cin, std::cout);
    return 0;
}

