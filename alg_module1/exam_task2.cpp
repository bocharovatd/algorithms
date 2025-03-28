// ВНИМАНИЕ! Для решения данной задачи нужно использовать структуру данных стек!

// Дана последовательность, состоящая из символов '(' и ')'. 
// Требуется определить, является ли данная последовательность правильной 
// скобочной последовательностью. 
// Длина исходной последовательности ≤ 800000.

#include <iostream>


template <class T>
class stack {
private:
    T *arr;
    int capacity_;
    int size_;
    int begin_;
    int end_;

public:
    stack() : capacity_(0), size_(0), begin_(1), end_(0), arr(new T[1]) {}
    stack(stack<T> &stack_argument) {
        capacity_ = stack_argument.capacity_;
        size_ = stack_argument.size_;
        begin_ = stack_argument.begin_;
        end_ = stack_argument.end_;
        arr = new T[capacity_];
        for (int i = begin_; i <= end_; ++i) {
            arr[i] = stack_argument.arr[i];
        }
    }

    ~stack() { delete[] arr; }

    stack &operator=(stack<T> &stack_argument) {
        if (this != &stack_argument) {
            delete[] arr;
            capacity_ = stack_argument.capacity_;
            size_ = stack_argument.size_;
            begin_ = stack_argument.begin_;
            end_ = stack_argument.end_;
            arr = new T[capacity_];
            for (int i = begin_; i <= end_; ++i) {
                arr[i] = stack_argument.arr[i];
            }
        }
        return *this;
    }

    int size() const {
        return size_;
    }


    void push(const T &element) {
        if (capacity_ > size_) {
            ++size_;
            ++end_;
            arr[end_] = element;
        } 
        else {
            int new_capacity_ = (size_ == 0) ? 1 : size_ * 2;
            T *arr_temp = new T[new_capacity_];
            for (int i = begin_; i <= end_; ++i) {
                arr_temp[i] = arr[i];
            }
            ++size_;
            ++end_;
            arr_temp[end_] = element;
            delete[] arr;
            arr = arr_temp;
            capacity_ = new_capacity_;
        }
    }

    T pop() {
        --size_;
        --end_;
        return arr[end_ + 1];
    }
};


int main() {
    stack <char> s;
    std::string str;
    std::cin >> str;
    bool flag = true;
    for (auto element : str) {
        if (element == '(') {
            s.push('(');
        }
        else {
            if (s.size() == 0) {
                flag = false;
                break;
            }
            else {
                s.pop();
            }
        }
    }
    std::cout << ((flag && (s.size() == 0)) ? "YES" : "NO");
    return 0;
}