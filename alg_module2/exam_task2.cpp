// AVL-дерево

// insert x — добавить в дерево ключ x. Если ключ x есть в дереве, то ничего делать не надо.
// delete x — удалить из дерева ключ x. Если ключа x в дереве нет, то ничего делать не надо.
// exists x — если ключ x есть в дереве, вывести «true», иначе «false»
// next x — минимальный элемент в дереве, больший x, или «none», если такого нет.
// prev x — максимальный элемент в дереве, меньший x, или «none», если такого нет.

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

template<class T, class Compare>
class AvlTree {
public:
    AvlTree(const Compare& _cmp) : root(nullptr), cmp(_cmp) {}
	~AvlTree();
    bool Has(T k);
    void Insert(T k);
    void FindAndRemoveMin();
    void Remove(T key);
    std::string Next(T key);
    std::string Prev(T key);
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
    void has(Node* p, const T& k, bool& result);
    Node* insert(Node* p, const T& k);
    Node* findandremovemin(Node* p, Node*& min);
    Node* remove(Node* p, const T& k);
    void next(Node* p, const T& k, std::string& nextPrev, std::string& nextRightLeft);
    void prev(Node* p, const T& k, std::string& prevPrev, std::string& nextLeftRight);
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
bool AvlTree<T, Compare>::Has(T k) {
    if (!root) {
        return false;
    }
    bool result = false;
    has(root, k, result);
    return result;
}

template<class T, class Compare>
void AvlTree<T, Compare>::has(Node* p, const T& k, bool& result) {
    if (!p) {
        result = false;
        return;
    }
	if (cmp(k, p->key)) {
		has(p->left, k, result);
    }
    else if (cmp(p->key, k)) {
        has(p->right, k, result);
    }
	else {
		result = true;
        return;
    }
}

template<class T, class Compare>
void AvlTree<T, Compare>::Insert(T k) {
    root = insert(root, k);
}

template<class T, class Compare>
typename AvlTree<T, Compare>::Node* AvlTree<T, Compare>::insert(Node* p, const T& k) {
	if (!p) {
        return new Node(k);
    }
	if (cmp(k, p->key)) {
		p->left = insert(p->left, k);
    }
    else if (cmp(p->key, k)) {
        p->right = insert(p->right, k);
    }
	else {
		return p;
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
std::string AvlTree<T, Compare>::Next(T k) {
    if (!root) {
        return "none";
    }
    std::string nextPrev = "none";
    std::string nextRightLeft = "none";
    next(root, k, nextPrev, nextRightLeft);
    if (nextRightLeft != "none") {
        return nextRightLeft;
    }
    return nextPrev;
}

template<class T, class Compare>
void AvlTree<T, Compare>::next(Node* p, const T& k, std::string& nextPrev, std::string& nextRightLeft) {
    if (!p) {
        return;
    }
	if (cmp(k, p->key)) {
        nextPrev = std::to_string(p->key);
		next(p->left, k, nextPrev, nextRightLeft);
    }
    else if (cmp(p->key, k)) {
        next(p->right, k, nextPrev, nextRightLeft);
    }
	else {
		if (!p->right) {
            return;
        }
        Node* nodeCheck = p->right;
        while (nodeCheck->left) {
            nodeCheck = nodeCheck->left;
        }
        nextRightLeft = std::to_string(nodeCheck->key);
        return;
    }
}

template<class T, class Compare>
std::string AvlTree<T, Compare>::Prev(T k) {
    if (!root) {
        return "none";
    }
    std::string prevPrev = "none";
    std::string prevLeftRight = "none";
    prev(root, k, prevPrev, prevLeftRight);
    if (prevLeftRight != "none") {
        return prevLeftRight;
    }
    return prevPrev;
}

template<class T, class Compare>
void AvlTree<T, Compare>::prev(Node* p, const T& k, std::string& prevPrev, std::string& prevLeftRight) {
    if (!p) {
        return;
    }
	if (cmp(k, p->key)) {
		prev(p->left, k, prevPrev, prevLeftRight);
    }
    else if (cmp(p->key, k)) {
        prevPrev = std::to_string(p->key);
        prev(p->right, k, prevPrev, prevLeftRight);
    }
	else {
		if (!p->left) {
            return;
        }
        Node* nodeCheck = p->left;
        while (nodeCheck->right) {
            nodeCheck = nodeCheck->right;
        }
        prevLeftRight = std::to_string(nodeCheck->key);
        return;
    }
}

template <class T>
struct DefaultCmp {
	bool operator()(const T& l, const T& r) const {return l < r;}
};

void Run(std::istream &input, std::ostream &output) {
    DefaultCmp<long> cmp;
    AvlTree<long, DefaultCmp<long>> tree(cmp);
    std::string operation;
	long num = 0;
	while (input >> operation >> num) {
        if (operation == "insert") {
            tree.Insert(num);
        }
        else if (operation == "delete") {
            tree.Remove(num);
        }
        else if (operation == "exists") {
            output << (tree.Has(num) ? "true" : "false") << std::endl;
        }
        else if (operation == "next") {
            output << tree.Next(num) << std::endl;
        }
		else if (operation == "prev") {
            output << tree.Prev(num) << std::endl;
        }
	}
}

void TestAvlTree() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "insert 2 insert 5 insert 3 exists 2 exists 4 next 4 prev 4 delete 5 next 4 prev 4";
        Run(input, output);
        assert(output.str() == "true\nfalse\n5\n3\nnone\n3\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "insert 1 insert 0 prev 1";
        Run(input, output);
        assert(output.str() == "0\n");
    }
}

int main() {
	//Run(std::cin, std::cout);
    TestAvlTree();
	return 0;
}
