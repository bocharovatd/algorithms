// 3. Who let the Rick out?
// Рик и Морти снова бороздят просторы вселенных, 
// но решили ограничиться только теми, номера которых меньше M. 
// Они могут телепортироваться из вселенной с номером z во вселенную (z+1) mod M за a бутылок лимонада 
// или во вселенную (z2+1) mod M за b бутылок лимонада. 
// Рик и Морти хотят добраться из вселенной с номером x во вселенную с номером y. 
// Сколько бутылок лимонада отдаст Рик за такое путешествие, если он хочет потратить их как можно меньше?
// Формат ввода
// В строке подряд даны количество бутылок a за первый тип телепортации, 
// количество бутылок b за второй тип телепортации, количество вселенных M, 
// номер стартовой вселенной x, номер конечной вселенной y (0 ≤ a, b ≤ 100, 1 ≤ M ≤ 106, 0 ≤ x, y < M).
// Формат вывода
// Выведите одно число — количество бутылок лимонада, которые отдаст Рик за такое путешествие.

#include <cassert>
#include <cmath>
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>

std::vector<unsigned long> GetNextVertices(int vertex);

int GetWeight(unsigned long to, unsigned long from, unsigned long m, int a, int b) {
    if ((from + 1) % m == (from * from + 1) % m) {
        return std::min(a, b);
    }
    if ((from + 1) % m == to) {
        return a;
    }
    if ((from * from + 1) % m == to) {
        return b;
    }
}  

std::vector<unsigned long> GetNextVertices(unsigned long vertex, unsigned long m) {
    std::vector<unsigned long> next;
    next.reserve(2);
    next.push_back((vertex + 1) % m);
    if ((vertex + 1) % m != (vertex * vertex + 1) % m) {
        next.push_back((vertex * vertex + 1) % m);
    }
    return next;
}

int lengthShortestWay(unsigned long from, unsigned long to, unsigned long m, int a, int b) {
    std::map<int, int> dist;
    dist[from] = 0;
    std::set<std::pair<int, unsigned int>> priorityQueue;
	priorityQueue.insert(std::make_pair(dist[from], from));
	while (!priorityQueue.empty()) {
		unsigned long current = priorityQueue.begin()->second;
        if (current == to) {
            return dist[to];
        }
		priorityQueue.erase(priorityQueue.begin());
		std::vector<unsigned long> adjList = GetNextVertices(current, m);
		for (int i = 0; i < adjList.size(); ++i) {
            if (dist.find(adjList[i]) == dist.end()) {
                dist[adjList[i]] = dist[current] + GetWeight(adjList[i], current, m, a, b);
                priorityQueue.insert(std::make_pair(dist[adjList[i]], adjList[i]));
            }
            else if (dist[adjList[i]] > dist[current] + GetWeight(adjList[i], current, m, a, b)) {
                priorityQueue.erase(std::make_pair(dist[adjList[i]], adjList[i]));
                dist[adjList[i]] = dist[current] + GetWeight(adjList[i], current, m, a, b);
                priorityQueue.insert(std::make_pair(dist[adjList[i]], adjList[i]));
            }
        }
    }
}

void Run(std::istream &input, std::ostream &output) {
    int a = 0;
    int b = 0;
    unsigned long m = 0;
    unsigned long x = 0;
    unsigned long y = 0;
    input >> a >> b >> m >> x >> y;
    output << lengthShortestWay(x, y, m, a, b);
}

void TestLengthShortestWay() {
        {
            std::stringstream input;
            std::stringstream output;
            input << "3 14 15 9 9";
            Run(input, output);
            assert(output.str() == "0");
        }
        {
            std::stringstream input;
            std::stringstream output;
            input << "6 1 5 2 3";
            Run(input, output);
            assert(output.str() == "6");
        }
        {
            std::stringstream input;
            std::stringstream output;
            input << "6 1 5 2 1";
            Run(input, output);
            assert(output.str() == "2");
        }
}

int main() {
    //TestLengthShortestWay();
    Run(std::cin, std::cout);
    return 0;
}