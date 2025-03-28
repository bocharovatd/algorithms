// Общие требования для всех задач
// Ввод/вывод отделены от решения.
// Не должно быть утечек памяти.
// Задача 5. Минимальное остовное дерево
// Дан неориентированный связный граф. Требуется найти вес минимального остовного дерева в этом графе.
// С помощью алгоритма Крускала.
// Ваш номер варианта прописан в ведомости.
// Формат ввода
// Первая строка содержит два натуральных числа n и m — количество вершин и 
// ребер графа соответственно (1 ≤ n ≤ 20000, 0 ≤ m ≤ 100000).
// Следующие m строк содержат описание ребер по одному на строке.
// Ребро номер i описывается тремя натуральными числами 
// bi, ei и wi — номера концов ребра и его вес соответственно (1 ≤ bi, ei ≤ n, 0 ≤ wi ≤ 100000).
// Формат вывода
// Выведите единственное целое число - вес минимального остовного дерева.

#include <cassert>
#include <iostream>
#include <stack>
#include <sstream>
#include <vector>

struct arc {
    int from;
    int to;
    int weight;
};

class ArcGraph {
public:
    ArcGraph(int arcsCount_);
    ArcGraph(const ArcGraph &graph);
    ~ArcGraph();
    void AddEdge(int from, int to, int weight);
    int ArcsCount() const;
    int ArcsWeight() const;
    arc* GetArcs ();

private:
    arc* arcs;
    int arcsCount;
    int arcsWeight;
    int currentEdge;
};

ArcGraph::ArcGraph(int arcsCount_) {
    assert(arcsCount_ > 0);
    arcsCount = arcsCount_;
    arcs = new arc[arcsCount_];
    currentEdge = 0;
    arcsWeight = 0;
}

ArcGraph::ArcGraph(const ArcGraph &graph) {
    arcsCount = graph.arcsCount;
    arcs = new arc[arcsCount];
    currentEdge = 0;
    for (currentEdge < arcsCount; ++currentEdge;) {
        arcs[currentEdge] = graph.arcs[currentEdge];
    }
    arcsWeight = graph.arcsWeight;
}

ArcGraph::~ArcGraph() {
    delete[] arcs;
}

void ArcGraph::AddEdge(int from, int to, int weight) {
    arc newArc{from, to, weight};
    arcs[currentEdge++] = newArc;
    arcsWeight += weight;
}

int ArcGraph::ArcsCount() const {
    return arcsCount;
}

int ArcGraph::ArcsWeight() const {
    return arcsWeight;
}

arc* ArcGraph::GetArcs () {
    return arcs;
}

class DSU {
public:
    DSU(int count);
    int FindParent(int v);
    void Union(int vi, int v2);

private:
    std::vector<int> parent;
    std::vector<int> rank;
};

DSU::DSU(int count) : parent(count + 1), rank(count + 1, 0) {
    for (int i = 0; i <= count; ++i) {
        parent[i] = i;
    }
}

int DSU::FindParent(int v) {
    if (v == parent[v]) {
        return v;
    }
    parent[v] = FindParent(parent[v]);
    return parent[v];
}

void DSU::Union(int v1, int v2) {
    int p1 = FindParent(v1);
    int p2 = FindParent(v2);
    if (p1 == p2) {
        return;
    }
    if (rank[p1] < rank[p2]) {
        parent[p1] = p2;
    }
    else {
        parent[p2] = p1;
        if (rank[p1] == rank[p2]){
            ++rank[p1];
        }
    }
}

template<class T , class IsLess>
int FindPivot(T* arr, int n, IsLess& isLess) {
    int a = 0;
    int b = n / 2;
    int c = n - 1;
    if (isLess(arr[a], arr[b])) {
        if (isLess(arr[b], arr[c])) {
            return b;
        }
        return isLess(arr[c], arr[a]) ? a : c;
    }
    if (isLess(arr[a], arr[c])) {
        return a;
    }
    return isLess(arr[c], arr[b]) ? b : c;
} // Поиск индекса медианы трех

