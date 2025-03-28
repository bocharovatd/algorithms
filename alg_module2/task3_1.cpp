// Постройте B-дерево минимального порядка t и выведите его по слоям.
// В качестве ключа используются числа, лежащие в диапазоне [0..232-1]
// Требования:
// B-дерево должно быть реализовано в виде шаблонного класса.
// Решение должно поддерживать передачу функции сравнения снаружи.
// Формат ввода
// Сначала вводится минимальный порядок дерева t.
// Затем вводятся элементы дерева.
// Формат вывода
// Программа должна вывести B-дерево по слоям.
// Каждый слой на новой строке, элементы должны выводится в том порядке, в котором они лежат в узлах.

#include <cassert>
#include <functional>
#include <iostream>
#include <queue>
#include <sstream>
#include <vector>

template<class T, class Compare>
class BTree {
public:
	BTree(int _t, const Compare& _cmp) : root(nullptr), t(_t), cmp(_cmp) {}
	~BTree();
	void Insert(const T& key);
	void Bfs(const std::function<void(T key)>& keyFunction,
			 const std::function<void()>& afterLayerFunction);

private:
	struct Node {
		bool IsLeaf;
		std::vector<T> Keys;
		std::vector<Node*> Children;
		Node(bool isLeaf) : IsLeaf(isLeaf) {}
	};
	Node* root;
	int t;
	Compare cmp;
	bool isNodeFull(Node* node) const { return node->Keys.size() == 2 * t - 1; }
	void splitChild(Node* node, int pos);
	void insertNonFull(Node* node, const T& key);
};

template<class T, class Compare>
BTree<T, Compare>::~BTree() {
    std::queue<Node*> queuePass;
	std::vector<Node*> resultBfs;
	queuePass.push(root);
	Node* currentNode;
	while (!queuePass.empty()) {
		currentNode = queuePass.front();
		queuePass.pop();
		resultBfs.push_back(currentNode);
		for (auto elementNode : currentNode->Children) {
			queuePass.push(elementNode);
		}
	}
	for (auto elementNode:  resultBfs) {
		delete elementNode;
    }
}

template<class T, class Compare>
void BTree<T, Compare>::Insert(const T& key) {
	if (root == nullptr) {
		root = new Node(true);
	}
	if (isNodeFull(root)) {
		Node* newRoot = new Node(false);
		newRoot->Children.push_back(root);
		root = newRoot;
		splitChild(root, 0);
	}
	insertNonFull(root, key);
}

template<class T, class Compare>
void BTree<T, Compare>::insertNonFull(Node* node, const T& key) {
	if(node->IsLeaf) {
		node->Keys.resize(node->Keys.size() + 1);
		int pos = node->Keys.size() - 1;
		while (pos - 1 >= 0 && cmp(key, node->Keys[pos - 1])) {
			node->Keys[pos] = node->Keys[pos - 1];
			--pos;
		}
		node->Keys[pos] = key;
	} 
    else {
		int pos = node->Keys.size();
		while (pos - 1 >= 0 && cmp(key, node->Keys[pos - 1])) {
			--pos;
		}
		if (isNodeFull(node->Children[pos])) {
			splitChild(node, pos);
			if (key > node->Keys[pos]) {
				++pos;
			}
		}
		insertNonFull(node->Children[pos], key);
	}
}

template<class T, class Compare>
void BTree<T, Compare>::splitChild(Node* node, int pos) {
	// вычисление позиции и вставка срединного ключа из ключей разделяемого ребенка в node
	int insertPosKeys = node->Children[pos]->Keys.size() / 2;
	T key = node->Children[pos]->Keys[insertPosKeys];
	node->Keys.insert(node->Keys.begin() + pos, key);
	// создание двух новых детей из одного
	Node* leftNode = new Node(node->Children[pos]->IsLeaf);
	Node* rightNode = new Node(node->Children[pos]->IsLeaf);
	// копирование в них данных из первоначального ребенка
	std::vector<T> leftKeys;
	std::copy(node->Children[pos]->Keys.begin(),
			  node->Children[pos]->Keys.begin() + insertPosKeys,
			  back_inserter(leftKeys));;
	std::vector<T> rightKeys;
	std::copy(node->Children[pos]->Keys.begin() + insertPosKeys + 1,
			  node->Children[pos]->Keys.end(),
			  back_inserter(rightKeys));
	leftNode->Keys = leftKeys;
	rightNode->Keys = rightKeys;
	if (node->Children[pos]->Children.size() != 0) {
		int insertPosLeftBorderChildren = insertPosKeys;
		std::vector<Node*> leftChildren;
		std::copy(node->Children[pos]->Children.begin(),
				  node->Children[pos]->Children.begin() + insertPosKeys + 1,
				  back_inserter(leftChildren));
		std::vector<Node*> rightChildren;
		std::copy(node->Children[pos]->Children.begin() + insertPosKeys + 1,
				  node->Children[pos]->Children.end(),
				  back_inserter(rightChildren));
		leftNode->Children = leftChildren;
		rightNode->Children = rightChildren;
	}
	// удаление узла разделяемого ребенка
	delete node->Children[pos];
	// привязка двух новых детей к node
	node->Children[pos] = leftNode;
	node->Children.insert(node->Children.begin() + pos + 1, rightNode);
}

template <class T, class Compare>
void BTree<T, Compare>::Bfs(const std::function<void(T key)>& keyFunction,
							const std::function<void()>& afterLayerFunction) {
	std::queue<Node*> queuePass;
	std::vector<Node*> resultBfs;
	queuePass.push(root);
	int count = 1;
	int countNewLayer = 0;
	Node* currentNode;
	while (!queuePass.empty()) {
		currentNode = queuePass.front();
		queuePass.pop();
		resultBfs.push_back(currentNode);
		countNewLayer += currentNode->Children.size();
		--count;
		if (count == 0) {
			resultBfs.push_back(nullptr);
			count = countNewLayer;
			countNewLayer = 0;
		}
		for (auto elementNode : currentNode->Children) {
			queuePass.push(elementNode);
		}
	}
	for (auto elementNode:  resultBfs) {
		if (elementNode == nullptr) {
			afterLayerFunction();
		}
		else {
			for (auto elementKey : elementNode->Keys) {
				keyFunction(elementKey);
			}
		}
    }
}

template <class T>
struct IntCmp {
	bool operator()(const T& l, const T& r) const {return l < r;}
};

void Run(std::istream &input, std::ostream &output) {
    int t;
    input >> t;
	IntCmp<unsigned long> cmp;
	BTree<unsigned long, IntCmp<unsigned long>> tree(t, cmp);
	unsigned long key;
    while (input >> key) {
		tree.Insert(key);
	}
	tree.Bfs([&output](unsigned long key)
			 { output << key << " "; },
			 [&output]()
			 { output << std::endl; });
}

void TestBTree() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "2 0 1 2 3 4 5 6 7 8 9";
        Run(input, output);
        assert(output.str() == "3 \n1 5 7 \n0 2 4 6 8 9 \n");
    }
	{
        std::stringstream input;
        std::stringstream output;
        input << "4 0 1 2 3 4 5 6 7 8 9";
        Run(input, output);
        assert(output.str() == "3 \n0 1 2 4 5 6 7 8 9 \n");
    }
}

int main() {
	//Run(std::cin, std::cout);
    TestBTree();
	return 0;
}
