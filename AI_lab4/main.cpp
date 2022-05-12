#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <getopt.h>
#include <vector>
#include "knn.cpp"

using namespace std;

float get_distance(vector<float> a, vector<float> b){
    float distance = 0;
    for (int i = 0;i < a.size();i++){
        distance += pow((b[i]-a[i]),2);
    }
    return distance;
}

vector<int> get_K_index(vector<float> distances, int k){
    vector<int> indexes;
    int num = 0;
    vector<float> copy_arr = distances;
    sort(copy_arr.begin(), copy_arr.end());
    for (int i = 0; i < distances.size(); i++){
        if (binary_search(copy_arr.begin(),copy_arr.begin() + k, distances[i])){
            if (k>num){
                indexes.push_back(i);
                num++;
            }
            else{
                break;
            }
        }
    }
    return indexes;
}

string find_knn(KNN knn, vector<KNN> trainlist, int K){
    vector<float> current_attr = knn.attributes;
    vector<float> distances;
    for (int i=0;i<trainlist.size();i++){
        float distance= get_distance(current_attr,trainlist[i].attributes);
        distances.push_back(distance);
    }
    vector<int> knn_index = get_K_index(distances,K);
    vector<KNN> result;
    for (int i=0;i<knn_index.size();i++){
        result.push_back(trainlist[knn_index[i]]);
    }
    vector<string> labels;
    for (int i=0;i<result.size();i++){
        if(find(labels.begin(), labels.end(), result[i].label) != labels.end()) {
            continue;
        }
        else{
            labels.push_back(result[i].label);
        }
    }
    vector<float> votes;
    for (int i=0;i<labels.size();i++){
        votes.push_back(0.0);
    }

    for (int i=0;i<result.size();i++){
        float temp_distance = distances[knn_index[i]];
        float weight = 0;
        int temp_index=0;
        if (temp_distance==0){
            weight = 99999999;
        }
        else weight = 1/temp_distance;
        string temp_label = result[i].label;
        auto it = find(labels.begin(), labels.end(), temp_label);
        if(it != labels.end()) {
            temp_index = it - labels.begin();
            votes[temp_index] += weight;
        }
        else cout<<"Error: No such label"<<endl;
    }
    int maxElementIndex =max_element(votes.begin(),votes.end()) - votes.begin();
    return labels[maxElementIndex];
}

string find_naive_bayes(KNN knn, vector<KNN> trainlist, int C){
    vector<int> attributes;
    for (int i=0;i<trainlist.size();i++){
        for (int j=0;j<trainlist[i].attributes.size();j++){
            if(find(attributes.begin(), attributes.end(), trainlist[i].attributes[j]) == attributes.end()) {
                attributes.push_back(trainlist[i].attributes[j]);
            }
        }
    }
    int dimension = attributes.size();
    vector<string> labels;
    for (int i=0;i<trainlist.size();i++){
        if(find(labels.begin(), labels.end(), trainlist[i].label) == labels.end()) {
            labels.push_back(trainlist[i].label);
        }
    }
    sort(labels.begin(),labels.end());

    vector<float> counts;
    vector<float> probabilities;
    int total = trainlist.size();

    for (int i=0;i<labels.size();i++){
        counts.push_back(0);
        probabilities.push_back(0.0);
    }
    int temp_index = 0;
    for (int i=0;i<trainlist.size();i++){
        auto it = find(labels.begin(), labels.end(), trainlist[i].label);
        temp_index = it - labels.begin();
        counts[temp_index]++;
    }
    for (int i=0;i<probabilities.size();i++){
        probabilities[i] = counts[i]/total;
    }

    for (int i=0;i<labels.size();i++){
        for (int j = 0;j<knn.attributes.size();j++){
            int temp_count=0;
            for (int k = 0;k<trainlist.size();k++){
                if (labels[i]==trainlist[k].label && knn.attributes[j]==trainlist[k].attributes[j]){
                    temp_count++;
                }
            }
            probabilities[i] *= (temp_count+C)/(counts[i]+C*dimension);
        }
    }
    int maxElementIndex =max_element(probabilities.begin(),probabilities.end()) - probabilities.begin();
    return labels[maxElementIndex];
}

string get_precision(string label,vector<KNN> testlist){
    vector<KNN> templist;
    int denominator = 0;
    int numerator = 0;
    for (int i=0;i<testlist.size();i++){
        if (testlist[i].predicted_label == label){
            templist.push_back(testlist[i]);
            denominator++;
        }
    }
    for (int i=0;i<templist.size();i++){
        if (templist[i].label == label){
            numerator++;
        }
    }
    string result = to_string(numerator)+'/'+ to_string(denominator);
    return result;
}

