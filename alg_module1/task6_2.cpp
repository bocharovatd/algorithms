// Дано множество целых чисел из [0..10^9] размера n. 
// Используя алгоритм поиска k-ой порядковой статистики, 
// требуется найти следующие параметры множества:
// 1) 10%  перцентиль
// 2) медиана
// 3) 90%  перцентиль

// Требования: к дополнительной памяти: O(n). 
// Среднее время работы: O(n)
// Должна быть отдельно выделенная функция partition. 
// Рекурсия запрещена. 
// Решение должно поддерживать передачу функции сравнения снаружи.


// Функцию Partition следует реализовывать методом прохода двумя итераторами в одном направлении. 

// 6_2. Реализуйте стратегию выбора опорного элемента “медиана трёх”. 
// Функцию Partition реализуйте методом прохода двумя итераторами от конца массива к началу.

#include <cassert>
#include <iostream>
#include <sstream>

template<class T>
class IsLessDefault {
public:
    bool operator()(const T& a, const T& b) {
        return a < b;
    }
}; // Сравнение по умолчанию

template<class T , class IsLess = IsLessDefault<T>>
int FindPivot(T* arr, int n, IsLess& isLess = IsLess()) {
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

template<class T , class IsLess = IsLessDefault<T>>
int Partition(T* arr, int n, IsLess& isLess = IsLess()) {
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

template<class T , class IsLess = IsLessDefault<T>>
int FindKStat(T* arr, int n, int k, IsLess& isLess = IsLess()) {
    assert(k >= 0 && k < n);
    int a = 0;
    int b = n - 1;
    int pivotPos = -1;
    while (true) {
        if (b - a + 1 <= 2) {
            if (b - a + 1 == 2 && isLess(arr[a + 1], arr[a])) {
                std::swap(arr[a + 1], arr[a]);
            }
            return arr[k];
        }
        pivotPos = a + Partition(arr + a, b - a + 1, isLess);
        if (pivotPos == k) {
            return arr[k];
        }
        if (pivotPos > k) {
            b = pivotPos - 1;
        }
        else {
            a = pivotPos;
        }
    }
} // Поиск k-ой статистики


void Run(std::istream& input, std::ostream& output) {
    int n;
    input >> n;
    unsigned int* arr = new unsigned int[n];
    for (int i = 0; i < n; ++i) {
        input >> arr[i];
    }
    IsLessDefault<unsigned int> isLess;
    output << FindKStat(arr, n, n / 10  , isLess) << std::endl;
    output << FindKStat(arr, n, n / 2 , isLess) << std::endl;
    output << FindKStat(arr, n, n * 9 / 10, isLess) << std::endl;
    delete[] arr;
}

void TestFindKStat() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "10 1 2 3 4 5 6 7 8 9 10 ";
        Run(input, output);
        assert(output.str() == "2\n6\n10\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "10 10 2 1 5 3 8 7 9 4 6";
        Run(input, output);
        assert(output.str() == "2\n6\n10\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "10 10 9 8 7 6 5 4 3 2 1";
        Run(input, output);
        assert(output.str() == "2\n6\n10\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "10 1 1 1 1 1 1 1 1 1 1";
        Run(input, output);
        assert(output.str() == "1\n1\n1\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "20 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1";
        Run(input, output);
        assert(output.str() == "3\n11\n19\n");
    }
}

int main() {
    //Run(std::cin, std::cout);
    TestFindKStat();
    return 0;
}