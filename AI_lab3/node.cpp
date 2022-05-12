#include <vector>
#include <string.h>

using namespace std;

class Node{
public:
    string name;
    float reward = NULL;
    float success_rate = NULL;
    vector<string> edges;
    vector<float> probabilities;
    string policy = "";
    float value;
};

void update_reward(vector<Node>& nodes, string name,float reward){
    bool exist = false;
    for (int i=0;i<nodes.size();i++){
        if (nodes[i].name==name){
            exist = true;
            nodes[i].reward = reward;
            break;
        }
    }
    if (!exist){
        Node newNode;
        newNode.name = name;
        newNode.reward = reward;
        nodes.push_back(newNode);
    }
}

void update_edges(vector<Node>& nodes, string name,vector<string> edge){
    bool exist = false;
    for (int i=0;i<nodes.size();i++){
        if (nodes[i].name==name){
            exist = true;
            nodes[i].edges = edge;
            break;
        }
    }
    if (!exist){
        Node newNode;
        newNode.name = name;
        newNode.edges = edge;
        nodes.push_back(newNode);
    }
}

void update_probability(vector<Node>& nodes, string name,vector<float> probabilities){
    bool exist = false;
    for (int i=0;i<nodes.size();i++){
        if (nodes[i].name==name){
            exist = true;
            if (probabilities.size()==1){
                nodes[i].success_rate = probabilities[0];
            }
            else{
                nodes[i].probabilities = probabilities;
            }
            break;
        }
    }
    if (!exist){
        Node newNode;
        newNode.name = name;
        if (probabilities.size()==1){
            newNode.success_rate = probabilities[0];
        }
        else {
            newNode.probabilities = probabilities;
        }
        nodes.push_back(newNode);
    }
}