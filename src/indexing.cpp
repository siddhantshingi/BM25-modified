#include "indexing.h"
#include "stemming.h"
#include <fstream>
#include <dirent.h>
#include <bits/stdc++.h>
using namespace std;


unordered_map<string,unordered_map<string, int>> invertedIndex;
unordered_map<string, int> doc_lens;
int sum_doc_len, n_doc;
float avg_doc_len;

processed_data::processed_data() {
	this->avg_doc_len = 0;
	this->n_doc = 0;
}

processed_data::processed_data(unordered_map<string,unordered_map<string, int>> ii, unordered_map<string, int> dl, float avg_dl, int n) {
	this->inverted_index = ii;
	this->doc_lens = dl;
	this->avg_doc_len = avg_dl;
	this->n_doc = n;
}


void processed_data::set_inverted_index(unordered_map<string,unordered_map<string, int>> inverted_index) {
	this->inverted_index = inverted_index;
}

void processed_data::set_doc_lens(unordered_map<string, int> dl) {
	this->doc_lens = dl;
}
	
void processed_data::set_n_doc(int n){
	this->n_doc = n;
}

void processed_data::set_avg_doc_len(float avgdl) {
	this->avg_doc_len = avgdl;
}

void processed_data::set_size(int n){
	this->size = n;
}

unordered_map<string,unordered_map<string, int>> processed_data::get_inverted_index() {
	return this->inverted_index;
}

unordered_map<string, int> processed_data::get_doc_lens(){
	return this->doc_lens;
}

int processed_data::get_n_doc() {
	return this->n_doc;
}		

float processed_data::get_avg_doc_len() {
	return this->avg_doc_len;
}

int processed_data::get_size() {
	return this->size;
}		

void saveInvertedIndex(char* outputFile) {
	ofstream outfile;
    outfile.open(outputFile);
    
    outfile << n_doc << endl;

    unordered_map<string, int>::iterator itr_dl;
    for(itr_dl = doc_lens.begin(); itr_dl != doc_lens.end(); ++itr_dl) {
    	outfile << itr_dl->first << " " << itr_dl->second << endl;
    }

    outfile << invertedIndex.size() << endl;
	unordered_map<string, unordered_map<string, int>>::iterator itr;
	for (itr = invertedIndex.begin(); itr != invertedIndex.end(); ++itr) { 
	    outfile << itr->first << " " << itr->second.size() << " ";
	    unordered_map<string, int>::iterator itr2; 
	    for (itr2 = itr->second.begin(); itr2 != itr->second.end(); ++itr2) { 
	    	outfile << itr2->first << " " << itr2->second<<" ";
	    } 
	    outfile << endl;
	}  

	outfile.close();
}

processed_data readInvertedIndex(char* inputFile) {
	ifstream infile; 
    infile.open(inputFile);

    processed_data p;

    int n, size;
    unordered_map<string, int> dl;
    infile >> n;
    int sum = 0;
    string doc_no;
    for(int i=0; i<n; i++) {
    	infile >> doc_no;
    	infile >> dl[doc_no];
    	sum += dl[doc_no];
    }
    infile >> size;
    unordered_map<string, unordered_map<string, int>> ii;
    string word;
    for (int i=0;i<size;i++) {
    	infile >> word;
    	int size_map;
    	infile >> size_map;
    	unordered_map<string, int> ii_inner;
    	string doc_no;
    	for(int j=0;j<size_map;j++) {
    		infile >> doc_no;
    		infile >> ii_inner[doc_no];
    	}
    	ii[word] = ii_inner;
    	cout<<i<<endl;
    }

    p.set_n_doc(n);
    p.set_doc_lens(dl);
    p.set_avg_doc_len(sum/n);
    p.set_inverted_index(ii);
 	infile.close();
 	return p;
}

string replace_string(string str, string substr_out, string substr_in) {
	while(true) {	
		size_t start_pos = str.find(substr_out);
		if(start_pos == std::string::npos)
		    break;
		str.replace(start_pos, substr_out.length(), substr_in);
	}
	return str;
}


