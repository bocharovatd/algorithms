// Дан массив, который получен путем нескольких циклических 
//сдвигов исходного отсортированного массива. 
//Нужно найти индекс элемента, с которого начинался исходный массив.

// Примеры:

// [12, 14, 16, 17, 1, 3, 5, 7] (исходный массив [1, 3, 5, 7, 12, 14, 16, 17])

// Ответ: 4

// [5, 5, 5, 1] (исходный массив [1, 5, 5, 5])

// Ответ: 3

// Требование: сложность O(logN)

#include <iostream>

int foundRealBegin(const int *arr, const int &n) {
    int begin = 0;
    int end = n - 1;
    int middle = 0;
    while (begin < end) {
        middle = (begin + end) / 2;
        if (arr[middle] > arr[end]) {
            begin = middle + 1;
        }
        else {
            end = middle;
        }
    }
    return begin;
}

int main() {
    int n;
    std::cin >> n;
    int* arr = new int[n];
    for (int i = 0; i < n; ++i) {
        std:: cin >> arr[i];
    }
    std::cout << foundRealBegin(arr, n);
}