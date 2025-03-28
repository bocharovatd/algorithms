#include "i_graph.hpp"
#include "list_graph.cpp"
#include "matrix_graph.cpp"
#include "set_graph.cpp"
#include "arc_graph.cpp"
#include "bfs.cpp"
#include "dfs.cpp"
#include "topological_sort.cpp"

#include <cassert>
#include <iostream>
#include <list>
#include <sstream>

void Run(std::istream &input, std::ostream &output) {
    int vertexCount = 0;
    input >> vertexCount;
    std::vector<IGraph*> vecGraphs;
    IGraph* listGraph = new ListGraph(vertexCount);
    IGraph* matrixGraph = new MatrixGraph(vertexCount);
    IGraph* setGraph = new SetGraph(vertexCount);
    IGraph* arcGraph = new ArcGraph(vertexCount);
    vecGraphs.push_back(listGraph);
    vecGraphs.push_back(matrixGraph);
    vecGraphs.push_back(setGraph);
    vecGraphs.push_back(arcGraph);
    int n = 0;
    input >> n;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < n; ++j) {
            int vertexA, vertexB;
            input >> vertexA >> vertexB;
            vecGraphs[i]->AddEdge(vertexA, vertexB);
        }
        DFS(*vecGraphs[i], [&output](int vertex)
            { output << vertex << " "; });
        output << std::endl;
        BFS(*vecGraphs[i], [&output](int vertex)
            { output << vertex << " "; });
        output << std::endl;
        std::list<int> sorted = TopologicalSort(*vecGraphs[i]);
        for (auto element : sorted) {
            output << element << " ";
        }
        output << std::endl;
    }
    delete listGraph;
    delete matrixGraph;
    delete setGraph;
    delete arcGraph;
}

void TestGraphs() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "7 9 ";
        for (int i = 0; i < 4; ++i) {
            input << "0 1 0 2 0 3 1 2 1 4 3 2 3 4 2 4 5 6 ";
        }
        Run(input, output);
        std::stringstream answer;
        for (int i = 0; i < 4; ++i) {
            answer << "0 3 4 2 1 5 6 \n0 1 2 3 4 5 6 \n5 6 0 1 3 2 4 \n";
        }
        assert(output.str() == answer.str());
    }
}