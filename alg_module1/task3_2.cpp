// Во всех задачах из следующего списка следует написать структуру данных,
// обрабатывающую команды push* и pop*.

// Реализовать дек с динамическим зацикленным буфером (на основе динамического массива).
// Требования: Дек должен быть реализован в виде класса.

// Команды:
// a = 1 - push front
// a = 2 - pop front
// a = 3 - push back
// a = 4 - pop back

// Если дана команда pop*, то число b - ожидаемое значение.
// Если команда pop вызвана для пустой структуры данных, то ожидается “-1”.

// Требуется напечатать YES - если все ожидаемые значения совпали.
// Иначе, если хотя бы одно ожидание не оправдалось, то напечатать NO.

#include <cassert>
#include <iostream>
#include <sstream>

int rem_of_div(const int &a, const int &b) {
    return (b + (a % b)) % b;
} // переопределение операции %
  //  для получения положительного остатка от деления отрицательных чисел

template <class T>
class deque {
private:
    T *arr;
    int capacity_;
    int size_;
    int begin_;
    int end_;

public:
    deque() : capacity_(0), size_(0), begin_(1), end_(0), arr(new T[1]) {} // конструктор по умолчанию

    deque(deque<T> &deque_argument) {
        capacity_ = deque_argument.capacity_;
        size_ = deque_argument.size_;
        begin_ = deque_argument.begin_;
        end_ = deque_argument.end_;
        arr = new T[capacity_];
        for (int i = begin_; i <= end_; ++i) {
            arr[rem_of_div(i, capacity_)] = deque_argument.arr[rem_of_div(i, capacity_)];
        }
    } // конструктор копирования

    ~deque() { delete[] arr; } // деструктор

    deque &operator=(deque<T> &deque_argument) {
        if (this != &deque_argument) {
            delete[] arr;
            capacity_ = deque_argument.capacity_;
            size_ = deque_argument.size_;
            begin_ = deque_argument.begin_;
            end_ = deque_argument.end_;
            arr = new T[capacity_];
            for (int i = begin_; i <= end_; ++i) {
                arr[rem_of_div(i, capacity_)] = deque_argument.arr[rem_of_div(i, capacity_)];
            }
        }
        return *this;
    } // оператор присваивания (копирования)

    int size() const {
        return size_;
    } // возвращает количество хранимых элементов

    void push_front(const T &element) {
        if (capacity_ > size_) {
            ++size_;
            --begin_;
            arr[rem_of_div(begin_, capacity_)] = element;
        } // не нужно увеличивать буфер
        else {
            int new_capacity_ = (size_ == 0) ? 1 : size_ * 2;
            T *arr_temp = new T[new_capacity_];
            for (int i = begin_; i <= end_; ++i) {
                arr_temp[rem_of_div(i, new_capacity_)] = arr[rem_of_div(i, capacity_)];
            }
            ++size_;
            --begin_;
            arr_temp[rem_of_div(begin_, new_capacity_)] = element;
            delete[] arr;
            arr = arr_temp;
            capacity_ = new_capacity_;
        }
    } // вставка элемента в начало

    T pop_front() {
        --size_;
        ++begin_;
        return arr[rem_of_div(begin_ - 1, capacity_)];
    } // удаление элемента из начала

    void push_back(const T &element) {
        if (capacity_ > size_) {
            ++size_;
            ++end_;
            arr[rem_of_div(end_, capacity_)] = element;
        } // не нужно увеличивать буфер
        else {
            int new_capacity_ = (size_ == 0) ? 1 : size_ * 2;
            T *arr_temp = new T[new_capacity_];
            for (int i = begin_; i <= end_; ++i) {
                arr_temp[rem_of_div(i, new_capacity_)] = arr[rem_of_div(i, capacity_)];
            }
            ++size_;
            ++end_;
            arr_temp[rem_of_div(end_, new_capacity_)] = element;
            delete[] arr;
            arr = arr_temp;
            capacity_ = new_capacity_;
        }
    } // вставка элемента в конец

    T pop_back() {
        --size_;
        --end_;
        return arr[rem_of_div(end_ + 1, capacity_)];
    } // удаление элемента из конца
};

void run(std::istream &input, std::ostream &output) {
    deque<int> int_deque;
    int n = 0;
    input >> n;
    bool result = true;
    for (int i = 0; i < n; ++i) {
        int command = 0;
        int data = 0;
        input >> command >> data;
        switch (command) {
        case 1:
            int_deque.push_front(data);
            break;
        case 2:
            if (int_deque.size() > 0) {
                result = result && (int_deque.pop_front() == data);
            }
            else {
                result = result && (data == -1);
            }
            break;
        case 3:
            int_deque.push_back(data);
            break;
        case 4:
            if (int_deque.size() > 0) {
                result = result && (int_deque.pop_back() == data);
            }
            else {
                result = result && (data == -1);
            }
            break;
        default:
            assert(false);
        }
    }
    output << (result ? "YES" : "NO");
}

void test_deque() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "3 1 44 3 50 2 44";
        run(input, output);
        assert(output.str() == "YES");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "2 2 -1 1 10";
        run(input, output);
        assert(output.str() == "YES");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "2 3 44 4 66";
        run(input, output);
        assert(output.str() == "NO");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "8 1 44 2 44 3 55 4 55 1 66 4 66 3 77 2 77";
        run(input, output);
        assert(output.str() == "YES");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "4 2 44 4 55 1 22 3 22";
        run(input, output);
        assert(output.str() == "NO");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "6 1 1 1 1 1 1 2 1 2 1 2 1";
        run(input, output);
        assert(output.str() == "YES");
    }
}

int main() {
    run(std::cin, std::cout);
    //test_deque();
    return 0;
}
