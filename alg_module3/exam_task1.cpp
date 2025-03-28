// 1. Количество компонент связности
// Посчитать кол-во компонент связности в неориентированном графе.
// Формат ввода
// Первая строка содержит число N – количество вершин.
// Вторая строка содержит число M - количество ребер.
// Каждая следующая строка содержит ребро (откуда, куда).
// Формат вывода
// Целое число - кол-во компонент связности в графе.

#include <iostream>
#include <functional>
#include <stack>
#include <cassert>
#include <sstream>

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

void dfs(const IGraph &graph, int vertex, std::vector<bool>& visited) {
	std::stack<int> dfsS;
	dfsS.push(vertex);
	while (!dfsS.empty()) {
		int current = dfsS.top();
		dfsS.pop();
		if (visited[current]) {
            continue;
        }
		visited[current] = true;
		std::vector<int> adjList = graph.GetNextVertices(current);
		for (int i = 0; i < adjList.size(); ++i) {
			if (!visited[adjList[i]]) {
				dfsS.push(adjList[i]);
			}
		}
	}
}

int CountConnectedComponents(const IGraph &graph) {
    int result = 0;
    std::vector<bool> visited(graph.VerticesCount(), false);
    for (int i = 0; i < graph.VerticesCount(); ++i) {
        if (!visited[i]) {
            ++result;
            dfs(graph, i, visited);
        }
    }
    return result;
}


void Run(std::istream &input, std::ostream &output) {
    int n = 0;
    int m = 0;
    input >> n >> m;
    IGraph* listGraph = new ListGraph(n);
    for (int i = 0; i < m; ++i) {
        int vertexA = 0;
        int vertexB = 0;
        input >> vertexA >> vertexB;
        listGraph->AddEdge(vertexA, vertexB);
        listGraph->AddEdge(vertexB, vertexA);
    }
    output << CountConnectedComponents(*listGraph);
    delete listGraph;
}

void TestCountConnectedComponents() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "3 2 0 1 0 2";
        Run(input, output);
        assert(output.str() == "1");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "4 2 0 1 0 2";
        Run(input, output);
        assert(output.str() == "2");
    }
}

int main() {
    //Run(std::cin, std::cout);
    TestCountConnectedComponents();
    return 0;
}
