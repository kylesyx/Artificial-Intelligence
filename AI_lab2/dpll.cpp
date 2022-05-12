#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class dpll{
public:
    tuple<char,bool> atom;
    tuple<char,bool>* temp;
    vector<char> literals;
    vector<char> guesses;
    vector<vector<string>> clauses;
    vector<tuple<char,bool>> assignments;
    vector<tuple<vector<vector<string>>,vector<tuple<char,bool>>>> history;

    void initialize_literals(vector<vector<string>> clause){
        clauses = clause;
        char atom;
        for (int i = 0; i < clauses.size(); i++){
            for (int j = 0; j < clauses[i].size(); j++){
                if (clauses[i][j][0]=='!'){
                    atom =clauses[i][j][1];
                }
                else{
                    atom = clauses[i][j][0];
                }
                if(find(literals.begin(), literals.end(), atom) != literals.end()) {
                    continue;
                }
                else {
                    literals.push_back(atom);
                }
            }
        }
        sort(literals.begin(), literals.end());
    }

    bool has_assignment(char atom){
        for (int i = 0; i < assignments.size(); i++){
            if (get<0>(assignments[i]) == atom)
                return true;
        }
        return false;
    }

    bool get_assignment(char atom){
        for (int i = 0; i < assignments.size(); i++){
            if (get<0>(assignments[i]) == atom)
                return get<1>(assignments[i]);
        }
        return false;
    }

    tuple<char,bool>* find_unit_literal(){
        for (int i = 0; i < clauses.size(); i++){
            if (clauses[i].size() == 1){
                if (clauses[i][0][0]=='!'){
                    atom = make_tuple(clauses[i][0][1],false);
                }
                else{
                    atom = make_tuple(clauses[i][0][0],true);
                }
                return &atom;
            }
        }
        return nullptr;
    }

    bool is_pure(char a){
        bool result,temp;
        int count = 0;
        for (int i = 0; i < clauses.size(); i++){
            for (int j = 0; j < clauses[i].size(); j++){
                int find = clauses[i][j].find(a);
                if (find!=string::npos){
                    if (find == 0){
                        temp = true;
                    }
                    else{
                        temp = false;
                    }
                    if (count>0 && temp!=result){
                        return false;
                    }
                    else{
                        result = temp;
                        count++;
                    }
                }
            }
        }
        return true;
    }

    bool get_pure(char a){
        for (int i = 0; i < clauses.size(); i++) {
            for (int j = 0; j < clauses[i].size(); j++) {
                int find = clauses[i][j].find(a);
                if (find != string::npos) {
                    if (find == 0) {
                        return true;
                    } else {
                        return false;
                    }
                }
            }
        }
    }

    tuple<char,bool>* find_pure_literal(){
        for (int i = 0; i < literals.size(); i++){
            if (!has_assignment(literals[i]) && is_pure(literals[i])){
                atom = make_tuple(literals[i], get_pure(literals[i]));
                return &atom;
            }
        }
        return nullptr;
    }

    tuple<char,bool>* find_easy_case(){
        temp = find_unit_literal();
        if (temp != nullptr){
            return temp;
        }
        temp = find_pure_literal();
        if (temp != nullptr){
            return temp;
        }
        return nullptr;
    }

    tuple<char,bool>* guess_hard_case(){
        tuple<vector<vector<string>>,vector<tuple<char,bool>>> backup;
        for (int i = 0; i < literals.size(); i++){
            if (!has_assignment(literals[i])){
                atom = make_tuple(literals[i], true);
                bool repeat = false;
                for(char c:guesses){
                    if (c == literals[i])
                        repeat = true;
                }
                if (!repeat){
                    guesses.push_back(literals[i]);
                    backup = make_tuple(clauses,assignments);
                    history.push_back(backup);
                }
                return &atom;
            }
        }
        return nullptr;
    }

    bool set_found(){
        return clauses.size()==0;
    }

    bool empty_found(){
        for (int i = 0; i < clauses.size(); i++){
            if (clauses[i].size() == 1){
                if (clauses[i][0][0]=='!' && has_assignment(clauses[i][0][1])){
                    if (clauses[i][0][1] != get_assignment(clauses[i][0][1])){
                        return true;
                    }
                }
                else if (clauses[i][0][0]!='!' && has_assignment(clauses[i][0][0])){
                    if (clauses[i][0][0] != get_assignment(clauses[i][0][1])){
                        return true;
                    }
                }
            }
        }
        return false;
    }

    void assign_then_eliminate(tuple<char,bool> assign){
        char target = get<0>(assign);
        bool value = get<1>(assign);
        assignments.push_back(assign);
        for (int i = 0; i < clauses.size(); i++){
            for (int j = 0; j < clauses[i].size();j++){
                int find = clauses[i][j].find(target);
                if (find!=string::npos){
                    if (find == 0){
                        if (value){
                            clauses.erase(clauses.begin()+i);
                            i--;
                            break;
                        }
                        else{
                            if (clauses[i].size()==1){
                                continue;
                            }
                            else{
                                clauses[i].erase(clauses[i].begin()+j);
                                j--;
                            }
                        }
                    }
                    else{
                        if (!value){
                            clauses.erase(clauses.begin()+i);
                            i--;
                            break;
                        }
                        else{
                            if (clauses[i].size()==1){
                                continue;
                            }
                            else{
                                clauses[i].erase(clauses[i].begin()+j);
                                j--;
                            }
                        }
                    }
                }
            }
        }
    }

    void propagate(bool verbose){
        if (verbose) print_clauses();
        if (set_found()){
            print_and_assign_unbounded(verbose);
            print_assignments();
            return;
        }
        tuple<char,bool> cur = make_tuple(NULL,NULL);
        if (empty_found()){
            if (verbose)
                cout<<"contradiction"<<endl;
            if (!guesses.empty()){
                char last_guess = guesses[guesses.size()-1];
                if (verbose)
                    cout<<"fail|hard case, try: "<<last_guess<<" = false"<<endl;
                guesses.pop_back();
                cur = make_tuple(last_guess, false);
                clauses = get<0>(history[history.size()-1]);
                assignments = get<1>(history[history.size()-1]);
                history.pop_back();
            }
        }
        else if (find_easy_case() != nullptr){
            cur = *find_easy_case();
            if (verbose)
                cout<<"easy case "<<get<0>(cur)<<" = "<<(get<1>(cur)?"true":"false")<<endl;
        }
        else if (guess_hard_case() != nullptr){
            cur = *guess_hard_case();
            if (verbose)
                cout<<"hard case, "<<"guess: "<<get<0>(cur)<<" = true"<<endl;
        }

        if (get<0>(cur)==NULL){
            cout<<"NO VALID ASSIGNMENT"<<endl;
            return;
        }

        assign_then_eliminate(cur);
        propagate(verbose);
    }

    void print_clauses(){
        for (int i = 0; i < clauses.size(); i++) {
            for (int j = 0; j < clauses[i].size(); j++) {
                if (j == clauses[i].size()-1)
                    cout<<clauses[i][j];
                else{
                    cout<<clauses[i][j]<<" ";
                }
            }
            cout<<endl;
        }
    }

    void print_assignments(){
        sort(assignments.begin(),assignments.end());
        for (int i = 0; i < assignments.size(); i++) {
            cout<<get<0>(assignments[i])<<" = "<<((get<1>(assignments[i]))?"true" : "false")<<endl;
        }
    }

    void print_and_assign_unbounded(bool verbose){
        for (auto i:literals){
            bool assigned = false;
            for (auto j:assignments){
                if (get<0>(j) == i){
                    assigned = true;
                }
            }
            if (!assigned){
                if (verbose){
                    cout<<"unbound "<<i<<" = false"<<endl;
                }
                tuple<char,bool> tup = make_tuple(i,false);
                assignments.push_back(tup);
            }
        }
    }
};