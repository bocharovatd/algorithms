// Решение всех задач данного раздела предполагает использование кучи, 
// реализованной в виде шаблонного класса. 
// Решение должно поддерживать передачу функции сравнения снаружи.
// Куча должна быть динамической.

// Напишите программу, которая использует кучу для слияния K отсортированных массивов суммарной длиной N.
// Требования: время работы O(N * logK). Ограничение на размер кучи O(K)..
// Формат входных данных: Сначала вводится количество массивов K. 
//Затем по очереди размер каждого массива и элементы массива. 
//Каждый массив упорядочен по возрастанию.
// Формат выходных данных: Итоговый отсортированный массив.

#include <cassert>
#include <iostream>
#include <sstream>

struct Element {
	int Value;
	int ArrayIndex;
	int IndexInArray;
};

int Log2(int n) {
    int result = 0;
    while (n >>= 1) {
        ++result;
    }
    return result;
} // Логарифм по основанию 2

int Pow2(int n) {
    return (static_cast<int>(1) << n);
} // Возведение 2 в степень n

template<class T>
class IsLessDefault {
public:
    bool operator()(const T& a, const T& b) {
        return a < b;
    }
}; // Дефолтное сравнение

class IsLessElement {
public:
    bool operator()(const Element& a, const Element& b)
    {
        return a.Value < b.Value;
    }
}; // Сравнение для элементов типа  Element

template<class T = int, class IsLess = IsLessDefault<T>>
class Heap {
public:
    Heap(int n = 0, IsLess isLess = IsLess()) : 
        isLess_(isLess), 
        bufferSize_(0), 
        size_(Pow2(Log2(n) + 1) - 1),
        buffer_((size_ == 0) ? new T[1] : new T[size_])
         {}  // Конструктор по умолчанию

    Heap(T* arr, int arrSize, IsLess isLess = IsLess()) {
        isLess_ = isLess;
        bufferSize_ = arrSize;
        size_ = Pow2(Log2(arrSize) + 1) - 1;
        buffer_ = (size_ == 0) ? new T[1] : new T[size_];
        for (int i = 0; i < bufferSize_; ++i) {
            buffer_[i] = arr[i];
        }
        buildHeap();
    } // Конструктор копирования массива

    ~Heap() {
        delete[] buffer_;
    } // Деструктор

	void Insert(const T& element) {
        if (size_ > bufferSize_) {
            ++bufferSize_;
        }
        else {
            grow();
        }
        buffer_[bufferSize_ - 1] = element;
        siftUp(bufferSize_ - 1);
    } // Добавить элемент в кучу

	T ExtractMax() {
        std::swap(buffer_[0], buffer_[bufferSize_ - 1]);
        --bufferSize_;
        siftDown(0);
        return buffer_[bufferSize_];
    } // Извлечь максимум из кучи

    void ChangeMaxAndSiftDown(const T& element) {
        buffer_[0] = element;
        siftDown(0);
    }

	const T& PeekMax() const {
        return buffer_[0];
    } // Посмотреть значение максимума в куче
    
	int Size() const {
        return size_;
    } // Посмотреть размер

private:
	IsLess isLess_;
	T* buffer_;
	int bufferSize_;
	int size_;

    void buildHeap() {
        for (int i = size_ / 2 - 1; i >= 0; --i) {
            siftDown(i);
        }
    } // Создание кучи по существующему буферу

    void siftDown(int const i) {
        int iSift = i;
        while (iSift < bufferSize_ / 2) {
            int leftDown = 2 * iSift + 1;
            int rightDown = 2 * iSift + 2;
            int largest = iSift;
            if (leftDown < bufferSize_ && isLess_(buffer_[iSift], buffer_[leftDown])) {
                largest = leftDown;
            }
            if (rightDown < bufferSize_ && isLess_(buffer_[largest], buffer_[rightDown])){
                largest = rightDown;
            }
            if (largest != iSift) {
                std::swap(buffer_[iSift], buffer_[largest]);
                iSift = largest;
            }
            else {
                break;
            }
        }
    } // Просеить элемент кучи вниз

