#include "hash_table.hpp"

HashTable::HashTable() : table(1, HashTableCell()),
						 keyCount(0),
						 deletedCount(0),
						 size(1) {}

HashTable::~HashTable() {}

bool HashTable::Has(const int &key) const {
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

bool HashTable::Add(const int &key) {
	// вопрос по правильному подбору индексов
	if (2 * keyCount >=  size || 2 * deletedCount >=  size){
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

bool HashTable::Delete(const int &key) {
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

void HashTable::growTable(bool grow2) {
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
