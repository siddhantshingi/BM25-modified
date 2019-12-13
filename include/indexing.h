#ifndef INDEXING
#define INDEXING

#include <bits/stdc++.h>
using namespace std;

struct processed_data  
{ 
	unordered_map<string,unordered_map<string, int>> inverted_index;
	unordered_map<string, int> doc_lens;
	int n_doc;
	float avg_doc_len;
	int size;

	processed_data();
	processed_data(unordered_map<string,unordered_map<string, int>> inverted_index, unordered_map<string, int> doc_lens, float avg_doc_len, int n_doc);

	void set_inverted_index(unordered_map<string,unordered_map<string, int>> invertedIndex);
	void set_doc_lens(unordered_map<string, int> dl);
	void set_n_doc(int n);
	void set_avg_doc_len(float avgdl);
	void set_size(int);
	unordered_map<string,unordered_map<string, int>> get_inverted_index();
	unordered_map<string, int> get_doc_lens();
	int get_n_doc();
	float get_avg_doc_len();
	int get_size();
}; 
extern unordered_map<string,unordered_map<string, int>> invertedIndex;
extern unordered_map<string, int> doc_lens;
extern int sum_doc_len, n_doc;
extern float avg_doc_len;
void saveInvertedIndex(char* outputFile);
processed_data readInvertedIndex(char* inputFile);
string replace_string(string str, string substr_out, string substr_in);
void parseFile(string fileName);
processed_data readDir(char* dirName);

#endif