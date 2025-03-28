// Напишите функцию, проверяющую, что во всех узлах бинарного дерева поиска хранится одинаковое значение.
// Значения - int.

// Формат ввода
// По очереди вводятся узлы бинарного дерева в порядке вставки.

// Формат вывода
// 0 - если все значения в узлах бинарного дерева разные.
// 1 - если все значения в узлах бинарного дерева одинаковые.

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
    bool CheckEqual() const;

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
bool BinaryTree<T, Compare>::CheckEqual() const {
    std::list<Node*> resultDfs;
    dfsPostOrder(resultDfs);
    bool flag = true;
    T rootElement = root->Key;
    for (auto element : resultDfs) {
        if (element->Key != rootElement) {
            flag = false;
            break;
        }
    }
    return flag;
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

template <class T>
struct IntCmp {
	bool operator()(const T& l, const T& r) const {return l < r;}
};

void Run(std::istream &input, std::ostream &output) {
    IntCmp<long> cmp;
	BinaryTree<long, IntCmp<long>> tree(cmp);
    int key;
    while (input >> key) {
        tree.Add(key);
    }
    output << tree.CheckEqual();
}

void TestBinaryTree() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "2 3 1";
        Run(input, output);
        assert(output.str() == "0");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "1 1 1";
        Run(input, output);
        assert(output.str() == "1");
    }
}

int main() {
	//Run(std::cin, std::cout);
    TestBinaryTree();
	return 0;
}
