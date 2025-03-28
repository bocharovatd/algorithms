#pragma once

#include <stack>

template<class T , class IsLess>
int FindPivot(T* arr, int n, IsLess& isLess);

template<class T , class IsLess>
int Partition(T* arr, int n, IsLess& isLess);

template<class T , class IsLess>
void QuickSort(T* arr, int n, IsLess& isLess);
