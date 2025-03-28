#include <bitset>
#include <iostream>
#include <list>
#include <map>
#include <stack>
#include <string>
#include <sstream>
#include <vector>

typedef unsigned char Byte;
typedef std::map<Byte, std::string> CodingTable;

class IInputStream {
public:
    IInputStream() : cursor(0) {}
    IInputStream(std::string str) {
        cursor = 0;
        for (auto c : str) {
            buffer.push_back(c);
        }
    }
    IInputStream(std::vector<Byte> vec) {
        cursor = 0;
        for (auto b : vec) {
            buffer.push_back(b);
        }
    }
    virtual bool Read(Byte& value) {
        if (cursor >= buffer.size()) {
            return false;
        }
        value = buffer[cursor++];
        return true;
    }
    std::vector<Byte> buffer;
    std::size_t cursor;
};

class IOutputStream {
public:
    IOutputStream() { buffer; }
    virtual void Write(Byte value){
        buffer.push_back(value);
    }
    std::string ToString() {
        std::string str = "";
        for (auto element : buffer) {
            str += element;
        }
    }
    void Print() {
        for (auto element : buffer) {
            std::cout << element;
        }
        std::cout << std::endl;
    }
    std::vector<Byte> buffer;
};

struct Node {
    Byte byte;
    int c;
    Node* left;
    Node* right;
};

class IsMorePtrNode {
public:
    bool operator()(const Node* nodeA, const Node* nodeB) {
        return nodeA->c > nodeB->c;
    }
};

class BitStream {
public:
    int bitsCount = 0;
    int bitsCountRead = 0;
    BitStream() = default;
    void WriteBit(Byte bit);
    void WriteByte(Byte byte);
    bool ReadBit();
    Byte ReadByte();
    std::vector<Byte> GetResult() const { return buffer; }
    int GetBitsCount() const { return bitsCount; }
private:
    std::vector<Byte> buffer;
};

template<class T, class IsMore>
class Heap {
public:
    Heap(int n, IsMore isMore);
    Heap(T* arr, int arrSize, IsMore isMore);
    ~Heap();
	void Insert(const T& element);
	T ExtractMin();
    void ChangeMinAndSiftDown(const T& element);
	T PeekMin() const;
	int Size() const;
    int BufferSize() const;
private:
	IsMore isMore_;
	T* buffer_;
	int bufferSize_;
	int size_;
    int log2(int n);
    int pow2(int n);
    void buildHeap();
    void siftDown(int const i);
    void siftUp(int const i);
	void grow();
};


////////////////////ByteCountingSort////////////////////
int* ByteCountingSort(std::vector<Byte> &vecByte) {
    int* vecCount = new int[256];
    for (int i = 0; i < 256; ++i) {
        vecCount[i] = 0;
    }
    for (auto elementByte : vecByte) {
        ++vecCount[elementByte];
    }
    return vecCount;
}


////////////////////BuildHeap////////////////////
Heap<Node*, IsMorePtrNode> BuildHeap(int* vecCount) {
    IsMorePtrNode isMorePtrNode;
    Heap<Node*, IsMorePtrNode> heap(1, isMorePtrNode);
    for (int i = 0; i < 256; ++i) {
        if (vecCount[i] != 0) {
            Node* node  = new Node;
            *node = {static_cast<Byte>(i), vecCount[i], nullptr, nullptr};
            heap.Insert(node);
        }
    }
    while (heap.BufferSize() != 1) {
        Node* leftNode = heap.ExtractMin();
        Node* rightNode = heap.ExtractMin();
        Node* newNode = new Node;
        newNode->c = leftNode->c + rightNode->c;
        newNode->left = leftNode;
        newNode->right = rightNode;
        heap.Insert(newNode);
    }
    delete vecCount;
    return heap;
}


