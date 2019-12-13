#include "indexing.h"
#include "BM25.h"
#include <bits/stdc++.h>
using namespace std;
  
int main(int argc, char** argv) 
{ 
	if (argc < 6) {
		cout<<"arguements required: <--input tag> <input file> <--topic tag> <topic file> <--output tag> <output file>"<<endl;
	}
	char* input_dir;
	char* topic_dir;
	char* output_dir;
	for (int i=1;i<=6;i=i+2) {
		string s = argv[i]; 
		if (s == "--input") 
			input_dir = argv[i+1];
		else if (s == "--topic") 
			topic_dir = argv[i+1];
		else if (s == "--output") 
			output_dir = argv[i+1];
	}
	cout<<"query parsing started"<<endl;
	query_parser_qf(input_dir, topic_dir, output_dir);


	// string query = "Prime Minister Rajiv Gandhi";
	// calDocRanking(query, p.get_n_doc(), p.get_avg_doc_len(), p.get_doc_lens(), p.get_inverted_index());

    return 0;
} 