// 7_3. Binary MSD для long long.
// Дан массив неотрицательных целых 64-разрядных чисел. Количество чисел не больше 106. 
// Отсортировать массив методом MSD по битам (бинарный QuickSort).

#include <cassert>
#include <iostream>
#include <sstream>

void BinaryQuickSort(unsigned long long* arr, int n, unsigned long long pivotWithoutZeros, int shift) {
    if (n <= 0 || shift < 0) {
        return;
    }
    unsigned long long pivot = pivotWithoutZeros << shift;
    int i = 0;
    int j = n - 1;
    while (i <= j) {
        for (; (i < n) && (arr[i] < pivot); ++i) {}
        for (; (j >= 0) && (arr[j] >= pivot); --j) {}
        if (i < j) {
            std::swap(arr[i++], arr[j--]);
        }
    }
    --shift;
    BinaryQuickSort(arr, i, ((pivotWithoutZeros >> 1) << 2) + 1, shift);
    BinaryQuickSort(arr + i, n - i, (pivotWithoutZeros << 1) + 1, shift);
}

void Run(std::istream &input, std::ostream &output) {
    int n;
    input >> n;
    unsigned long long* arr = new unsigned long long[n];
    for (int i = 0; i < n; ++i) {
        input >> arr[i];
    }
    unsigned long long pivotWithoutZeros = 1;
    int shift = 63;
    BinaryQuickSort(arr, n, pivotWithoutZeros, shift);
    for (int i = 0; i < n; ++i) {
        output << arr[i] << " ";
    }
    delete[] arr;
}

void TestBinaryQuickSort() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "3 4 1000000 7";
        Run(input, output);
        assert(output.str() == "4 7 1000000 ");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "5 98738963687132698 143143 535354353 757 66";
        Run(input, output);
        assert(output.str() == "66 757 143143 535354353 98738963687132698 ");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "3 100 100 100";
        Run(input, output);
        assert(output.str() == "100 100 100 ");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "20 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1";
        Run(input, output);
        assert(output.str() == "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 ");
    }
}

int main() {
    Run(std::cin, std::cout);
    //TestBinaryQuickSort();
    return 0;
}