////////////////////EncodeHeap////////////////////
void EncodeHeap(Heap<Node*, IsMorePtrNode>& heap, BitStream& bitStream) {
    std::stack<Node*> stackPassNodes;
    std::list<Node*> resultDfs;
    Node* currentNode = heap.PeekMin();
    stackPassNodes.push(currentNode);
    while (!stackPassNodes.empty()) {
        currentNode = stackPassNodes.top();
        stackPassNodes.pop();
        resultDfs.push_front(currentNode);
        if (currentNode->left != nullptr) {
            stackPassNodes.push(currentNode->left);
        }
        if (currentNode->right != nullptr) {
            stackPassNodes.push(currentNode->right);
        }
    }
    for (auto node: resultDfs) {
        if (node->left == nullptr && node->right == nullptr) {
            bitStream.WriteBit(1);
            bitStream.WriteByte(node->byte);
        }
        else {
            bitStream.WriteBit(0);
        }
    }
}


////////////////////DecodeHeap////////////////////
void DecodeHeap(Heap<Node*, IsMorePtrNode>& heap, BitStream& bitStream, int sizeAlphabet) {
    std::stack<Node*> stackPassNodes;
    int countSizeAlphabet = 0;
    bool bit;
    Byte byte;
    while (sizeAlphabet != countSizeAlphabet || stackPassNodes.size() != 1) {
        bit = bitStream.ReadBit();
        if (bit) {
            byte = bitStream.ReadByte();
            Node* node = new Node;
            node->byte = byte;
            node->left = nullptr;
            node->right = nullptr;
            stackPassNodes.push(node);
            ++countSizeAlphabet;
        }
        else {
            Node* rightNode = stackPassNodes.top();
            stackPassNodes.pop();
            Node* leftNode = stackPassNodes.top();
            stackPassNodes.pop();
            Node *newNode = new Node;
            newNode->left = leftNode;
            newNode->right = rightNode;
            stackPassNodes.push(newNode);
        }
    }
    heap.Insert(stackPassNodes.top());
}


////////////////////BuildTable////////////////////
CodingTable BuildTable(Heap<Node*, IsMorePtrNode>& heap) {
    CodingTable codingTable;
    std::stack<Node*> stackPassNodes;
    std::stack<std::string> stackPassCodes;
    std::vector<Node*> resultDfsNodes;
    std::vector<std::string> resultDfsCodes;
    Node* currentNode = heap.PeekMin();
    std::string currentCode;
    stackPassNodes.push(currentNode);
    stackPassCodes.push("");
    while (!stackPassNodes.empty()) {
        currentNode = stackPassNodes.top();
        stackPassNodes.pop();
        resultDfsNodes.push_back(currentNode);
        currentCode = stackPassCodes.top();
        stackPassCodes.pop();
        resultDfsCodes.push_back(currentCode);
        if (currentNode->left != nullptr) {
            stackPassNodes.push(currentNode->left);
            stackPassCodes.push(currentCode + "0");
        }
        if (currentNode->right != nullptr) {
            stackPassNodes.push(currentNode->right);
            stackPassCodes.push(currentCode + "1");
        }
    }
    for (int i = resultDfsNodes.size() - 1 ; i >= 0; --i) {
        if (resultDfsNodes[i]->left == nullptr && resultDfsNodes[i]->right == nullptr) {
            codingTable[resultDfsNodes[i]->byte]  = resultDfsCodes[i];
        }
    }
    return codingTable;
}


////////////////////EncodeString////////////////////
void EncodeString(CodingTable& codingTable, std::vector<Byte>& vecByte, BitStream& bitStream) {
    for (auto byte : vecByte) {
        for (auto strBit : codingTable[byte]) {
            if (strBit == '0') {
                bitStream.WriteBit(0);
            }
            else {
                bitStream.WriteBit(1);
            }
        }
    }
}


////////////////////DecodeString////////////////////
void DecodeString(Heap<Node*, IsMorePtrNode>& heap, std::vector<Byte>& vecByte, BitStream& bitStream) {
    bool bit;
    Node* currentNode = heap.PeekMin();
    while (bitStream.bitsCountRead != bitStream.bitsCount) {
        bit = bitStream.ReadBit();
        if (bit) {
            currentNode = currentNode->right;
        }
        else {
            currentNode = currentNode->left;
        }
        if (currentNode->left == nullptr && currentNode->right == nullptr) {
            vecByte.push_back(currentNode->byte);
            currentNode = heap.PeekMin();
        }
    }
}


