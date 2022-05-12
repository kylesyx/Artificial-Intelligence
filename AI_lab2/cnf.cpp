#include <vector>
#include <stack>
#include <string.h>

using namespace std;

class Node{
public:
    vector<string> sentence;
    Node* left;
    Node* right;

    Node(){
        left = nullptr;
        right = nullptr;
    };

    Node(vector<string> sentence){
        this->sentence = sentence;
        left = nullptr;
        right = nullptr;
    }

    Node(vector<string> sentence, Node* left, Node* right){
        this->sentence = sentence;
        this->left = left;
        this->right = right;
    }
};

void generate_cnf_for_dpll(vector<vector<string>>& clauses,Node* head){
    stack<Node*> s;
    Node* cur = head;

    while (cur != nullptr || !s.empty()){
        while( cur != nullptr){
            s.push(cur);
            cur = cur->left;
        }
        cur = s.top();
        s.pop();
        if (cur->sentence[0] == "|"){
        }
        else if (cur->sentence[0] == "&") {
            vector<string> clause;
            clauses.push_back(clause);
        }
        else if (cur->sentence[0] == "!" && cur->sentence.size()==1 && cur->right->sentence[0][0]!='!'){
            cur->right->sentence[0] = "!"+cur->right->sentence[0];
        }
        else if (cur->sentence[0] == "!"){}
        else {
            if (clauses.size()==0){
                vector<string> clause;
                clauses.push_back(clause);
            }
            clauses[clauses.size()-1].push_back(cur->sentence[0]);
            if (cur->sentence[0] != "!" && cur->sentence[0][0] == '!'){
                cur->sentence[0] = cur->sentence[0].substr(1);
            }
        }
        cur = cur->right;
    }
}

void print_tree(Node* cur,int i){
    if (cur != nullptr){
        if (cur->left != nullptr || cur->right != nullptr){
            if (i>0)
                cout<<"(";
        }
        print_tree(cur->left,++i);
        cout<<cur->sentence[0];
        print_tree(cur->right,++i);
        if (cur->left != nullptr || cur->right != nullptr){
            if (i>2)
                cout<<")";
        }
    }
}

void print_step(Node* cur,int i){
    cout<<"step"<<i<<": "<<endl;
    print_tree(cur,0);
    cout<<endl;
}

void print_result(Node* cur){
    if (cur != nullptr){
        print_result(cur->left);
        if (cur->sentence[0] == "|"){}
        else if (cur->sentence[0] == "&") {
            cout << endl;
        }
        else if (cur->sentence[0] == "!"){
            cout<<cur->sentence[0];
        }
        else {
            cout<<cur->sentence[0]<<" ";
        }
        print_result(cur->right);
    }
}

Node* initialize_tree(vector<string> s){
    if (s.size()==0) return nullptr;
    if (s.size()==1) {
        Node* n = new Node();
        n->sentence = s;
        return n;
    }
    for (int i = s.size()-1; i>=0; i--) {
        if (s[i] == "<=>") {
            Node *n = new Node();
            n->sentence.push_back(s[i]);
            vector<string> n_left(s.begin(), s.begin() + i);
            vector<string> n_right(s.begin() + i + 1, s.end());
            n->left = initialize_tree(n_left);
            n->right = initialize_tree(n_right);
            return n;
        }
    }
    for (int i = s.size()-1; i>=0; i--) {
        if (s[i] == "=>") {
            Node *n = new Node();
            n->sentence.push_back(s[i]);
            vector<string> n_left(s.begin(), s.begin() + i);
            vector<string> n_right(s.begin() + i + 1, s.end());
            n->left = initialize_tree(n_left);
            n->right = initialize_tree(n_right);
            return n;
        }
    }
    for (int i = s.size()-1; i>=0; i--) {
        if (s[i] == "|") {
            Node *n = new Node();
            n->sentence.push_back(s[i]);
            vector<string> n_left(s.begin(), s.begin() + i);
            vector<string> n_right(s.begin() + i + 1, s.end());
            n->left = initialize_tree(n_left);
            n->right = initialize_tree(n_right);
            return n;
        }
    }
    for (int i = s.size()-1; i>=0; i--) {
        if (s[i] == "&") {
            Node *n = new Node();
            n->sentence.push_back(s[i]);
            vector<string> n_left(s.begin(), s.begin() + i);
            vector<string> n_right(s.begin() + i + 1, s.end());
            n->left = initialize_tree(n_left);
            n->right = initialize_tree(n_right);
            return n;
        }
    }
    for (int i = s.size()-1; i>=0; i--) {
        if (s[i] == "!"){
            Node* n = new Node();
            n->sentence.push_back(s[i]);
            //vector<string> n_left(s.begin(),s.begin()+i);
            vector<string> n_right(s.begin()+i+1,s.end());
            //n->left = initialize_tree(n_left);
            n->right = initialize_tree(n_right);
            return n;
        }
    }
}

