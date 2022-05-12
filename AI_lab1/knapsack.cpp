#include <algorithm>
#include <random>
#include "json.hpp"

using namespace nlohmann;
using namespace std;

struct Item{
    char name;
    int V;
    int W;
};

bool inbag(Item a, vector<Item> bag){
    for (auto i : bag){
        if (a.name == i.name){
            return true;
        }
    }
    return false;
}

class knapsack: public hill_climbing_state{
public:
    static int T;
    static int M;
    static vector<Item> items;
    static vector<char> start_board;

    vector<vector<Item>> temp_arr;
    vector<Item> temp;
    vector<knapsack> k_arr;
    vector<Item> bag;
    knapsack(){
        for (int i =0; i<items.size();i++){
            for (int j =0; j<start_board.size();j++){
                if (items[i].name == start_board[j]){
                    bag.push_back(items[i]);
                }
            }
        }
    }

    int value() override{
        int V_total = 0;
        int W_total = 0;
        for (int i=0;i<bag.size();i++){
            V_total += bag[i].V;
            W_total += bag[i].W;
        }
        V_ = V_total;
        W_ = W_total;
        int count = max(T-V_total,0)+ max(W_total-M,0);
        return count;
    }

    vector<hill_climbing_state*> next() override{
        vector<hill_climbing_state*> k_neighbors;

        temp_arr.clear();
        temp.clear();
        k_arr.clear();

        for (int i=0;i<items.size();i++){
            if (!inbag(items[i],bag)){
                temp = bag;
                temp.push_back(items[i]);
                temp_arr.push_back(temp);
            }
        }//add operation

        for (int i=0;i<bag.size();i++) {
            temp = bag;
            temp.erase(temp.begin() + i);
            temp_arr.push_back(temp);
        }//remove operation

        for (int j=0;j<items.size();j++){
            for (int i=0;i<bag.size();i++){
                if (!inbag(items[j],bag)){
                    temp= bag;
                    temp.erase(temp.begin() + i);
                    temp.push_back(items[j]);
                    temp_arr.push_back(temp);
                }
            }
        }//swap operation

        for (int i=0;i<temp_arr.size();i++){
            knapsack k;
            k.bag = temp_arr[i];
            k_arr.push_back(k);
        }
        for (int i=0;i<k_arr.size();i++){
            k_neighbors.push_back(&k_arr[i]);
        }

        return k_neighbors;
    };

    hill_climbing_state* restart() override{
        vector<Item> new_bag = items;
        random_shuffle(new_bag.begin(), new_bag.end(),[&](int i) {return rand() % i;
        });
        int amount = rand() % items.size();
        new_bag.erase(new_bag.begin()+amount);
        knapsack k;
        k.bag = new_bag;
        k_arr.push_back(k);
        return &k_arr.back();
    };

    string print() override{
        string c;
        string s="";
        vector<char> set;
        for( auto i : bag ) {
            set.push_back(i.name);
        }
        sort(set.begin(),set.end());

        s.push_back('[');

        for( auto i : set ) {
            s.push_back(i);
            s.push_back(' ');
        }
        if (s.length()>1)
        s.pop_back();
        s.append("] = ");
        s.append(to_string(value()));
        return s;
    };

    void read_json(string input){
        ifstream f(input);
        json j = json::parse(f);

        for (auto& i:j["Items"]){
            Item item;
            string temp = i["name"];
            item.name = temp[0];
            item.V = i["V"];
            item.W = i["W"];
            items.push_back(item);
        }

        if (j["T"] == detail::value_t::null){
            cout<<"no T given, error"<<endl;
            exit(0);
        }
        T = j["T"];

        if (j["M"] == detail::value_t::null){
            cout<<"no M given, error"<<endl;
            exit(0);
        }
        M = j["M"];

        for (auto& i:j["Start"]){
            string temp = i;
            start_board.push_back(temp[0]);
        }
    };
};
int knapsack::T;
int knapsack::M;
vector<Item> knapsack::items;
vector<char> knapsack::start_board;  //define static variables
