#pragma once 

#include <vector>
#include <string>
#include <cassert>
#include <iostream>
#include <sstream>

class HashTable {
public:
	HashTable();
	~HashTable();
	bool Has(const int &key) const;
	bool Add(const int &key);
	bool Delete(const int &key);
private:
	std::hash<int> hasher;
	int prober(const int prevProbeValue, const int i) const {
        return prevProbeValue + i;
    }
	enum CellState {
		Empty,
		Key,
		Deleted
	};
	struct HashTableCell {
		int Key;
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
