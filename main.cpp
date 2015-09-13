#include <iostream>
#include <string>

#include "Hash.h"

using namespace std;

int main() {

	hashTable test;

	for (int i = 0; i < 1000; ++i) {
		string var = "hi" + to_string(i);
		bool * b = new bool(true);
		test.insert(var, b);
		if (i < 200) {
			b = new bool(false);
			test.setPointer(var, b);
		}
	}

	bool * p = static_cast<bool *>(test.getPointer("hi30"));
	cout << *p << endl;

	system("pause");
	return 0;
}