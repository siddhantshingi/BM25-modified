#include "indexing.h"
#include "stemming.h"
#include <fstream>
#include <vector>
#include <climits>

double k = 2;
double b = 0.55;
double k3 = 3.7;
double delta = -1;
ofstream outfile;
unordered_map<string,unordered_map<string, int>> ii;
unordered_map<string, int> dl;
int n;
float avg_dl;

void calDocRanking_qf(int q_no, unordered_map<string, int> qf) {
  map<string, double> score;
  unordered_map<string, int>::iterator itr; 
  unordered_map<string, int>::iterator itr_qf;
  for (itr_qf = qf.begin(); itr_qf != qf.end(); itr_qf++) {
    string intermediate = itr_qf->first;
    unordered_map<string, int> vt = ii[intermediate];
    int df = vt.size();
    //BM25:
    for (itr = vt.begin(); itr != vt.end(); ++itr) {
      double tf = itr->second;
      if (tf == 0) {
        continue;
      }
      double bm25_of_d = 0;
      double term0 = (n - df + 0.5)/(df + 0.5);
      double term1 = log(term0);
      double term2 = 1 - b + b*itr->second/avg_dl;
      double term3 = (k+1)*tf/(tf + k*term2) + delta;
      double term4 = (k3 + 1)*itr_qf->second/(k3 + itr_qf->second);
      bm25_of_d = term1 * term3 * term4;
      score[itr->first] += bm25_of_d;
    } 
  } 

  int num = min(int(score.size()),50);
  for(int i=0;i<num;i++)
  {
    auto x = std::max_element(score.begin(), score.end(),
    [](const pair<string, double>& p1, const pair<string, double>& p2) {
        return p1.second < p2.second; });
    outfile<<q_no<<" 0 "<<x->first<<" "<<i+1<<" "<<x->second<<" s"<<endl;
    score.erase(x);
  }
}

void readInvertedIndex_qf(char* inputFile) {
  ifstream infile; 
  infile.open(inputFile);

  int size;
  infile >> n;
  int sum = 0;
  string doc_no;
  for(int i=0; i<n; i++) {
    infile >> doc_no;
    infile >> dl[doc_no];
    sum += dl[doc_no];
  }
  infile >> size;
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

  avg_dl = sum/n;
  infile.close();
}


