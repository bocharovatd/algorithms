// Общие требования для всех задач
// Ввод/вывод отделены от решения.
// Не должно быть утечек памяти.
// Задача 5. Приближенное решение метрической неориентированной задачи коммивояжера.
// Найдите приближенное решение метрической неориентированной задачи коммивояжера в полном графе (на плоскости) 
// с помощью минимального остовного дерева.
// Оцените качество приближения на случайном наборе точек, нормально распределенном на плоскости с дисперсией 1. 
// Нормально распределенный набор точек получайте с помощью преобразования Бокса-Мюллера.
// При фиксированном N, количестве вершин графа, несколько раз запустите оценку качества приближения. 
// Вычислите среднее значение и среднеквадратичное отклонение качества приближения для данного N.
// Запустите данный эксперимент для всех N в некотором диапазоне, например, [2, 10].
// Автоматизируйте запуск экспериментов.
// В решении требуется разумно разделить код на файлы. Каждому классу - свой заголовочный файл и файл с реализацией.
// Вариант 1. Для построения минимального остовного дерева используйте алгоритм Крускала.
// В контесте протестируйте работу алгоритма построения минимального остовного дерева. 
// (Варианты в контесте - не те, который описаны здесь. Правильные варианты - здесь.)

#include "ArcGraph.cpp"
#include "DSU.cpp"
#include "Sort.cpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <random>
#include <stack>
#include <sstream>
#include <unordered_set>
#include <vector>

std::default_random_engine generator;
std::uniform_real_distribution<double> dist(-1.0, 1.0);

struct IsLessArcs {
    bool operator()(const Arc& arcA, const Arc& arcB) {
        return arcA.weight < arcB.weight;
    }
};

void SortArcs(Arc* arcs, int n) {
    IsLessArcs isLessArcs;
    QuickSort(arcs, n, isLessArcs);
}

ArcGraph CreateMST(Arc* arcs, int n) {
    SortArcs(arcs, n);
    DSU dsu(n);
    std::vector<std::pair<Vertice, Vertice>> mstArcs;
    std::unordered_set<int> vertices;
    for (int i = 0; i < n; ++i) {
        Vertice v1 = arcs[i].from;
        Vertice v2 = arcs[i].to;
        if (dsu.FindParent(v1.num) != dsu.FindParent(v2.num)) {
            mstArcs.push_back(std::make_pair(v1, v2));
            vertices.insert(v1.num);
            vertices.insert(v2.num);
            dsu.Union(v1.num, v2.num);
        }
    }
    ArcGraph mstGraph(mstArcs.size(), vertices.size());
    for (int i = 0; i < mstArcs.size(); ++i) {
        Vertice verticeA = mstArcs[i].first;
        Vertice verticeB = mstArcs[i].second;
        mstGraph.AddEdge(verticeA, verticeB);
    }
    return mstGraph;
}

std::vector<Point> GeneratePoints(int countVertices) {
    std::vector<Point> points;
    points.reserve(countVertices);
    while (points.size() != countVertices){
        double s = 0;
        double x = dist(generator);
        double y = dist(generator);
        while (s == 0 || s > 1) {
            x = dist(generator);
            y = dist(generator);
            s = pow(x, 2) + pow(y, 2);
        }
        double z0 = x * sqrt(-2 * log(s) / s);
        double z1 = y * sqrt(-2 * log(s) / s);
        points.push_back({z0, z1});
    }
    return points;
}

double ApproximateTSPSolution(ArcGraph& arcGraph) {
    double result = 0;
    ArcGraph mstGraph = CreateMST(arcGraph.GetArcs(), arcGraph.ArcsCount());
    std::vector<bool> visited(mstGraph.VerticesCount(), false);
    std::vector<Point> dfsResult;
    dfsResult.reserve(mstGraph.VerticesCount());
    std::stack<Vertice> dfsS;
    Vertice vertice = mstGraph.GetArcs()[0].from;
	dfsS.push(vertice);
	while (!dfsS.empty()) {
		Vertice current = dfsS.top();
		dfsS.pop();
		if (visited[current.num]) {
            continue;
        }
        dfsResult.push_back(current.point);
		visited[current.num] = true;
		std::vector<Vertice> adjList = mstGraph.GetNextVertices(current.num);
		for (int i = 0; i < adjList.size(); ++i) {
			if (!visited[adjList[i].num]) {
				dfsS.push(adjList[i]);
			}
		}
	}
    for (int i = 1; i < dfsResult.size(); ++i) {
        result += sqrt(pow(dfsResult[i].x - dfsResult[i - 1].x, 2) + pow(dfsResult[i].y - dfsResult[i - 1].y, 2));
    }
    result += sqrt(pow(dfsResult[0].x - dfsResult[dfsResult.size() - 1].x, 2) + pow(dfsResult[0].y - dfsResult[dfsResult.size() - 1].y, 2));
    return result;
}

double ExactTSPSolution(std::vector<Point> &points) {
    double result = 1000;
    do {
        double currentResult = 0;
        for (int i = 1; i < points.size(); ++i) {
            currentResult += sqrt(pow(points[i].x - points[i - 1].x, 2) + pow(points[i].y - points[i - 1].y, 2));
        }
        currentResult += sqrt(pow(points[0].x - points[points.size() - 1].x, 2) + pow(points[0].y - points[points.size() - 1].y, 2));
        if (currentResult < result) {
            result = currentResult;
        }
    } while (std::next_permutation(points.begin(), points.end()));
    return result;
}

double Avarage(const std::vector<double> &vec) {
    double result = 0;
    for (auto element : vec) {
        result += element;
    }
    return result / vec.size();
}

double StandartDeviation(const std::vector<double> &vec, double avarage) {
    double result = 0;
    for (auto element : vec) {
        result += pow(element - avarage, 2);
    }
    result /= vec.size();
    return sqrt(result);
}

void Run(std::istream &input, std::ostream &output){
    for (int n = 2; n <= 10; ++n) {
        output << "n = " << n << std::endl;
        output << "approximateResult exactResult approximationFactor:" << std::endl;
        std::vector<double> vecApproximationFactor;
        vecApproximationFactor.reserve(10);
        for (int i = 0; i < 10; ++i) {
            std::vector<Point> points = GeneratePoints(n);
            int countArcs = n * (n - 1) * 0.5;
            ArcGraph arcGraph(countArcs, n);
            for (int i = 0; i < points.size(); ++i) {
                for (int j = i + 1; j < points.size(); ++j) {
                    Vertice verticeA{i + 1, points[i]};
                    Vertice verticeB{j + 1, points[j]};
                    arcGraph.AddEdge(verticeA, verticeB);
                }
            }
            double approximateResult = ApproximateTSPSolution(arcGraph);
            output << approximateResult << "\t";
            double exactResult = ExactTSPSolution(points);
            output << exactResult << "\t";
            double approximationFactor = approximateResult / exactResult;
            output << approximationFactor << std::endl;
            vecApproximationFactor.push_back(approximationFactor);
        }
        double avarage = Avarage(vecApproximationFactor);
        double standartDeviation = StandartDeviation(vecApproximationFactor, avarage);
        output << std::endl;
        output << "avarage standartDeviation:" << std::endl;
        output << avarage << " " << standartDeviation << std::endl;
        output << std::endl << std::endl;
    }
}

int main() {
    Run(std::cin, std::cout);
    return 0;
}