    void siftUp(int const i) {
        int iSift = i;
        while (iSift > 0) {
            int up = iSift / 2 - 1 + iSift % 2;
            if (isLess_(buffer_[up], buffer_[iSift])) {
                std::swap(buffer_[iSift], buffer_[up]);
                iSift = up;
            }
            else {
                break;
            }
        }
    } // Просеить элемента кучи наверх

	void grow() {
        size_ = size_ * 2 + 1;
        T* bufferNew = new T[size_];
        for (int i = 0; i < bufferSize_; ++i) {
            bufferNew[i] = buffer_[i];
        }
        delete[] buffer_;
        buffer_ = bufferNew;
    } // Увеличить буфер кучи в 2 раза
};

void Merge(Element** arrElements, int* arrSizes, const int& k, const int& n, int* arrAnswer) {
    Element* arrHeap = new Element[k];
    for (int i = 0; i < k; ++i) {
        arrHeap[i] = arrElements[i][0];
    }
    Heap heapElement(arrHeap, k, IsLessElement());
    for (int i = 0; i < n; ++i) {
        Element elementMaxNow = heapElement.PeekMax();
        // // Заполняем arrAnswer в порядке убывания
        arrAnswer[(n - 1) - i] = elementMaxNow.Value;
        if (elementMaxNow.IndexInArray + 1 < arrSizes[elementMaxNow.ArrayIndex]) {
            heapElement.ChangeMaxAndSiftDown(arrElements[elementMaxNow.ArrayIndex][elementMaxNow.IndexInArray + 1]);
        }
        else {
            heapElement.ExtractMax();
        }
    }
    delete[] arrHeap;
} // Слияние в отсортированный массив arrAnswer из arrElements

void Run(std::istream &input, std::ostream &output) {
    int k; // Количество подмассивов
    int n = 0; // Общее количество элементов
    input >> k;
    int* arrSizes = new int[k]; // Массив размеров подмассивов
    Element** arrElements = new Element* [k]; // Массив подмассивов
    for (int i = 0; i < k; ++i) {
        input >> arrSizes[i];
        n += arrSizes[i]; 
        arrElements[i] = new Element[arrSizes[i]];
        for (int j = 0; j < arrSizes[i]; ++j) {
            // Заполняем подмассивы в порядке убывания
            input >>  arrElements[i][(arrSizes[i] - 1) - j].Value;
            arrElements[i][(arrSizes[i] - 1) - j].ArrayIndex = i;
            arrElements[i][(arrSizes[i] - 1) - j].IndexInArray = (arrSizes[i] - 1) - j;
        }
    }
    int* arrAnswer = new int[n];
    Merge(arrElements, arrSizes, k, n, arrAnswer);
    for (int i = 0; i < n; ++i) {
        output << arrAnswer[i] << " ";
    }
    delete[] arrSizes;
    for (int i = 0; i < k; ++i) {
        delete[] arrElements[i];
    }
    delete[] arrElements;
    delete[] arrAnswer;
}

void TestMerge() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "3 1 6 2 50 90 3 1 10 70";
        Run(input, output);
        assert(output.str() == "1 6 10 50 70 90 ");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "3 1 3 4 3 3 3 3 2 3 3";
        Run(input, output);
        assert(output.str() == "3 3 3 3 3 3 3 ");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "1 3 1 2 3";
        Run(input, output);
        assert(output.str() == "1 2 3 ");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "3 4 1 2 10 12 3 2 15 90 3 0 20 100";
        Run(input, output);
        assert(output.str() == "0 1 2 2 10 12 15 20 90 100 ");
    }
}

int main() {
    Run(std::cin, std::cout);
    //TestMerge();
    return 0;
}