void query_parser_qf(char* input_file, char* topic_file, char* output_file) {
  
  cout<<"reading: "<<endl;

  readInvertedIndex_qf(input_file);

  cout<<endl;
  cout<<"read data from output_dir: "<<endl; 
  cout<<"size of invertedIndex: "<<ii.size()<<endl;
  cout<<"avg_doc_len: "<<avg_dl<<endl;
  cout<<"n_doc: "<<n<<endl;
  cout<<"doc_lens size: "<<dl.size()<<endl;  

  ifstream file;
  file.open(topic_file);
  outfile.open(output_file);


  int state = 0, q_no;
  string line, tag;
  unordered_set <string> inc_tags = {"<smry","<narr","<con>","<def>"};
  unordered_set <string> spec_tags = {"<titl"};
  unordered_set <string> ign_tags_1 = {"<fac>","<desc","</fac>"};
  unordered_set <string> ign_tags_2 = {"<top>","<head","<dom>"};
  unordered_set <string> stopwords_set={"a","about","above","after","again","against","all","am","an","and","any","are","aren't","as","at","be","because","been","before","being","below","between","both","but","by","can't","cannot","could","couldn't","did","didn't","do","does","doesn't","doing","don't","down","during","each","few","for","from","further","had","hadn't","has","hasn't","have","haven't","having","he","he'd","he'll","he's","her","here","here's","hers","herself","him","himself","his","how","how's","i","i'd","i'll","i'm","i've","if","in","into","is","isn't","it","it's","its","itself","let's","me","more","most","mustn't","my","myself","no","nor","not","of","off","on","once","only","or","other","ought","our","ours","ourselves","out","over","own","same","shan't","she","she'd","she'll","she's","should","shouldn't","so","some","such","than","that","that's","the","their","theirs","them","themselves","then","there","there's","these","they","they'd","they'll","they're","they've","this","those","through","to","too","under","until","up","very","was","wasn't","we","we'd","we'll","we're","we've","were","weren't","what","what's","when","when's","where","where's","which","while","who","who's","whom","why","why's","with","won't","would","wouldn't","you","you'd","you'll","you're","you've","your","yours","yourself","yourselves"};

  
  string query = "";
  while(getline(file,line))
  {
    // cout<<line<<endl;
    // cout<<"state: "<<state<<endl;
    if (line == "") continue;
    tag = line.substr(0,5);
    // cout<<endl<<tag<<state<<endl;
    if (ign_tags_2.find(tag) != ign_tags_2.end()) continue;
    if (tag == "</top") {
      query = replace_string(query, " n't", "n't");
      query = replace_string(query, "''", "\"");

      cout<<q_no<<endl;
      unordered_map<string, int> qf;      
      char cstr[query.size() + 1];
      strcpy(cstr, query.c_str());
      char* token = strtok(cstr, " <>!-:|;\"_`),.(?\n");
      // cout<<"starting"<<endl;
      while (token != NULL) 
      {
        string intermediate = token;

        //remove spaces
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

        //do stemming
        char cstr[intermediate.size() + 1];
        strcpy(cstr, intermediate.c_str());
        int end_point = stem(cstr,0,intermediate.length() - 1);
        intermediate = intermediate.substr(0,end_point + 1);
        
        qf[intermediate] += 1;

        token = strtok(NULL, " <>!-:|;\"_`),.(?\n"); 
      } 

      calDocRanking_qf(q_no, qf);
      // cout<<query<<endl;
      query = "";
      state = 0;
      continue;
    }
    
    if (state == 0) {
      istringstream iss(line);
      string s;
      iss >> s;
      if (s == "<num>") {
        while(true) {
          iss >> s;
          if (s == " " || s == "Number:")
            continue;
          q_no = stoi(s);
          break;
        }
        // cout<<endl<<"q_no: "<<q_no<<endl;
        state = 1;
      }
      continue;

    } else if (state == 1) {
      if (spec_tags.find(tag) != spec_tags.end()) {
        if (query == "")
          query = line.substr(15,line.size()) + " " + line.substr(15,line.size());
        else
          query = query + " " + line.substr(15,line.size()) + " " + line.substr(15,line.size());
      } else if (inc_tags.find(tag) != inc_tags.end()) {
        state = 2;
      }
      continue;
    } else if (state == 2) {
      if (spec_tags.find(tag) != spec_tags.end()) {
        if (query == "")
          query = line.substr(15,line.size()) + " " + line.substr(15,line.size());
        else
          query = query + " " + line.substr(15,line.size()) + " " + line.substr(15,line.size());
        continue;
      } else if (inc_tags.find(tag) != inc_tags.end()) {
        state = 2;
        continue;
      } else if (ign_tags_1.find(tag) != ign_tags_1.end()) {
        state = 3;
        continue;
      }
      // cout<<line<<endl;
      if (query == "")
        query = line;
      else
        query = query + " " + line;
      
      continue;

    } else if (state == 3) {
      if (spec_tags.find(tag) != spec_tags.end()) {
        if (query == "")
          query = line.substr(15,line.size()) + " " + line.substr(15,line.size());
        else
          query = query + " " + line.substr(15,line.size()) + " " + line.substr(15,line.size());
      } else if (inc_tags.find(tag) != inc_tags.end()) {
        state = 2;
      } else if (ign_tags_1.find(tag) != ign_tags_1.end()) {
        state = 3;
      }
      continue;

    } 
      
  }
  cout<<"out of loop"<<endl;
  outfile.close();
  cout<<"out file closed"<<endl;
  file.close();
  cout<<"topic file closed"<<endl;
}




