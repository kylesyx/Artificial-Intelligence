#include <iostream>
#include <fstream>
#include <sstream>
#include <getopt.h>
#include "dpll.cpp"
#include "cnf.cpp"

using namespace std;

bool check_complete_clause(vector<string> clause){
    for (int i=0;i<clause.size()-1;i++){
        for (int j=i+1;j<clause.size();j++){
            if (clause[i].size()==1 && clause[j].size()==2 && clause[i][0]==clause[j][1]){
                return true;
            }
            if (clause[i].size()==2 && clause[j].size()==1 && clause[i][1]==clause[j][0]){
                return true;
            }
        }
    }
    return false;
}

int main(int argc, char* argv[]) {
    int opt = 0;
    string input;
    int index = 0;
    bool verbose = false;
    vector<vector<string>> clauses;
    vector<vector<string>> sentences;

    char* mode;
    const struct option longopts[]={
            {"mode",1,0,'m'},
            {"input",1,0,'i'},
            {"verbose",0,0,'v'},
            {NULL,0,NULL,0}
    };

    while((opt = getopt_long(argc, argv, "m:i:v",longopts,&index)) != EOF){
        switch(opt){
            case 'm':
                mode = optarg;
                break;
            case 'i':
                input = optarg;
                break;
            case 'v':
                verbose = true;
                break;
            default:
                fprintf(stderr, "%s: option '-%c' is invalid: ignored\n",
                        argv[0], optopt);
                break;
        }
    }
    if (strcmp(mode,"dpll")==0) {
        ifstream in(input.c_str());
        if (!in) {
            cout << "Cannot open the File : " << input << endl;
            return 0;
        }
        string str, temp;
        while (getline(in, str)) {
            if (str.size() > 0) {
                vector<string> clause;
                stringstream ss(str);
                while (getline(ss, temp, ' ')) {
                    if (temp[0] != NULL) {
                        clause.push_back(temp);
                    }
                }
                if (!check_complete_clause(clause))
                    clauses.push_back(clause);
            }
        }
        in.close();
        dpll d;
        d.initialize_literals(clauses);
        d.propagate(verbose);
    }

    if (strcmp(mode,"cnf")==0) {
        ifstream in(input.c_str());
        if (!in) {
            cout << "Cannot open the File : " << input << endl;
            return 0;
        }
        string str,temp;
        while (getline(in, str)) {
            if (str.size() > 0) {
                vector<string> sentence;
                stringstream ss(str);
                while (getline(ss, temp, ' ')) {
                    if (temp[0] != NULL) {
                        sentence.push_back(temp);
                    }
                }
                sentences.push_back(sentence);
            }
        }
        in.close();
        converter(sentences,verbose,false,clauses);
    }

    if (strcmp(mode,"solver")==0){
        ifstream in(input.c_str());
        if (!in) {
            cout << "Cannot open the File : " << input << endl;
            return 0;
        }
        string str,temp;
        while (getline(in, str)) {
            if (str.size() > 0) {
                vector<string> sentence;
                stringstream ss(str);
                while (getline(ss, temp, ' ')) {
                    if (temp[0] != NULL) {
                        sentence.push_back(temp);
                    }
                }
                sentences.push_back(sentence);
            }
        }
        in.close();
        converter(sentences,false,true,clauses);
        for (int i=0;i<clauses.size();i++){
            if (check_complete_clause(clauses[i])){
                clauses.erase(clauses.begin()+i);
                i--;
            }
        }
        dpll d;
        d.initialize_literals(clauses);
        d.propagate(verbose);
    }
    return 0;
}
