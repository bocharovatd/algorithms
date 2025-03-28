// Дан отсортированный массив целых чисел A[0..n - 1] и массив целых чисел B[0..m - 1].
// Для каждого элемента массива B[i] найдите минимальный индекс k минимального элемента массива A, 
// равного или превосходящего B[i] : A[k] >= B[i].Если такого элемента нет, выведите n.
// n, m ≤ 10000. 

// Требования : Время работы поиска k для каждого элемента B[i] : O(log(k)).
// Внимание !В этой задаче для каждого B[i] сначала нужно определить диапазон для бинарного поиска 
// размером порядка k с помощью экспоненциального поиска, 
// а потом уже в нем делать бинарный поиск.

#include <cassert>
#include <iostream>
#include <sstream>

int find_range(const int *arr_a, const int &n, const int &find) {
    int i_border = 1;
    while (i_border < n) {
        if (find <= arr_a[i_border]) {
            return i_border;
        }
        i_border *= 2;
    }
    return i_border;
}

int found_or_next_index(const int *arr_a, const int &n, const int &find) {
    int found_border = find_range(arr_a, n, find);
    int begin = (found_border == 1) ? 0 : (found_border / 2 + 1);
    int end = (found_border < n) ? found_border : n;
    int middle = 0;
    while (begin < end) {
        middle = (begin + end) / 2;
        if (find > arr_a[middle]) {
            begin = middle + 1;
        }
        else {
            end = middle;
        }
    }
    return begin;
}

void run(std::istream &input, std::ostream &output) {
    int n = 0;
    int m = 0;
    input >> n >> m;
    int *arr_a = new int[n];
    int *arr_b = new int[m];
    for (int i = 0; i < n; ++i) {
        input >> arr_a[i];
    }
    for (int i = 0; i < m; ++i) {
        input >> arr_b[i];
    }
    for (int i = 0; i < m; ++i) {
        output << found_or_next_index(arr_a, n, arr_b[i]) << " ";
    }
    delete[] arr_a;
    delete[] arr_b;
}

void test_bin_search() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "2 1 1 2 2";
        run(input, output);
        assert(output.str() == "1 ");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "4 3 2 4 5 7 4 6 1";
        run(input, output);
        assert(output.str() == "1 3 0 ");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "3 2 2 3 4 1 5";
        run(input, output);
        assert(output.str() == "0 3 ");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "6 2 1 2 2 3 3 4 2 3";
        run(input, output);
        assert(output.str() == "1 3 ");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "3 1 2 2 2 2";
        run(input, output);
        assert(output.str() == "0 ");
    }
}

int main() {
    //run(std::cin, std::cout);
    test_bin_search();
    return 0;
}