string get_recall(string label,vector<KNN> testlist){
    vector<KNN> templist;
    int denominator = 0;
    int numerator = 0;
    for (int i=0;i<testlist.size();i++){
        if (testlist[i].label == label){
            templist.push_back(testlist[i]);
            denominator++;
        }
    }
    for (int i=0;i<templist.size();i++){
        if (templist[i].predicted_label == label){
            numerator++;
        }
    }
    string result = to_string(numerator)+'/'+ to_string(denominator);
    return result;
}

void evaluate(vector<KNN> trainlist,vector<KNN> testlist){
    vector<string> labels;
    for (int i=0;i<trainlist.size();i++){
        if(find(labels.begin(), labels.end(), trainlist[i].label) != labels.end()) {
            continue;
        }
        else{
            labels.push_back(trainlist[i].label);
        }
    }
    sort(labels.begin(),labels.end());
    for (int i=0;i<labels.size();i++){
        cout<<"Label="<<labels[i]<<" Precision="<<get_precision(labels[i],testlist)<<" Recall="<<get_recall(labels[i],testlist)<<endl;
    }
}

int main(int argc, char* argv[]) {
    int opt = 0;
    string train_file;
    string test_file;
    int index = 0;
    //define default parameters
    int K = 0;
    int C = 0;

    const struct option longopts[] = {
            {"train",   1, 0,    't'},
            {"test",  1, 0,    'e'},
            {"K",  1, 0,    'k'},
            {"C", 1, 0,    'c'},
            {NULL,   0, NULL, 0}
    };

    while ((opt = getopt_long(argc, argv, "t:e:k:c:", longopts, &index)) != EOF) {
        switch (opt) {
            case 't':
                train_file = optarg;
                break;
            case 'e':
                test_file = optarg;
                break;
            case 'k':
                K = atoi(optarg);
                break;
            case 'c':
                C = atoi(optarg);
                break;
            default:
                fprintf(stderr, "%s: option '-%c' is invalid: ignored\n",
                        argv[0], optopt);
                break;
        }
    }

    if (K>0){
        if (C>0){
            cout<<"Error: K and C cannot both be greater than 0."<<endl;
            exit(0);
        }
        ifstream in(train_file.c_str());
        if (!in) {
            cout << "Cannot open the File : " << train_file << endl;
            return 0;
        }
        string line,word;
        vector<KNN> train_list;
        while (getline(in, line)){
            if (line.empty()) continue;
            stringstream str(line);
            KNN knn;
            while(getline(str,word,',')){
                if (str.eof()){
                    knn.label = word;
                }
                else{
                    knn.attributes.push_back(stof(word));
                }
            }
            train_list.push_back(knn);
        }
        ifstream in2(test_file.c_str());
        if (!in2) {
            cout << "Cannot open the File : " << train_file << endl;
            return 0;
        }

        vector<KNN> test_list;
        while (getline(in2, line)){
            if (line.empty()) continue;
            stringstream str(line);
            KNN knn;
            while(getline(str,word,',')){
                if (str.eof()){
                    knn.label = word;
                }
                else{
                    knn.attributes.push_back(stof(word));
                }
            }
            test_list.push_back(knn);
        }

        for (int i=0;i<test_list.size();i++){
            string test = find_knn(test_list[i],train_list,K);
            test_list[i].predicted_label = test;
        }
        for (int i=0;i<test_list.size();i++){
            cout<<"want="<<test_list[i].label<<" got="<<test_list[i].predicted_label<<endl;
        }
        evaluate(train_list,test_list);
    }//kNN
    else if (K==0){
        ifstream in(train_file.c_str());
        if (!in) {
            cout << "Cannot open the File : " << train_file << endl;
            return 0;
        }
        string line,word;
        vector<KNN> train_list;
        while (getline(in, line)){
            if (line.empty()) continue;
            stringstream str(line);
            KNN knn;
            while(getline(str,word,',')){
                if (str.eof()){
                    knn.label = word;
                }
                else{
                    knn.attributes.push_back(stof(word));
                }
            }
            train_list.push_back(knn);
        }
        ifstream in2(test_file.c_str());
        if (!in2) {
            cout << "Cannot open the File : " << train_file << endl;
            return 0;
        }

        vector<KNN> test_list;
        while (getline(in2, line)){
            if (line.empty()) continue;
            stringstream str(line);
            KNN knn;
            while(getline(str,word,',')){
                if (str.eof()){
                    knn.label = word;
                }
                else{
                    knn.attributes.push_back(stof(word));
                }
            }
            test_list.push_back(knn);
        }
        for (int i=0;i<test_list.size();i++){
            string test = find_naive_bayes(test_list[i],train_list,C);
            test_list[i].predicted_label = test;
        }
        evaluate(train_list,test_list);
    }//Naive Bayes
    else{
        cout<<"Error: K should be greater than or equal 0."<<endl;
        exit(0);
    }
}


