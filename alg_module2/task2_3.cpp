// Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N.
// Требуется построить бинарное дерево, заданное наивным порядком вставки.
// Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K, 
// то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
// Требования: Рекурсия запрещена. Решение должно поддерживать передачу функции сравнения снаружи.
// 2_3. Выведите элементы в порядке post-order (снизу вверх).

#include <cassert>
#include <iostream>
#include <list>
#include <sstream>
#include <stack>

template<class T, class Compare>
class BinaryTree {
public:
	BinaryTree(const Compare& _cmp) : root(nullptr), cmp(_cmp) {}
	~BinaryTree();
	void Add(const T& key);
	bool Has(const T& key) const;
    void PrintPostOrder(std::ostream &output) const;

private:
	struct Node {
		T Key;
		Node* Left;
		Node* Right;
		Node(const T& key) : Key(key), Left(nullptr), Right(nullptr) {}
	};
	Node* root;
	Compare cmp;
    void dfsPostOrder(std::list<Node*>& resultDfs) const;
};

template<class T, class Compare>
BinaryTree<T, Compare>::~BinaryTree() {
    std::list<Node*> resultDfs;
    dfsPostOrder(resultDfs);
	for (auto element: resultDfs) {
        delete element;
    }
}

template<class T, class Compare>
void BinaryTree<T, Compare>::Add(const T& key) {
    if (root == nullptr) {
        root = new Node(key);
        return;
    }
    Node* currentNode = root;
	while (true) {
        if (cmp(key, currentNode->Key)) {
            if (currentNode->Left == nullptr) {
                currentNode->Left = new Node(key);
                return;
            }
            currentNode = currentNode->Left;
        }
        else {
            if (currentNode->Right == nullptr) {
                currentNode->Right = new Node(key);
                return;
            }
            currentNode = currentNode->Right;
        }
    }
}

template<class T, class Compare>
bool BinaryTree<T, Compare>::Has( const T& key ) const {
    if (root == nullptr) {
        return false;
    }
	Node* currentNode = root;
    while (currentNode != nullptr && currentNode->Key != key) {
        if (cmp(key, currentNode->Key)) {
            currentNode = currentNode->Left;
        }
        else {
            currentNode = currentNode->Right;
        }
    }
    if (currentNode != nullptr) {
        return true;
    }
    return false;
}

template<class T, class Compare>
void BinaryTree<T, Compare>::dfsPostOrder(std::list<Node*>& resultDfs) const {
    if (root == nullptr) {
        return;
    }
    std::stack<Node*> stackPass;
    stackPass.push(root);
    Node* currentNode;
    while (!stackPass.empty()) {
        currentNode = stackPass.top();
        stackPass.pop();
        resultDfs.push_front(currentNode);
        if (currentNode->Left != nullptr) {
            stackPass.push(currentNode->Left);
        }
        if (currentNode->Right != nullptr) {
            stackPass.push(currentNode->Right);
        }
    }
}

template<class T, class Compare>
void BinaryTree<T, Compare>::PrintPostOrder(std::ostream &output) const {
    std::list<Node*> resultDfs;
    dfsPostOrder(resultDfs);
    for (auto element: resultDfs) {
        output << element->Key << " ";
    }
}

template <class T>
struct IntCmp {
	bool operator()(const T& l, const T& r) const {return l < r;}
};

void Run(std::istream &input, std::ostream &output) {
    IntCmp<long> cmp;
	BinaryTree<long, IntCmp<long>> tree(cmp);
    int n;
    input >> n;
    for (int i = 0; i < n; ++i) {
        long key;
        input >> key;
        tree.Add(key);
    }
    tree.PrintPostOrder(output);
}

void TestBinaryTree() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "10 7 2 10 8 5 3 6 4 1 9";
        Run(input, output);
        assert(output.str() == "1 4 3 6 5 2 9 8 10 7 ");
    }
	{
        std::stringstream input;
        std::stringstream output;
        input << "10 7 10 2 6 1 9 4 5 8 3";
        Run(input, output);
        assert(output.str() == "1 3 5 4 6 2 8 9 10 7 ");
    }
	{
        std::stringstream input;
        std::stringstream output;
        input << "10 4 3 9 5 6 8 7 2 10 1";
        Run(input, output);
        assert(output.str() == "1 2 3 7 8 6 5 10 9 4 ");
    }
}

int main() {
	//Run(std::cin, std::cout);
    TestBinaryTree();
	return 0;
}
