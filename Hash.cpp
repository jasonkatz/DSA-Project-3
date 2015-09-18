#include "Hash.h"
#include <iostream>
#include <vector>
using namespace std;

hashTable::hashTable(int size) {
	capacity = getPrime(size);
	filled = 0;

	data.resize(capacity, hashItem());
}

int hashTable::hash(const std::string &key) {
	int hashVal = 0;

	for (unsigned int i = 0; i < key.length(); ++i) {
		hashVal = 37 * hashVal + key[i];
	}

	hashVal %= capacity;

	if (hashVal < 0) {
		hashVal += capacity;
	}

	return hashVal;
}

int hashTable::insert(const std::string &key, void *pv) {
	int hashNum = hash(key);

	if (data[hashNum].isOccupied && data[hashNum].key == key) {
		return 1;
	}

	// Linear probing
	while (data[hashNum].isOccupied) {
		hashNum++;
		hashNum %= capacity;

		if (filled > capacity) {
			return 3;
		}
	}

	// Insert item
	data[hashNum] = hashItem(key, pv);

	// Rehash check
	if ((unsigned int)++filled > (data.size() / 2)) {
		if (!rehash()) {
			return 2;
		}
	}

	return 0;
}

bool hashTable::rehash() {
	vector<hashItem> oldData = data;

	// Initialize new, bigger table, and copy old data over
	data.clear();
	data.resize(capacity = getPrime(2 * filled), hashItem());
	filled = 0;
	for (unsigned int i = 0; i < oldData.size(); ++i) {
		if (oldData[i].isOccupied && !oldData[i].isDeleted) {
			insert(oldData[i].key, oldData[i].pv);
		}
	}

	return true;
}

bool hashTable::contains(const std::string &key) {
	// Return true if findPos is successful, false otherwise
	return findPos(key) != -1;
}

void * hashTable::getPointer(const std::string &key, bool * b) {
	int index = findPos(key);

	// If no index found from key, failed
	if (index == -1) {
		if (b) {
			*b = false;
		}
		return NULL;
	}

	// Return success
	if (b) {
		*b = true;
	}
	return data[index].pv;
}

int hashTable::setPointer(const std::string &key, void * pv) {
	int index = findPos(key);

	// Return 1 if key not found in table
	if (index == -1) {
		return 1;
	}

	// Set data and return 0 if key found
	data[index].pv = pv;
	return 0;
}

bool hashTable::remove(const std::string &key) {
	int index = findPos(key);

	// Return false if key not found in table
	if (index == -1) {
		return false;
	}

	// Delete data and return true if key found
	data[index].isDeleted = true;
	return true;
}

int hashTable::findPos(const std::string &key) {
	int hashVal = hash(key);

	// Loop through occupied data (linear probing) until key is found
	while (data[hashVal].isOccupied) {
		// Return index if key matches and datum not deleted
		if (data[hashVal].key == key && !data[hashVal].isDeleted) {
			return hashVal;
		}

		++hashVal;
		hashVal %= capacity; // Be sure to wrap around to beginning
	}

	return -1;
}

unsigned int hashTable::getPrime(int size) {
	unsigned int sizeList[26] = {
		53, 97, 193, 389, 769,
		1542, 3079, 6151, 12289, 24593,
		49157, 98317, 199613, 393241, 786443,
		1572869, 3145739, 6291469, 12582917, 25165843,
		50331653, 100663319, 201326611, 402653189, 805306457,
		1610612741
	};

	// Find smallest prime in list bigger than size
	for (int i = 0; i < 26; ++i) {
		if (sizeList[i] > (unsigned int)size) {
			return sizeList[i];
		}
	}

	// Return smallest prime if no good size found
	return sizeList[0];
}

hashTable::hashItem::hashItem() {
	isOccupied = false;
	isDeleted = false;
}

hashTable::hashItem::hashItem(const std::string &k, void *p) {
	key = k;
	isOccupied = true;
	isDeleted = false;
	pv = p;
}