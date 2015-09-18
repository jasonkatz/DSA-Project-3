#include <iostream>
#include <string>
#include <fstream>

#include "Hash.h"

using namespace std;

bool wordLetter(char);
int checkWord(string, hashTable&);

int main() {

	string dictionaryName;
	string inputName;
	string outputName;

	cout << "Enter a dictionary filename: ";
	//cin >> dictionaryName;
	dictionaryName = "wordlist_small.sdx"; // TEMP LINE
	cout << dictionaryName << endl; // TEMP LINE

	cout << "Enter an input filename: ";
	//cin >> inputName;
	inputName = "lyrics.txt"; // TEMP LINE
	cout << inputName << endl; // TEMP LINE

	cout << "Enter an output filename: ";
	//cin >> outputName;
	outputName = "lyrics.txt"; // TEMP LINE
	cout << outputName << endl; // TEMP LINE

	// Load dictionary into hash table
	hashTable dictionary;

	ifstream dictionaryFile(dictionaryName);
	if (!dictionaryFile) {
		cout << "Error opening dictionary file" << endl;
		exit(1);
	}

	string line;
	while (getline(dictionaryFile, line)) {
		dictionary.insert(line);
	}

	// Open input file
	ifstream inputFile(inputName);
	if (!inputFile) {
		cout << "Error opening input file" << endl;
		exit(1);
	}

	char * current = new char[1];
	string word = "";
	int lineNumber = 1;
	
	// Start main check loop:
	// Read each character from the input file,
	// Determine if each character is a valid word character,
	// If so, append to current word,
	// Else, reset the current word
	while (inputFile.read(current, 1)) {
		// Incrememt line number counter on newline character
		if (*current == '\n') {
			++lineNumber;
		}
		if (wordLetter(*current)) {
			word += *current;
		} else {
			if (word != "") {
				// Dump out contains data
				cout << word << "\t";
				int wordStatus = checkWord(word, dictionary);
				if (wordStatus == 0) {
					cout << "Nothing wrong" << endl;
				} else if (wordStatus == 1) {
					cout << "Too long" << endl;
				} else if (wordStatus == 2) {
					cout << "Not in dictionary" << endl;
				}
				word = "";
			}
		}
	}

	cout << "Done reading" << endl;

	system("pause");
	return 0;
}

// Just a long condition checking if c is a valid word char
bool wordLetter(char c) {
	return ((c >= '0' && c <= '9') || // numbers
			(c >= 'a' && c <= 'z') || // lowercase letters
			(c >= 'A' && c <= 'Z') || // uppercase letters
			c == '-' || c == '\'');   // - and '
}

// Checks word validity and returns and integer status:
// 0 for success
// 1 for long word
// 2 for non-existing word
int checkWord(string word, hashTable &dictionary) {
	if (word.length() > 20) {
		return 1;
	}
	if (!dictionary.contains(word)) {
		return 2;
	}
	return 0;
}