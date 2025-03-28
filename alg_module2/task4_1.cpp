// Требование для всех вариантов Задачи 4
// Решение должно поддерживать передачу функции сравнения снаружи.

// 4_1. Солдаты. В одной военной части решили построить в одну шеренгу по росту. 
// Т.к. часть была далеко не образцовая, то солдаты часто приходили не вовремя, 
// а то их и вовсе приходилось выгонять из шеренги за плохо начищенные сапоги. 
// Однако солдаты в процессе прихода и ухода должны были всегда быть выстроены по росту 
// – сначала самые высокие, а в конце – самые низкие. 
// За расстановку солдат отвечал прапорщик, который заметил интересную особенность 
// – все солдаты в части разного роста. 
// Ваша задача состоит в том, чтобы помочь прапорщику правильно расставлять солдат, 
// а именно для каждого приходящего солдата указывать, перед каким солдатом в строе он должен становится. 
// Требования: скорость выполнения команды - O(log n).
// Формат входных данных.
// Первая строка содержит число N – количество команд (1 ≤ N ≤ 30 000). 
// В каждой следующей строке содержится описание команды: 
// число 1 и X если солдат приходит в строй (X – рост солдата, натуральное число до 100 000 включительно) 
// и число 2 и Y если солдата, стоящим в строе на месте Y надо удалить из строя. 
// Солдаты в строе нумеруются с нуля.
// Формат выходных данных.
// На каждую команду 1 (добавление в строй) вы должны выводить число K – номер позиции, 
// на которую должен встать этот солдат (все стоящие за ним двигаются назад).

#include <cassert>
#include <iostream>
#include <sstream>

template<class T, class Compare>
class AvlTree {
public:
    AvlTree(const Compare& _cmp) : root(nullptr), cmp(_cmp) {}
	~AvlTree();
    void Insert(T k);
    void FindAndRemoveMin();
    void Remove(T key);
    int InsertAndShowWhere(T key);
    void FindKStatAndRemove(int i);
    
private:
    struct Node {
        T key;
        int count;
        int height;
        Node *left;
        Node *right;
        Node(int k) : key(k), left(nullptr), right(nullptr), count(1), height(1) {}
    };
    Node* root;
    Compare cmp;
    int count(Node* p) const { return p ? p->count : 0; }
    int height(Node* p) const { return p ? p->height : 0; }
    int bfactor(Node* p) const { return height(p->right) - height(p->left); }
    void fixcount(Node* p);
    void fixheight(Node* p);
    Node* rotateright(Node* p);
    Node* rotateleft(Node* q);
    Node* balance(Node* p);
    void clear(Node* node);
    Node* insert(Node* p, const T& k, int& iStat);
    Node* findandremovemin(Node* p, Node*& min);
    Node* remove(Node* p, const T& k);
    void findkstat(Node* p, int& i, Node*& iNode) const;
};

template<class T, class Compare>
AvlTree<T, Compare>::~AvlTree() {
    clear(root);
}

template<class T, class Compare>
void AvlTree<T, Compare>::clear(Node* node) {
    if (node) {
        clear(node->left);
        clear(node->right);
        delete node;
    }
}

template<class T, class Compare>
void AvlTree<T, Compare>::fixcount(Node* p) {
	p->count = count(p->left) + count(p->right) + 1;
}

template<class T, class Compare>
void AvlTree<T, Compare>::fixheight(Node* p) {
	int hl = height(p->left);
	int hr = height(p->right);
	p->height = (hl > hr ? hl : hr) + 1;
}

template<class T, class Compare>
typename AvlTree<T, Compare>::Node* AvlTree<T, Compare>::rotateright(Node* p) {
	Node* q = p->left;
	p->left = q->right;
	q->right = p;
    fixcount(p);
	fixheight(p);
    fixcount(q);
	fixheight(q);
	return q;
}

template<class T, class Compare>
typename AvlTree<T, Compare>::Node* AvlTree<T, Compare>::rotateleft(Node* q) {
	Node* p = q->right;
	q->right = p->left;
	p->left = q;
    fixcount(q);
	fixheight(q);
    fixcount(p);
	fixheight(p);
	return p;
}

