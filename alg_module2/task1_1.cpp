// Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы 
// с открытой адресацией. Хранимые строки непустые и состоят из строчных латинских букв.
// Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
// Начальный размер таблицы должен быть равным 8-ми. 
// Перехеширование выполняйте при добавлении элементов в случае, 
// когда коэффициент заполнения таблицы достигает 3/4.
// Структура данных должна поддерживать операции добавления строки в множество, 
// удаления строки из множества и проверки принадлежности данной строки множеству.
// 1_1. Для разрешения коллизий используйте квадратичное пробирование. i-ая проба
// g(k, i)=g(k, i-1) + i (mod m). m - степень двойки.
// Требования: В таблице запрещено хранение указателей на описатель элемента.
// Формат входных данных
// Каждая строка входных данных задает одну операцию над множеством. 
// Запись операции состоит из типа операции и следующей за ним через пробел строки, 
// над которой проводится операция.
// Тип операции  – один из трех символов:
//     +  означает добавление данной строки в множество; 
//     -  означает удаление  строки из множества;  
//     ?  означает проверку принадлежности данной строки множеству. 
// При добавлении элемента в множество НЕ ГАРАНТИРУЕТСЯ, что он отсутствует в этом множестве.
// При удалении элемента из множества НЕ ГАРАНТИРУЕТСЯ, что он присутствует в этом множестве.
// Формат выходных данных
// Программа должна вывести для каждой операции одну из двух строк OK или FAIL, 
// в зависимости от того, встречается ли данное слово в нашем множестве.

#include <vector>
#include <string>
#include <cassert>
#include <iostream>
#include <sstream>

template <class T, class H, class P>
class HashTable {
public:
	HashTable(int initialSize, const H &_hasher, const P &_probe);
	~HashTable();
	bool Has(const T &key);
	bool Add(const T &key);
	bool Delete(const T &key);
private:
	H hasher;
	P prober;
	enum CellState {
		Empty,
		Key,
		Deleted
	};
	struct HashTableCell {
		T Key;
		unsigned int Hash;
		CellState State;
		HashTableCell() : State(Empty) {}
	};
	std::vector<HashTableCell> table;
	unsigned int keyCount;
	unsigned int deletedCount;
	unsigned int size;

	void growTable(bool grow2);
};

template <class T, class H, class P>
HashTable<T, H, P>::HashTable(int initialSize, const H &_hasher, const P &_probe) : hasher(_hasher),
																					prober(_probe),
																					table(initialSize, HashTableCell()),
																					keyCount(0),
																					deletedCount(0), 
																					size(initialSize) {}

template <class T, class H, class P>
HashTable<T, H, P>::~HashTable() {}

template <class T, class H, class P>
bool HashTable<T, H, P>::Has(const T &key) {
	unsigned int prevProbeValue = hasher(key) % size;
	for (int i = 0; i < size; ++i) {	
		unsigned int probe = prober(prevProbeValue, i) % size;
		if (table[probe].State == Empty) {
			break;
		}
		else if (table[probe].State == Key && table[probe].Key == key) {
			return true;
		}
		prevProbeValue = probe;
	}
	return false;
}

template <class T, class H, class P>
bool HashTable<T, H, P>::Add(const T &key) {
	// вопрос по правильному подбору индексов
	if (4 * keyCount >=  3 * size || 2 * deletedCount >=  size){
		if (2 * deletedCount >=  size) {
			growTable(false);
		}
		else {
			growTable(true);
		}
	}
	unsigned int hash = hasher(key);
	unsigned int prevProbeValue = hash % size;
	unsigned int deletedIndex;
	bool flagDeleted = false;
	for (int i = 0; i < size; ++i) {	
		unsigned int probe = prober(prevProbeValue, i) % size;
		if (table[probe].State == Empty) {
			if (flagDeleted)  {
				table[deletedIndex].Key = key;
				table[deletedIndex].Hash = hash;
				table[deletedIndex].State = Key;
			}
			else {
				table[probe].Key = key;
				table[probe].Hash = hash;
				table[probe].State = Key;
			}
			++keyCount;
			return true;
		}
		else if (table[probe].State == Deleted) {
			if (!flagDeleted) {
				flagDeleted = true;
				deletedIndex = probe;
			}
		}
		else if (table[probe].Key == key) {
			break;
		}
		prevProbeValue = probe;
	}
	return false;
}

template <class T, class H, class P>
bool HashTable<T, H, P>::Delete(const T &key) {
	unsigned int prevProbeValue = hasher(key) % size;
	for (int i = 0; i < size; ++i) {	
		unsigned int probe = prober(prevProbeValue, i) % size;
		if (table[probe].State == Empty) {
			break;
		}
		else if (table[probe].State == Key && table[probe].Key == key) {
				table[probe].State = Deleted;
				--keyCount;
				++deletedCount;
				return true;
		}
		prevProbeValue = probe;
	}
	return false;
}

template <class T, class H, class P>
void HashTable<T, H, P>::growTable(bool grow2) {
	deletedCount = 0;
	unsigned int newTableSize = size;
	if (grow2) {
		newTableSize *= 2;
	}
	std::vector<HashTableCell> newTable(newTableSize, HashTableCell());
	for (int listIndex = 0; listIndex < size; ++listIndex) {
		if (table[listIndex].State == Key) {
			unsigned int hash = table[listIndex].Hash;
			unsigned int prevProbeValue = hash % newTableSize;
			for (int i = 0; i < newTableSize; ++i) {
				unsigned int probe = prober(prevProbeValue, i) % newTableSize;
				if (newTable[probe].State == Empty) {
					newTable[probe] = table[listIndex];
					break;
				}
				prevProbeValue = probe;
			}
		}
	}
	size = newTableSize;
	table = std::move(newTable);
}	

struct StringHasher {
	unsigned int operator()(const std::string &key) const {
		unsigned int hash = 0;
		for (unsigned int i = 0; i < key.size(); ++i) {
			hash = hash * 137 + key[i];
		}
		return hash;
	}
};

struct Prober {
	int operator()(const int prevProbeValue, const int i) const {
		return prevProbeValue + i;
	}
};

void Run(std::istream &input, std::ostream &output) {
    StringHasher hasher;
	Prober prober;
	HashTable<std::string, StringHasher, Prober> table(8, hasher, prober);
	char operation = 0;
	std::string word;
	while (input >> operation >> word) {
		switch (operation) {
		case '+':
			output << (table.Add(word) ? "OK" : "FAIL") << std::endl;
			break;
		case '-':
			output << (table.Delete(word) ? "OK" : "FAIL") << std::endl;
			break;
		case '?':
			output << (table.Has(word) ? "OK" : "FAIL") << std::endl;
			break;
		}
	}
}

void TestHashTable() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "+ hello + bye ? bye + bye - bye ? bye ? hello";
        Run(input, output);
        assert(output.str() == "OK\nOK\nOK\nFAIL\nOK\nFAIL\nOK\n");
    }
	{
        std::stringstream input;
        std::stringstream output;
        input << "+ hello - hello + hello - hello - hello - hello ? hello";
        Run(input, output);
        assert(output.str() == "OK\nOK\nOK\nOK\nFAIL\nFAIL\nFAIL\n");
    }
	{
        std::stringstream input;
        std::stringstream output;
        input << "- hello - hello - hello - hello - hello + hello ? hello";
        Run(input, output);
        assert(output.str() == "FAIL\nFAIL\nFAIL\nFAIL\nFAIL\nOK\nOK\n");
    }
}

int main() {
	Run(std::cin, std::cout);
    //TestHashTable();
	return 0;
}