void step1(Node* cur){
    if (cur == nullptr){
        return;
    }
    if (cur->sentence[0] == "<=>"){
        cur->sentence[0] = "&";
        vector<string> temp;
        temp.push_back("=>");
        Node* l = new Node(temp,cur->left,cur->right);
        Node* r = new Node(temp,cur->right,cur->left);
        cur->left = l;
        cur->right = r;
    }
    step1(cur->left);
    step1(cur->right);
}

void step2(Node* cur){
    if (cur == nullptr){
        return;
    }
    if (cur->sentence[0] == "=>"){
        cur->sentence[0] = "|";
        vector<string> temp;
        temp.push_back("!");
        Node* l = new Node(temp, nullptr,cur->left);
        cur->left = l;
    }
    step2(cur->left);
    step2(cur->right);
}

void step3_a(Node* cur){
    if (cur == nullptr){
        return;
    }
    if (cur->sentence[0] == "!" && cur->right->sentence[0] == "|"){
        cur->sentence[0] = "&";
        vector<string> temp;
        temp.push_back("!");
        Node* l = new Node(temp, nullptr,cur->right->left);
        Node* r = new Node(temp, nullptr,cur->right->right);
        cur->left = l;
        cur->right = r;
    }
    step3_a(cur->left);
    step3_a(cur->right);
}
void step3_b(Node* cur){
    if (cur == nullptr){
        return;
    }
    if (cur->sentence[0] == "!" && cur->right->sentence[0] == "&"){
        cur->sentence[0] = "|";
        vector<string> temp;
        temp.push_back("!");
        Node* l = new Node(temp, nullptr,cur->right->left);
        Node* r = new Node(temp, nullptr,cur->right->right);
        cur->left = l;
        cur->right = r;
    }
    step3_b(cur->left);
    step3_b(cur->right);
}
void step3_c(Node* cur){
    if (cur == nullptr){
        return;
    }
    if (cur->sentence[0] == "!" && cur->right->sentence[0] == "!"){
        cur->sentence[0] = cur->right->right->sentence[0];
        cur->left = cur->right->right->left;
        cur->right = cur->right->right->right;
        step3_c(cur);
    }
    step3_c(cur->left);
    step3_c(cur->right);
}
bool need_step3(Node* cur){
    if (cur == nullptr){
        return false;
    }
    if ((cur->sentence[0] == "!" && cur->right->sentence[0] == "!")
    ||(cur->sentence[0] == "!" && cur->right->sentence[0] == "&")
    ||(cur->sentence[0] == "!" && cur->right->sentence[0] == "|")){
        return true;
    }
    return (need_step3(cur->left)||need_step3(cur->right));
}
void step3(Node* cur){
    while (need_step3(cur)){
        step3_a(cur);
        step3_b(cur);
        step3_c(cur);
    }
}

bool need_step4(Node* cur){
    if (cur == nullptr){
        return false;
    }
    if ((cur->sentence[0] == "|" && cur->right->sentence[0] == "&")
        ||(cur->sentence[0] == "|" && cur->left->sentence[0] == "&")){
        return true;
    }
    return (need_step4(cur->left)||need_step4(cur->right));
}
void step4_(Node* cur){
    if (cur == nullptr){
        return;
    }
    if (cur->sentence[0] == "|" && cur->right->sentence[0] == "&"){
        cur->sentence[0] = "&";
        vector<string> temp;
        temp.push_back("|");
        Node* l = new Node(temp, cur->left,cur->right->left);
        Node* r = new Node(temp, cur->left,cur->right->right);
        cur->left = l;
        cur->right = r;
    }
    else if (cur->sentence[0] == "|" && cur->left->sentence[0] == "&"){
        cur->sentence[0] = "&";
        vector<string> temp;
        temp.push_back("|");
        Node* l = new Node(temp, cur->left->left, cur->right);
        Node* r = new Node(temp, cur->left->right,cur->right);
        cur->left = l;
        cur->right = r;
    }
    step4_(cur->left);
    step4_(cur->right);
}
void step4(Node* cur){
    while (need_step4(cur)){
        step4_(cur);
    }
}

void converter(vector<vector<string>> sentences, bool verbose, bool solver,vector<vector<string>>& clauses){
    vector<Node*> nodes;
    for (int i=0; i<sentences.size();i++){
        vector<string> sentence = sentences[i];
        Node* d;
        d = initialize_tree(sentence);
        if (verbose){
            print_step(d,0);
        }
        step1(d);
        if (verbose){
            print_step(d,1);
        }
        step2(d);
        if (verbose){
            print_step(d,2);
        }
        step3(d);
        if (verbose){
            print_step(d,3);
        }
        step4(d);
        if (verbose){
            print_step(d,4);
            cout<<endl;
        }
        nodes.push_back(d);
    }

    for (int i=0; i<sentences.size();i++){
        if (!solver){
            print_result(nodes[i]);
            cout<<endl;
        }
        else{
            vector<string> clause;
            clauses.push_back(clause);
            generate_cnf_for_dpll(clauses,nodes[i]);
        }
    }
}