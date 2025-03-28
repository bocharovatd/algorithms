// Во всех задачах данного раздела необходимо реализовать и 
// использовать сортировку слиянием в виде шаблонной функции. 
// Решение должно поддерживать передачу функции сравнения снаружи.
// Общее время работы алгоритма O(n log n).

// Группа людей называется современниками если был такой момент, когда они могли собраться вместе. 
// Для этого в этот момент каждому из них должно было  уже исполниться 18 лет, но ещё не исполниться 80 лет.
// Дан список Жизни Великих Людей. Необходимо получить максимальное количество современников. 
// В день 18летия человек уже может принимать участие в собраниях, 
// а в день 80летия и в день смерти уже не может.

// Замечание. Человек мог не дожить до 18-летия, либо умереть в день 18-летия. 
// В этих случаях принимать участие в собраниях он не мог.

#include <cassert>
#include <iostream>
#include <sstream>

struct Date {
    int day;
    int month;
    int year;
};

struct Element {
    Date date;
    int delta;
};

class IsLessDate {
public:
    bool operator()(const Date& a, const Date& b) {
        if (a.year < b.year) {
            return true;
        }
        else if (a.year == b.year) {
            if (a.month < b.month) {
                return true;
            }
            else if (a.month == b.month) {
                if (a.day < b.day) {
                    return true;
                }
            }
        }
        return false;
    } 
};

class IsLessElement {
public:
    bool operator()(const Element& a, const Element& b) {
        if (a.date.year < b.date.year) {
            return true;
        }
        else if (a.date.year == b.date.year) {
            if (a.date.month < b.date.month) {
                return true;
            }
            else if (a.date.month == b.date.month) {
                if (a.date.day < b.date.day) {
                    return true;
                }
            }
        }
        return false;
    } 
};

template<class T>
class IsLessDefault {
public:
    bool operator()(const T& a, const T& b) {
        return a < b;
    }
}; // Сравнение для элементов типа  Element

template<class T , class IsLess = IsLessDefault<T>>
void Merge(T* arr, const int& aIndex, const int& bIndex, const int& sizeOfMergedParts, IsLess isLess = IsLess()) {
    T* arrTemp = new T[sizeOfMergedParts];
    for (int iStep = 0; iStep < (bIndex - aIndex + 1); iStep += sizeOfMergedParts) {
        int iLeft = 0;
        int iRight = 0;
        int iMax = (bIndex - aIndex + 1) - iStep >= sizeOfMergedParts ? sizeOfMergedParts : (bIndex - aIndex + 1) - iStep;
        // Размер arrTemp
        // Для проверки последнего куска, который может быть размером меньше sizeOfMergedParts
        for (int i = 0; i < iMax ; ++i) {
            if (iLeft >= (sizeOfMergedParts / 2)) {
                arrTemp[i] = arr[aIndex + iStep + sizeOfMergedParts / 2 + iRight];
                ++iRight;
            } // Элементы в левой части закончились
            else if (iRight >= (iMax - sizeOfMergedParts / 2))
            {
                arrTemp[i] = arr[aIndex + iStep + iLeft];
                ++iLeft;
            } // Элементы в правой части закончились
            else {
                if (isLess(arr[aIndex + iStep + sizeOfMergedParts / 2 + iRight], arr[aIndex + iStep + iLeft]))
                {
                    arrTemp[i] = arr[aIndex + iStep + sizeOfMergedParts / 2 + iRight];
                    ++iRight;
                }
                else
                {
                    arrTemp[i] = arr[aIndex + iStep + iLeft];
                    ++iLeft;
                }
            }
        }
        for (int i = 0; i < iMax; ++i) {
            arr[aIndex + iStep + i] = arrTemp[i];
        }
    }
    delete[] arrTemp;
} // Объединение всех частей массива размером sizeOfMergedParts

template<class T , class IsLess = IsLessDefault<T>>
void MergeSort(T* arr, const int& aIndex, const int& bIndex, IsLess isLess = IsLess()) {
    for (int i = aIndex + 1; i <= bIndex; i += 2) {
        if (isLess(arr[aIndex + i], arr[aIndex + i - 1])) {
            std::swap(arr[aIndex + i - 1], arr[aIndex + i]);
        }
    } // Первая сортировка массива парами чисел
    int sizeOfMergedParts = 4;
    while (sizeOfMergedParts < (bIndex - aIndex + 1)) {
        Merge(arr, aIndex, bIndex, sizeOfMergedParts, isLess);
        sizeOfMergedParts *= 2;
    } // Поэтапное объединение отсортированных частей массива
    Merge(arr, aIndex, bIndex, sizeOfMergedParts, isLess); 
    // Последнее объединение двух оставшихся отсортиванных частей массива
}

void Run(std::istream &input, std::ostream &output) {
    int n;
    input >> n;
    Element* arr = new Element[n * 2];
    int iArr = 0;
    IsLessDate isLessDate;
    for (int i = 0; i < n; ++i) {
        Date dateBirth;
        Date dateDeath;
        input >> dateBirth.day >> dateBirth.month >> dateBirth.year;
        input >> dateDeath.day >> dateDeath.month >> dateDeath.year;
        Date dateStart = dateBirth;
        dateStart.year += 18;
        Date dateEnd = dateBirth;
        dateEnd.year += 80;
        if (isLessDate(dateDeath, dateEnd)) {
            dateEnd = dateDeath;
        }
        if (isLessDate(dateStart, dateEnd)) {
            arr[iArr].date = dateStart;
            arr[iArr].delta = 1;
            ++iArr;
            arr[iArr].date = dateEnd;
            arr[iArr].delta = -1;
            ++iArr;
        }
    }
    int arrSize = iArr;
    IsLessElement isLessElement;
    MergeSort(arr, 0, arrSize - 1, isLessElement);
    int count = 0;
    int answer = 0;
    for (int i = 0; i < arrSize; ++i) {
        count += arr[i].delta;
        if (count > answer) {
            answer = count;
        }
    }
    output << answer;
    delete[] arr;
}

void TestContemproraries() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "3 2 5 1980 13 11 2055 1 1 1982 1 1 2030 2 1 1920 2 1 2000";
        Run(input, output);
        assert(output.str() == "3");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "3 2 5 1980 13 11 2055 1 1 1782 1 1 1790 2 1 2060 2 1 2160";
        Run(input, output);
        assert(output.str() == "1");
    }
}

int main() {
    Run(std::cin, std::cout);
    //TestContemproraries();
    return 0;
}