////////////////////DeleteNodes////////////////////
void DeleteNodes(Heap<Node*, IsMorePtrNode>& heap) {
    std::stack<Node*> stackPassNodes;
    std::list<Node*> resultDfs;
    Node* currentNode = heap.PeekMin();
    heap.ExtractMin();
    stackPassNodes.push(currentNode);
    while (!stackPassNodes.empty()) {
        currentNode = stackPassNodes.top();
        stackPassNodes.pop();
        resultDfs.push_front(currentNode);
        if (currentNode->left != nullptr) {
            stackPassNodes.push(currentNode->left);
        }
        if (currentNode->right != nullptr) {
            stackPassNodes.push(currentNode->right);
        }
    }
    for (auto node: resultDfs) {
        delete node;
    }
}


////////////////////BitStream////////////////////
void BitStream::WriteBit(Byte bit) {
    if (bitsCount + 1 > 8 * buffer.size()) {
        buffer.push_back(0);
    }  
    if (bit != 0) {
        int offset = bitsCount % 8;
        buffer[buffer.size() - 1] |= ((static_cast<Byte>(1) << 7) >> offset);
    }
    ++bitsCount;
}

void BitStream::WriteByte(Byte byte) {
    if (bitsCount % 8 == 0) {
        buffer.push_back(byte);
    } 
    else {
        int offset = bitsCount % 8;
        buffer[buffer.size() - 1] |= (byte >> offset);
        buffer.push_back(byte << (8 - offset));
    }
    bitsCount += 8;
}

bool BitStream::ReadBit() {
    return (buffer[bitsCountRead / 8] >> (7 - (bitsCountRead++ % 8))) & 1;
}

Byte BitStream::ReadByte() {
    Byte byte;
    int offset = bitsCountRead % 8;
    byte = buffer[bitsCountRead / 8] << offset;
    byte |= buffer[bitsCountRead / 8 + 1] >> (8 - offset);
    bitsCountRead += 8;
    return byte;
}


////////////////////Heap////////////////////
template<class T, class IsMore>
Heap<T, IsMore>::Heap(int n, IsMore isMore) {
    isMore_ = isMore;
    bufferSize_ = 0; 
    size_ = pow2(log2(n) + 1) - 1;
    buffer_ = (size_ == 0) ? new T[1] : new T[size_];
}  // Конструктор по умолчанию

template<class T, class IsMore>
Heap<T, IsMore>::Heap(T* arr, int arrSize, IsMore isMore) {
    isMore_ = isMore;
    bufferSize_ = arrSize;
    size_ = pow2(log2(arrSize) + 1) - 1;
    buffer_ = (size_ == 0) ? new T[1] : new T[size_];
    for (int i = 0; i < bufferSize_; ++i) {
        buffer_[i] = arr[i];
    }
    buildHeap();
} // Конструктор копирования массива

template<class T, class IsMore>
Heap<T, IsMore>::~Heap() {
    delete[] buffer_;
} // Деструктор

template<class T, class IsMore>
void Heap<T, IsMore>::Insert(const T& element) {
    if (size_ <= bufferSize_) {
        grow();
    }
    ++bufferSize_;
    buffer_[bufferSize_ - 1] = element;
    siftUp(bufferSize_ - 1);
} // Добавить элемент в кучу

template<class T, class IsMore>
T Heap<T, IsMore>::ExtractMin() {
    std::swap(buffer_[0], buffer_[bufferSize_ - 1]);
    --bufferSize_;
    siftDown(0);
    return buffer_[bufferSize_];
} // Извлечь максимум из кучи

template<class T, class IsMore>
void Heap<T, IsMore>::ChangeMinAndSiftDown(const T& element) {
    buffer_[0] = element;
    siftDown(0);
}

template<class T, class IsMore>
T Heap<T, IsMore>::PeekMin() const {
    return buffer_[0];
} // Посмотреть значение максимума в куче

template<class T, class IsMore>
int Heap<T, IsMore>::Size() const {
    return size_;
} // Посмотреть размер

template<class T, class IsMore>
int Heap<T, IsMore>::BufferSize() const {
    return bufferSize_;
}

