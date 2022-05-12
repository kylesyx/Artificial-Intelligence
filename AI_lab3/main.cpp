#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <getopt.h>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include "node.cpp"
using namespace std;

float find_value_by_name(string name, vector<Node> nodes){
    float temp;
    for (int j = 0; j<nodes.size();j++){
        if (nodes[j].name==name){
            temp = nodes[j].value;
            break;
        }
    }
    return temp;
};

float get_value(Node n, vector<Node> nodes, float df){
    float v=0.0;
    if (n.edges.size()==0){ //terminal node
        return n.reward;
    }
    if (n.policy == ""){ //chance nod
        for (int i=0;i<n.edges.size();i++){
            v += n.probabilities[i] * find_value_by_name(n.edges[i], nodes);
        }
        return n.reward + df * v;
    }
    else{ //decision node
        for (int i=0;i<n.edges.size();i++){
            if (n.edges[i] == n.policy){
                v += n.success_rate * find_value_by_name(n.edges[i], nodes);
            }
            else{
                v += (1-n.success_rate)/(n.edges.size()-1) * find_value_by_name(n.edges[i], nodes);
            }
        }
        return n.reward + df * v;
    }
}

float value_iteration(vector<Node>& nodes, float df){
    float change = 0.0;
    vector<Node> temp_nodes = nodes;
    for (int i=0;i<nodes.size();i++){
        float temp = nodes[i].value;
        temp_nodes[i].value = get_value(nodes[i], nodes, df);
        float temp_change = abs(temp_nodes[i].value-temp);
        if (change<temp_change){
            change = temp_change;
        }
    }
    nodes = temp_nodes;
    return change;
}

bool single_greedy_policy(Node& n, vector<Node>& nodes, bool min){
    float current_value = find_value_by_name(n.policy, nodes);
    string temp_node = n.policy;
    float temp_value;
    bool change = false;
    for (int i=0;i<n.edges.size();i++){
        if (n.edges[i]!=n.policy){
            temp_value = find_value_by_name(n.edges[i], nodes);
            if ((!min && temp_value>current_value) || (min && temp_value<current_value )){
                current_value = temp_value;
                temp_node = n.edges[i];
                change = true;
            }
        }
    }
    n.policy = temp_node;
    return change;
}

bool greedy_policy(vector<Node>& nodes, bool min){
    bool change = false;
    for (int i=0;i<nodes.size();i++){
        if (nodes[i].policy!="" && single_greedy_policy(nodes[i],nodes,min)){
            change = true;
        }
    }
    return change;
}

void print_value(vector<Node> nodes){
    cout << setprecision(3) << fixed;
    sort( nodes.begin( ), nodes.end( ), [ ]( const Node& lhs, const Node& rhs ){
        return lhs.name<rhs.name;
    });

    for (int i=0;i<nodes.size();i++){
        cout<<nodes[i].name<<" = "<<nodes[i].value<<" ";
    }
    cout<<endl;
}

void print_policy(vector<Node> nodes){
    cout << setprecision(3) << fixed;
    sort( nodes.begin( ), nodes.end( ), [ ]( const Node& lhs, const Node& rhs ){
        return lhs.name<rhs.name;
    });
    for (int i=0;i<nodes.size();i++){
        if (nodes[i].policy!="")
            cout<<nodes[i].name<<" -> "<<nodes[i].policy<<endl;
    }
}

int main(int argc, char* argv[]) {
    int opt = 0;
    string input;
    int index = 0;
    vector<Node> nodes;
    //define default parameters
    float df = 1.0;
    bool min = false;
    float tol = 0.01;
    int iter = 100;

    const struct option longopts[]={
            {"df",1,0,'d'},
            {"min",0,0,'m'},
            {"tol",1,0,'t'},
            {"iter",1,0,'i'},
            {NULL,0,NULL,0}
    };

    while((opt = getopt_long(argc, argv, "d:mt:i:",longopts,&index)) != EOF){
        string temp;
        switch(opt){
            case 'd':
                temp=optarg;
                df = stof(temp);
                break;
            case 'm':
                min = true;
                break;
            case 't':
                temp = optarg;
                tol = stof(temp);
                break;
            case 'i':
                iter = atoi(optarg);
                break;
            default:
                fprintf(stderr, "%s: option '-%c' is invalid: ignored\n",
                        argv[0], optopt);
                break;
        }
    }

    input = argv[argc-1];

    ifstream in(input.c_str());
    if (!in) {
        cout << "Cannot open the File : " << input << endl;
        return 0;
    }
    string str, temp;
    while (getline(in, str)) {
        if (str.size() > 0 && str[0]!='#') {
            size_t found = str.find("=");
            if (found!=string::npos){
                //assign reward
                str.erase(remove(str.begin(), str.end(), ' '), str.end());
                size_t found = str.find("=");
                update_reward(nodes,str.substr(0,found),stoi(str.substr(found+1)));

            }
            found= str.find(":");
            if (found!=string::npos){
                //assign edges
                str.erase(remove(str.begin(), str.end(), ' '), str.end());
                str.erase(remove(str.begin(), str.end(), '['), str.end());
                str.erase(remove(str.begin(), str.end(), ']'), str.end());
                found= str.find(":");

                stringstream ss(str.substr(found+1));
                vector<string> temp_edges;
                while (getline(ss, temp, ',')) {
                    temp_edges.push_back(temp);
                }
                update_edges(nodes,str.substr(0,found),temp_edges);
            }
            found= str.find("%");
            if (found!=string::npos){
                //assign edges
                string temp_name = str.substr(0,found);
                temp_name.erase(remove(temp_name.begin(), temp_name.end(), ' '), temp_name.end());
                stringstream ss(str.substr(found+1));
                vector<float> temp_probability;
                while (getline(ss, temp, ' ')) {
                    if (temp.size()!=0){
                        temp_probability.push_back(stof(temp));
                    }
                }
                update_probability(nodes,temp_name,temp_probability);
            }
        }
    }
    in.close();

    //check rules
    for (int i=0;i<nodes.size();i++){
        float sum;
        if (nodes[i].probabilities.size()!=0){
            sum = 0.0;
            for (int j = 0;j<nodes[i].probabilities.size();j++){
                sum+=nodes[i].probabilities[j];
            }
            if (abs(1-sum) >= 0.001){
                cout<<"Error: probabilities should sum to 1"<<endl;
                return 0;
            }
        }
        if (nodes[i].edges.size()!=0){
            if (nodes[i].edges.size()>1 && nodes[i].probabilities.size()==0 && nodes[i].success_rate == NULL){
                nodes[i].success_rate = 1;
            }
            if (nodes[i].edges.size()==1 && nodes[i].probabilities.size()==0 && nodes[i].success_rate == NULL){
                nodes[i].probabilities.push_back(1.0);
            }
            if (nodes[i].reward==NULL){
                nodes[i].reward = 0;
            }
            if (nodes[i].success_rate != NULL){
                nodes[i].policy = nodes[i].edges[0];
            }
        }
        else{
            if (nodes[i].probabilities.size()!=0){
                cout<<"Error: terminal node should not have a probability entry"<<endl;
                return 0;
            }
        }
        nodes[i].value = nodes[i].reward;
    }

    float limit=0;
    while (true){
        for(int i=0;i<iter;i++){
            limit = value_iteration(nodes,df);
            if (limit<=tol){
                break;
            }
        }//compute V using stationery P
        if (!greedy_policy(nodes,min)){
            break;
        }
    }
    print_policy(nodes);
    cout<<endl;
    print_value(nodes);
    return 0;
}