template<class T, class Compare>
typename AvlTree<T, Compare>::Node* AvlTree<T, Compare>::balance(Node* p) {
    fixcount(p);
	fixheight(p);
	if (bfactor(p) == 2) {
		if (bfactor(p->right) < 0) {
			p->right = rotateright(p->right);
        }
		return rotateleft(p);
	}
	if (bfactor(p) == -2) {
		if (bfactor(p->left) > 0 ) {
			p->left = rotateleft(p->left);
        }
		return rotateright(p);
	}
	return p;
}

template<class T, class Compare>
void AvlTree<T, Compare>::Insert(T k) {
    int iStat = 0;
    if (root) {
        iStat = count(root->left);
    }
    root = insert(root, k, iStat);
}

template<class T, class Compare>
typename AvlTree<T, Compare>::Node* AvlTree<T, Compare>::insert(Node* p, const T& k, int& iStat) {
	if (!p) {
        return new Node(k);
    }
	if (cmp(k, p->key)) {
		p->left = insert(p->left, k, iStat);
    }
	else {
        iStat += (count(p->left) + 1);
		p->right = insert(p->right, k, iStat);
    }
	return balance(p);
}

template<class T, class Compare>
void AvlTree<T, Compare>::FindAndRemoveMin() {
    Node* min;
    root = findandremovemin(root, min);
    delete min;
}

template<class T, class Compare>
typename AvlTree<T, Compare>::Node* AvlTree<T, Compare>::findandremovemin(Node* p, Node*& min) {
	if (!p->left) {
        min = p;
		return p->right;
    }
	p->left = findandremovemin(p->left, min);
	return balance(p);
}

template<class T, class Compare>
void AvlTree<T, Compare>::Remove(T key){
    root = remove(root, key);
}

template<class T, class Compare>
typename AvlTree<T, Compare>::Node* AvlTree<T, Compare>::remove(Node* p, const T& k) {
    if (!p) {
        return p;
    }
    if (cmp(k, p->key)) {
        p->left = remove(p->left, k);
    }
    else if (cmp(p->key, k)) {
        p->right = remove(p->right, k);
    }
    else {
        Node *q = p->left;
        Node *r = p->right;
        delete p;
        if (!r) {
            return q;
        }
        Node* min;
        Node* minRight = findandremovemin(r, min);
        min->right = minRight;
        min->left = q;
        return balance(min);
    } 
    return balance(p);
}

template<class T, class Compare>
void AvlTree<T, Compare>::findkstat(Node* p, int& i, Node*& iNode) const {
    if (i == count(p->left)) {
        iNode = p;
        return;
    }
    if (i < count(p->left)) {
        findkstat(p->left, i, iNode);
    }
    else {
        i = i - (count(p->left) + 1);
        findkstat(p->right, i, iNode);
    }
}

// Insert под задачу
template<class T, class Compare>
int AvlTree<T, Compare>::InsertAndShowWhere(T key) {
    int iStat = 0;
    root = insert(root, key, iStat);
    return root->count - iStat - 1;
}

// Remove под задачу
template<class T, class Compare>
void AvlTree<T, Compare>::FindKStatAndRemove(int i) {
    i = root->count - i - 1;
    if (i >= 0 && i < count(root)) {
        Node* iNode;
        findkstat(root, i, iNode);
        root = remove(root, iNode->key);
    }
}

template <class T>
struct IntCmp {
	bool operator()(const T& l, const T& r) const {return l < r;}
};

void Run(std::istream &input, std::ostream &output) {
    IntCmp<int> cmp;
    AvlTree<int, IntCmp<int>> tree(cmp);
    int n = 0;
    input >> n;
	char operation = 0;
	int num = 0;
	for (int i = 0; i < n; ++i) {
        input >> operation >> num;
		switch (operation) {
		case '1':
			output << tree.InsertAndShowWhere(num) << std::endl;
			break;
		case '2':
			tree.FindKStatAndRemove(num);
			break;
        }
	}
}

void TestAvlTree() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "5 1 100 1 200 1 50 2 1 1 150";
        Run(input, output);
        assert(output.str() == "0\n0\n2\n1\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "7 1 700 1 600 1 500 1 700 1 50 1 60 1 40";
        Run(input, output);
        assert(output.str() == "0\n1\n2\n0\n4\n4\n6\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "10 1 500 1 600 1 700 1 800 1 400 1 400 1 300 1 900 2 0 1 850";
        Run(input, output);
        assert(output.str() == "0\n0\n0\n0\n4\n4\n6\n0\n0\n");
    }
}

int main() {
	Run(std::cin, std::cout);
    //TestAvlTree();
	return 0;
}