template<class T, class IsMore>
int Heap<T, IsMore>::log2(int n) {
    int result = 0;
    while (n >>= 1) {
        ++result;
    }
    return result;
} // логарифм по основанию 2

template<class T, class IsMore>
int Heap<T, IsMore>::pow2(int n) {
    return (static_cast<int>(1) << n);
} // возведение 2 в степень n

template<class T, class IsMore>
void Heap<T, IsMore>::buildHeap() {
    for (int i = size_ / 2 - 1; i >= 0; --i) {
        siftDown(i);
    }
} // Создание кучи по существующему буферу

template<class T, class IsMore>
void Heap<T, IsMore>::siftDown(int const i) {
    int iSift = i;
    while (iSift < bufferSize_ / 2) {
        int leftDown = 2 * iSift + 1;
        int rightDown = 2 * iSift + 2;
        int smallest = iSift;
        if (leftDown < bufferSize_ && isMore_(buffer_[iSift], buffer_[leftDown])) {
            smallest = leftDown;
        }
        if (rightDown < bufferSize_ && isMore_(buffer_[smallest], buffer_[rightDown])){
            smallest = rightDown;
        }
        if (smallest != iSift) {
            std::swap(buffer_[iSift], buffer_[smallest]);
            iSift = smallest;
        }
        else {
            break;
        }
    }
} // Просеить элемент кучи вниз

template<class T, class IsMore>
void Heap<T, IsMore>::siftUp(int const i) {
    int iSift = i;
    while (iSift > 0) {
        int up = iSift / 2 - 1 + iSift % 2;
        if (isMore_(buffer_[up], buffer_[iSift])) {
            std::swap(buffer_[iSift], buffer_[up]);
            iSift = up;
        }
        else {
            break;
        }
    }
} // Просеить элемента кучи наверх

template<class T, class IsMore>
void Heap<T, IsMore>::grow() {
    size_ = size_ * 2 + 1;
    T* bufferNew = new T[size_];
    for (int i = 0; i < bufferSize_; ++i) {
        bufferNew[i] = buffer_[i];
    }
    delete[] buffer_;
    buffer_ = bufferNew;
} // Увеличить буфер кучи в 2 раза


////////////////////Encode&Decode////////////////////
void Encode(IInputStream& original, IOutputStream& compressed) {
    std::vector<Byte> vecByte;
    Byte byte;
    while (original.Read(byte)) {
        vecByte.push_back(byte);
    }
    Heap<Node*, IsMorePtrNode> heap = BuildHeap(ByteCountingSort(vecByte));
    BitStream bitStream;
    EncodeHeap(heap, bitStream);
    CodingTable codingTable = BuildTable(heap);
    Byte sizeAlphabet = codingTable.size();
    EncodeString(codingTable, vecByte, bitStream);
    Byte sizeFreeBits = (bitStream.GetBitsCount() % 8 == 0) ? 0 : (8 - bitStream.GetBitsCount() % 8);
    compressed.Write(sizeAlphabet);
    compressed.Write(sizeFreeBits);
    for (auto element : bitStream.GetResult()) {
        compressed.Write(element);
    }
    DeleteNodes(heap);
}

void Decode(IInputStream& compressed, IOutputStream& original) {
    Byte byte;
    compressed.Read(byte);
    Byte sizeAlphabet = byte;
    compressed.Read(byte);
    Byte sizeFreeBits = byte;
    BitStream bitStream;
    while (compressed.Read(byte)) {
        bitStream.WriteByte(byte);
    }
    bitStream.bitsCount -= sizeFreeBits;
    IsMorePtrNode isMorePtrNode;
    Heap<Node*, IsMorePtrNode> heap(1, isMorePtrNode);
    DecodeHeap(heap, bitStream, sizeAlphabet);
    std::vector<Byte> vecByte;
    DecodeString(heap, vecByte, bitStream);
    for (auto byte : vecByte) {
        original.Write(byte);
    }
    DeleteNodes(heap);
}

int main() {
    IInputStream original("ecigyudguysddtuu");
    IOutputStream compressed;
    Encode(original, compressed);
    IInputStream compressed1(compressed.buffer);
    IOutputStream original1;
    Decode(compressed1, original1);
    original1.Print();
    return 0;
}