// Задано N точек на плоскости. Указать (N-1)-звенную несамопересекающуюся 
// замкнутую ломаную, проходящую через все эти точки.
// Предполагается, что никакие три точки не лежат на одной прямой.
// Стройте ломаную от точки, имеющей наименьшую координату x.
// Если таких точек несколько, то используйте точку с наименьшей координатой y. 
// Точки на ломаной расположите в порядке убывания углов лучей от начальной 
// точки до всех остальных точек.
// Для сортировки точек реализуйте алгоритм сортировки вставками.

#include <iostream>

struct Point {
    int x;
    int y;
};

template<class T>
class IsLessPoints {
public:
    bool operator()(const T& a, const T& b) {
        if ((float(a.x) / float(a.y)) == (float(b.x) / float(b.y))) {
            if (float(a.x) == float(b.x)) {
                return float(a.y) <  float(b.y);
            }
            return float(a.x) < float(b.x);
        }
        return ((float(a.x) / float(a.y)) < (float(b.x) / float(b.y)));
    }
}; // Сравнение для элементов типа  Element

template<class T , class IsLess>
void InsertionSort(T *a, int n, IsLess isLess)
{
    for (int i = 1; i < n; ++i)
    {
        T tmp = a[i];
        int j = i - 1;
        for (; j >= 0 && isLess(tmp, a[j]); --j)
        {
            a[j + 1] = a[j];
        }
        a[j + 1] = tmp;
    }
}

int main() {
    int n;
    std::cin >> n;
    Point* arr = new Point[n];
    for (int i = 0; i < n; ++i) {
        Point point;
        std::cin >> point.x;
        std::cin >> point.y;
        arr[i] = point;
    }
    IsLessPoints<Point> isLess;
    InsertionSort(arr, n, isLess);
    for (int i = 0; i < n; ++i) {
        std::cout << arr[i].x << " " << arr[i].y << std::endl;
    }
    return 0;
}