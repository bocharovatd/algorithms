// 2. Проверка пути на гамильтоновость
// Нужно проверить, является ли путь в неориентированном графе гамильтоновым.
// Формат ввода
// Первая строка содержит число N – количество вершин.
// Вторая строка содержит число M - количество ребер.
// Третья строка содержит число K - количество вершин в пути.
// Далее идут M строк с ребрами (откуда, куда).
// Далее идут K вершин, принадлежащих пути.
// Формат вывода
// 1 - если путь гамильтонов
// 0 - если путь не гамильтонов

#include <iostream>
#include <vector>
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
    virtual bool HasNextVertice( int vertex, int findVertex ) const = 0;
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
    virtual bool HasNextVertice(int vertex, int findVertex) const override;

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

bool ListGraph::HasNextVertice(int vertex, int findVertex) const {
    assert(vertex >= 0 && vertex < adjLists.size());
    for (auto element : adjLists[vertex]) {
        if (element == findVertex) {
            return true;
        }
    }
    return false;
}

bool IsHamiltonianWay(const std::vector<int> &vertices, const IGraph &graph) {
    std::vector<bool> visited(graph.VerticesCount(), false); 
    visited[vertices[0]] = true;
    int countVisited = 1;
    for (int i = 1; i < vertices.size(); ++i) {
        if (visited[vertices[i]]) {
            return false;
        }
        if (!graph.HasNextVertice(vertices[i - 1], vertices[i])) {
            return false;
        }
        visited[vertices[i]] = true;
        ++countVisited;
    }
    if (countVisited != graph.VerticesCount()) {
        return false;
    }
    return true;
}

void Run(std::istream &input, std::ostream &output) {
    int n = 0;
    int m = 0;
    int k;
    input >> n >> m >> k;
    IGraph* listGraph = new ListGraph(n);
    for (int i = 0; i < m; ++i) {
        int vertexA = 0;
        int vertexB = 0;
        input >> vertexA >> vertexB;
        listGraph->AddEdge(vertexA, vertexB);
        listGraph->AddEdge(vertexB, vertexA);
    }
    std::vector<int> vertices;
    vertices.reserve(k);
    for (int i = 0; i < k; ++i) {
        int vertex = 0;
        input >> vertex;
        vertices.push_back(vertex);
    }
    output << IsHamiltonianWay(vertices, *listGraph);
    delete listGraph;
}

void TestCountConnectedComponents() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "4 4 4 0 1 1 2 2 3 3 0 0 1 2 3";
        Run(input, output);
        assert(output.str() == "1");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "4 4 5 0 1 1 2 1 3 0 1 2 1 3";
        Run(input, output);
        assert(output.str() == "0");
    }
}

int main() {
    //Run(std::cin, std::cout);
    TestCountConnectedComponents();
    return 0;
}