void parseFile(string fileName) {
	// cout<<fileName<<endl;

	ifstream file;
    file.open(fileName.c_str());

    int count = 0;
    int state = 0;
	unordered_set <string> stopwords_set={"a","about","above","after","again","against","all","am","an","and","any","are","aren't","as","at","be","because","been","before","being","below","between","both","but","by","can't","cannot","could","couldn't","did","didn't","do","does","doesn't","doing","don't","down","during","each","few","for","from","further","had","hadn't","has","hasn't","have","haven't","having","he","he'd","he'll","he's","her","here","here's","hers","herself","him","himself","his","how","how's","i","i'd","i'll","i'm","i've","if","in","into","is","isn't","it","it's","its","itself","let's","me","more","most","mustn't","my","myself","no","nor","not","of","off","on","once","only","or","other","ought","our","ours","ourselves","out","over","own","same","shan't","she","she'd","she'll","she's","should","shouldn't","so","some","such","than","that","that's","the","their","theirs","them","themselves","then","there","there's","these","they","they'd","they'll","they're","they've","this","those","through","to","too","under","until","up","very","was","wasn't","we","we'd","we'll","we're","we've","were","weren't","what","what's","when","when's","where","where's","which","while","who","who's","whom","why","why's","with","won't","would","wouldn't","you","you'd","you'll","you're","you've","your","yours","yourself","yourselves"};
	// unordered_set <string> tags_set={"<ORGANIZATION>","</ORGANIZATION>","<LOCATION>","</LOCATION>","<PERSON>","</PERSON>"};
    string line;
    string doc_no;
    while(getline(file,line))
    {
    	// cout<<line<<endl;
    	// cout<<"state: "<<state<<endl;
    	if (state == 0) {
    		if (line == "<DOC>") {
    			state = 1;
    			continue;
    		}
		} else if (state == 1) {
			istringstream iss(line);
			string s;
			iss >> s;
			if (s == "<DOCNO>") {
				iss >> doc_no;
				// cout<<"doc_no: "<<doc_no<<endl;
				count = 0;
				state = 2;
			} else {
				cout<<"PARSING ERROR: <DOCNO> not found at state = 1"<<endl;
			}
			continue;
		} else if (state == 2) {
			if (line == "</DOC>") {
				state = 0;
			}
			if (line == "<TEXT>") {
				state = 3;
			}
			continue;
		} else if (state == 3) {
			if (line == "</TEXT>") {
				state = 4;
				continue;
			}

			line = replace_string(line, "<ORGANIZATION> ", "");
			line = replace_string(line, " </ORGANIZATION>", "");
			line = replace_string(line, "<LOCATION> ", "");
			line = replace_string(line, " </LOCATION>", "");
			line = replace_string(line, "<PERSON> ", "");
			line = replace_string(line, " </PERSON>", "");
			line = replace_string(line, " n't", "n't");
			line = replace_string(line, "''", "\"");

			char cstr[line.size() + 1];
			strcpy(cstr, line.c_str());
		    char* token = strtok(cstr, " <>!-:|;\"_`),.(?\n"); 
			while (token != NULL) 
			{
				string intermediate = token;
				count++;
		    	if (intermediate.compare(" ") == -1) {
		    		token = strtok(NULL, " <>!-:|;\"_`),.(?\n"); 
  					continue;
  				}
		    	
		    	// remove stopwords
  				if (stopwords_set.find(intermediate) != stopwords_set.end()) {
  					token = strtok(NULL, " <>!-:|;\"_`),.(?\n"); 
  					continue;
  				}

				//do lowercase substitution
		    	transform(intermediate.begin(), intermediate.end(), intermediate.begin(), ::tolower); 
		    	
		    	// do stemming
		    	char cstr[intermediate.size() + 1];
				strcpy(cstr, intermediate.c_str());
  				int end_point = stem(cstr,0,intermediate.length() - 1);
  				intermediate = intermediate.substr(0,end_point + 1);

		    	if (invertedIndex.find(intermediate) == invertedIndex.end()) {
		    		unordered_map<string, int> v;
		    		v[doc_no] = 1;
		    		invertedIndex.insert(pair<string, unordered_map<string, int>>(intermediate, v));
		    	} else {
		    		if (invertedIndex[intermediate].find(doc_no) == invertedIndex[intermediate].end()) {
		    			invertedIndex[intermediate][doc_no] = 1;
		    		} else {
		    			invertedIndex[intermediate][doc_no] += 1;
		    		}
		    	}
		    	token = strtok(NULL, " <>!-:|;\"_`),.(?\n"); 
		    }
		    // state = 4;
		    continue;
		} else if (state == 4) {
			if (line == "<TEXT>") {
				state = 3;
			}
			if (line == "</DOC>") {
				doc_lens[doc_no] = count;
				sum_doc_len += count;
				n_doc++;
				state = 0;
			}
			continue;
		}
    }

	// unordered_map<string, unordered_map<string, int>>::iterator itr; 
 //    for (itr = invertedIndex.begin(); itr != invertedIndex.end(); ++itr) { 
 //        cout << '\t' << itr->first << setw(20) << itr->second.size() << '\t';
 //        unordered_map<string, int>::iterator itr2; 
 //        for (itr2 = itr->second.begin(); itr2 != itr->second.end(); ++itr2) { 
 //        	cout<<"<"<<itr2->first<<","<<itr2->second<<">;";
 //        } 
 //        cout<<endl;
 //    } 

    // cout<<"parsed file: "<<fileName<<endl;
}

processed_data readDir(char* path){
	// cout<<path<<endl;

	struct dirent *entry;
	DIR *dir = opendir(path);
   
	if (dir == NULL) {
		processed_data p;
		return p;
	}
	sum_doc_len = 0;
	n_doc = 0;
	avg_doc_len = 0.0;
	while ((entry = readdir(dir)) != NULL) {
		string dir_path = path;
		string fileName = entry->d_name;
		if (fileName != "." && fileName != "..") { 
			fileName = dir_path.append(fileName);
			parseFile(fileName);
		}
	}
	avg_doc_len = sum_doc_len/n_doc;

	cout<<endl;
	cout<<"processed data: "<<endl; 
	cout<<"size of invertedIndex: "<<invertedIndex.size()<<endl;
	cout<<"avg_doc_len: "<<avg_doc_len<<endl;
	cout<<"n_doc: "<<n_doc<<endl;
	cout<<"doc_lens size: "<<doc_lens.size()<<endl;

	processed_data p = processed_data(invertedIndex, doc_lens, avg_doc_len, n_doc);
	// readInvertedIndex("data2.dat");
	closedir(dir);
	return p;
}	
