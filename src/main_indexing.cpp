#include "indexing.h"
#include <bits/stdc++.h>
using namespace std;
  
int main(int argc, char** argv) 
{ 
	if (argc < 4) {
		cout<<"arguements required: <--input tag> <input dir> <--output tag> <output dir>"<<endl;
	}
	char* input_dir;
	char* output_dir;
	for (int i=1;i<=4;i=i+2) {
		string s = argv[i];
		if (s == "--input") 
			input_dir = argv[i+1];
		else if (s == "--output") 
			output_dir = argv[i+1];
	}

	processed_data p = readDir(input_dir);

	cout<<endl;
	cout<<"processed data in main.cpp: "<<endl; 
	cout<<"size of invertedIndex: "<<p.get_inverted_index().size()<<endl;
	cout<<"avg_doc_len: "<<p.get_avg_doc_len()<<endl;
	cout<<"n_doc: "<<p.get_n_doc()<<endl;
	cout<<"doc_lens size: "<<p.get_doc_lens().size()<<endl;

	// dump in a file
	saveInvertedIndex(output_dir);
	
    return 0;
} 