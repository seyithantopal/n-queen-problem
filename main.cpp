#include <iostream>
#include <array>
#include <chrono>
#include <random>
#include <cmath>
#include <set>
#include <string>
#include <algorithm>
using namespace std;

#define TABLE_SIZE 8

int h = 0;

// Random number generator
unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
minstd_rand0 g1(seed);
default_random_engine generator{static_cast<unsigned int>(seed)};
uniform_int_distribution<unsigned int> distribution(0, TABLE_SIZE - 1);

unsigned int randomNumber() {	
	return distribution(generator);
}

void displayTable(array<array<int, TABLE_SIZE>, TABLE_SIZE>& table, string tableName) {
	cout << tableName << endl;
	for(int i = 0; i < TABLE_SIZE; ++i) {
		for(int j = 0; j < TABLE_SIZE; ++j) {
			cout << table[i][j] << ' ';
		}
		cout << endl;
	}
}

void cleanRow(array<array<int, TABLE_SIZE>, TABLE_SIZE>& table, int row) {
	for(int i = 0; i < TABLE_SIZE; ++i) {
		table[row][i] = 0;
	}
}

void fillTable(array<array<int, TABLE_SIZE>, TABLE_SIZE>& table, array<int, TABLE_SIZE>& queens) {	
	for(int i = 0; i < TABLE_SIZE; ++i) {
		int num = randomNumber();
		for(int j = 0; j < TABLE_SIZE; ++j) {
			if(j == num) {
				table[i][j] = 1;
				queens[i] = num;
			} else {
				table[i][j] = 0;
			}
		}
	}
}

int calculateH(array<array<int, TABLE_SIZE>, TABLE_SIZE>& table, array<int, TABLE_SIZE>& queens) {
	int h = 0;
	array<int, TABLE_SIZE> closedCorner{};
	array<int, TABLE_SIZE> closedAround{};
	for(int i = 0; i < TABLE_SIZE; ++i) {
		for(int j = 0; j < TABLE_SIZE; ++j) {
			if(abs(i - j) == abs(queens[i] - queens[j])) {
				if(i != j) {
					if(closedCorner[j] == 0) {
						closedCorner[i] = 1;
						h++;
					}
				}
			}
			if(abs(i - j) == 0 || abs(queens[i] - queens[j]) == 0) {
				if(i != j) {
					if(closedAround[j] == 0) {
						closedAround[i] = 1;
						h++;
					}
				}
			}
		}
	}
	return h;
}

void process(array<array<int, TABLE_SIZE>, TABLE_SIZE>& table, array<array<int, TABLE_SIZE>, TABLE_SIZE>& alteredTable, array<int, TABLE_SIZE>& queens) {
	int temp = -1, trials;
	set<int> selected;
	set<int>::iterator iter;
	for(int i = 0; i < TABLE_SIZE; ++i) {
		if(temp == 0) break;
		selected.clear();
		trials = 0;
		while(true) {
			++trials;
			if(trials == 1000) break;
			int random = randomNumber();
			cleanRow(table, i);
			table[i][random] = 1;
			queens[i] = random;
			temp = calculateH(table, queens);
			if(temp < h) {
				h = temp;
				copy(table.begin(), table.end(), alteredTable.begin());
				break;
			}
		}
	}
}

int main() {
	array<array<int, TABLE_SIZE>, TABLE_SIZE> table{};
	array<array<int, TABLE_SIZE>, TABLE_SIZE> alteredTable{};
	array<int, TABLE_SIZE> queens{};

	// Fill the table with random values
	fillTable(table, queens);

	// Calculate heuristic function
	h = calculateH(table, queens);

	// Display original table and heuristic function
	displayTable(table, "Original Table");
	cout << "Original h: " << h << endl;
	cout << endl;

	// Start process to find the best heuristic function
	process(table, alteredTable, queens);

	// Display altered table and heuristic function
	displayTable(alteredTable, "Altered Table");
	cout << "Final h: " << h << endl;
}