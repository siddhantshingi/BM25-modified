#ifndef BM25
#define BM25

#include "indexing.h"
#include <bits/stdc++.h>
#include <fstream>
using namespace std;

extern double k;
extern double b;
extern double k3;
extern double delta;
extern ofstream outfile;
extern unordered_map<string,unordered_map<string, int>> invertedIndex;
extern unordered_map<string, int> doc_lens;
extern int n_doc;
extern float avg_doc_len;

void query_parser_qf(char* input_file, char* topic_file, char* output_file);
void calDocRanking_qf(int q_no, unordered_map<string, int> qf);
void readInvertedIndex_qf(char* inputFile);


#endif