template<class T , class IsLess>
int Partition(T* arr, int n, IsLess& isLess) {
    std::swap(arr[FindPivot(arr, n, isLess)], arr[n - 1]);
    T pivot = arr[n - 1];
	int i = n - 2;
	int j = n - 2;
    // в промежутке (j, i] все элементы меньше или равны pivot
    // в промежутке (i, n - 1) все элементы больше pivot
	while (j >= 0) {
        if (isLess(pivot, arr[j])) {
            std::swap(arr[i], arr[j]);
            --i;
        }
        --j;
    }
    std::swap(arr[i + 1], arr[n - 1]);
	return i + 1;
} // Поиск индекса, на который встанет пивот после разделения

struct IsLessArcs {
    bool operator()(const arc& arcA, const arc& arcB) {
        return arcA.weight < arcB.weight;
    }
};

template<class T , class IsLess>
void QuickSort(T* arr, int n, IsLess& isLess) {
    std::stack<std::pair<int, int>> callStack;
    int left = 0;
    int right = n - 1;
    callStack.push(std::make_pair(left, right));
    while (!callStack.empty()) {
        left = callStack.top().first;
        right = callStack.top().second;
        callStack.pop();
        if (right - left + 1 <= 2) {
            if (right - left + 1 == 2 && isLess(arr[left + 1], arr[left])) {
                std::swap(arr[left + 1], arr[left]);
            }
            continue;
        }
        int pivotPos = left + Partition(arr + left, right - left + 1, isLess);
        if (pivotPos > left) {
            callStack.push(std::make_pair(left, pivotPos - 1));
        }
        if (pivotPos < right) {
            callStack.push(std::make_pair(pivotPos, right));
        }
    }
}

void SortArcs(arc* arcs, int n) {
    IsLessArcs isLessArcs;
    QuickSort(arcs, n, isLessArcs);
}

ArcGraph CreateMST(arc* arcs, int n) {
    SortArcs(arcs, n);
    DSU dsu(n);
    std::vector<arc> mstArcs;
    for (int i = 0; i < n; ++i) {
        int v1 = arcs[i].from;
        int v2 = arcs[i].to;
        int weight = arcs[i].weight;
        if (dsu.FindParent(v1) != dsu.FindParent(v2)) {
            mstArcs.push_back({v1, v2, weight});
            dsu.Union(v1, v2);
        }
    }
    ArcGraph mstGraph(mstArcs.size());
    for (int i = 0; i < mstArcs.size(); ++i) {
        int vertexA = mstArcs[i].from;
        int vertexB = mstArcs[i].to;
        int weight = mstArcs[i].weight;
        mstGraph.AddEdge(vertexA, vertexB, weight);
    }
    return mstGraph;
}

void Run(std::istream &input, std::ostream &output){
    int n = 0;
    int m = 0;
    input >> n >> m;
    ArcGraph arcGraph(m);
    for (int i = 0; i < m; ++i) {
        int vertexA = 0;
        int vertexB = 0;
        int weight = 0;
        input >> vertexA >> vertexB >> weight;
        arcGraph.AddEdge(vertexA, vertexB, weight);
    }
    ArcGraph mstGraph = CreateMST(arcGraph.GetArcs(), arcGraph.ArcsCount());
    output << mstGraph.ArcsWeight();
}

void TestCreateMST() {
        {
            std::stringstream input;
            std::stringstream output;
            input << "4 4 1 2 1 2 3 2 3 4 5 4 1 4";
            Run(input, output);
            assert(output.str() == "7");
        }
        {
            std::stringstream input;
            std::stringstream output;
            input << "5 10 4 3 3046 4 5 90110 5 1 57786 3 2 28280 4 3 18010 4 5 61367 4 1 18811 4 2 69898 3 5 72518 3 1 85838";
            Run(input, output);
            assert(output.str() == "107923");
        }
}

int main() {
    TestCreateMST();
    //Run(std::cin, std::cout);
    return 0;
}