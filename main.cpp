#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <ctime>

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

	// Load dictionary into hash table
	hashTable dictionary;

	clock_t t1 = clock();
	
	// Open dictionary file
	ifstream dictionaryFile(dictionaryName.c_str());
	if (!dictionaryFile) {
		cout << "Error opening dictionary file" << endl;
		exit(1);
	}

	// Insert each word into dictionary hash table
	string line;
	while (getline(dictionaryFile, line)) {
		// Make word lower case, then insert into dictionary
		transform(line.begin(), line.end(), line.begin(), ::tolower);
		dictionary.insert(line);
	}

	// Close dictionary file
	dictionaryFile.close();

	clock_t t2 = clock();
	double timeDiff = ((double) (t2 - t1)) / CLOCKS_PER_SEC;
	cout << "Total time (in seconds) to load dictionary: " << timeDiff << endl;

	cout << "Enter an input filename: ";
	//cin >> inputName;
	inputName = "lyrics.txt"; // TEMP LINE
	cout << inputName << endl; // TEMP LINE

	cout << "Enter an output filename: ";
	//cin >> outputName;
	outputName = "output.txt"; // TEMP LINE
	cout << outputName << endl; // TEMP LINE

	clock_t t3 = clock();

	// Open input file
	ifstream inputFile(inputName.c_str());
	if (!inputFile) {
		cout << "Error opening input file" << endl;
		exit(1);
	}

	// Open output file
	ofstream outputFile(outputName.c_str());
	if (!outputFile) {
		cout << "Error opening output file" << endl;
		exit(1);
	}

	// Spellcheck vars
	char * current = new char[1];
	string word = "";
	int lineNumber = 1;
	bool hasNumber = false;
	
	// Start main check loop:
	// Read each character from the input file,
	// Determine if each character is a valid word character,
	// If so, append to current word,
	// Else, reset the current word
	while (inputFile.read(current, 1)) {
		// Convert current to lowercase if uppercase
		if (*current >= 'A' && *current <= 'Z') {
			*current += 32;
		}

		// On file end, add char manually and ensure that wordLetter returns false
		if (inputFile.peek() == EOF) {
			word += *current;
			*current = '\0'; // Change to separator character to wordLetter returns false
		}

		if (wordLetter(*current)) {
			// If current is still a word letter, add to the current word
			// And set hasNumber condition if applicable
			word += *current;
			if (*current >= '0' && *current <= '9') {
				hasNumber = true;
			}
		} else {
			// If current is a separator, spell check the word if it exists and doesn't contain a number
			if (word != "") {
				// Don't spellcheck if word contains number
				if (!hasNumber) {
					int wordStatus = checkWord(word, dictionary);
					if (wordStatus == 1) {
						outputFile << "Long word at line " << lineNumber << ", starts: " << word.substr(0, 20) << "\n";
					} else if (wordStatus == 2) {
						outputFile << "Unknown word at line " << lineNumber << ": " << word << "\n";
					}
				} else {
					hasNumber = false;
				}
				word = "";
			}

			// Incrememt line number counter on newline character
			if (*current == '\n') {
				++lineNumber;
			}
		}
	}

	// Close input and output files
	inputFile.close();
	outputFile.close();

	clock_t t4 = clock();
	timeDiff = ((double) (t4 - t3)) / CLOCKS_PER_SEC;
	cout << "Total time (in seconds) to check document: " << timeDiff << endl;

    system("pause");
	return 0;
}

// Just a long condition checking if c is a valid word char
bool wordLetter(char c) {
	return ((c >= '0' && c <= '9') || // numbers
			(c >= 'a' && c <= 'z') || // lowercase letters
			(c >= 'A' && c <= 'Z') || // uppercase letters (not really necessary)
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
