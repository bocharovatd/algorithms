#include "Sort.hpp"

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
