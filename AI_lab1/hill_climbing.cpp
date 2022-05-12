#include <iostream>
#include <vector>

using namespace std;


class hill_climbing_state{
public:
    int V_=0;
    int W_=0;
    hill_climbing_state() {};
    virtual int value(){};
    virtual vector<hill_climbing_state*> next(){};
    virtual hill_climbing_state* restart(){};
    virtual string print(){};
};

bool visited(hill_climbing_state* state, vector<hill_climbing_state*> visited_list){
    for (int i=0;i<visited_list.size();i++){
        if (state->print() == visited_list[i]->print()){
            return true;
        }
    }
    return false;
}

hill_climbing_state* hill_climbing(hill_climbing_state* start_state,int sideways, int restarts, bool verbose, char type){

    vector<hill_climbing_state*> tier_list;
    vector<hill_climbing_state*> visited_list;
    srand(time(nullptr));
    int sideway_left = sideways;
    int restart_left = restarts;
    hill_climbing_state* state = start_state;

    hill_climbing_state* temp;
    cout<<"Start: "<<state->print()<<endl;

    while(true){
        if (state->value() == 0){
            cout<<"Goal: "<<state->print()<<endl;
            return state;
        }
        visited_list.push_back(state);
        vector<hill_climbing_state*> neighbors = state->next();
        int nextValue = state->value();
        hill_climbing_state* nextState;
        bool next_found = false;

        if (verbose){
            for (hill_climbing_state* i:neighbors)
                cout<<i->print()<<endl;
        }

        for (int i=0;i<neighbors.size();i++){
            if (neighbors[i]->value()<nextValue){
                nextState = neighbors[i];
                nextValue = neighbors[i]->value();
                next_found = true;

                tier_list.clear();
            }
            else if (neighbors[i]->value()==nextValue){
                if (type == 'n'){ //for n-queens problem
                    if (visited_list.empty()){
                        tier_list.push_back(neighbors[i]);
                    }   //no visited state, push to candidate
                    else if (!visited(neighbors[i],visited_list)){
                        tier_list.push_back(neighbors[i]);
                    }  // not visited, push to candidate
                }
               else{
                    if ((neighbors[i]->V_ > nextState->V_ ||(neighbors[i]->V_ == nextState->V_ && neighbors[i]->W_ < nextState->W_ ))&& next_found){
                        temp = nextState;
                        tier_list.push_back(temp);
                        nextState = neighbors[i];
                    }
                    else{
                        if (visited_list.empty()){
                            tier_list.push_back(neighbors[i]);
                        }   //no visited state, push to candidate
                        else if (!visited(neighbors[i],visited_list)){
                            tier_list.push_back(neighbors[i]);
                        }  // not visited, push to candidate
                    }
               } //for knapsack problem
            }

        }
        if (next_found){
            if (tier_list.size()!=0 && type=='n'){
                int random = rand() % tier_list.size();
                state = tier_list[random];
            }
            else{
                state = nextState;
            }
            cout<<"choose: "<<state->print()<<endl;
            sideway_left = sideways;
            tier_list.clear();
            visited_list.clear();
            continue;
        }
        if (!next_found && sideway_left>0 && tier_list.size()!=0){
            sideway_left --;
            //move to sideway with tier breaker
            if (type == 'n'){
                int random = rand() % tier_list.size();
                state = tier_list[random];
                cout<<"choose(sideways): "<<state->print()<<endl;
                tier_list.clear();
                continue;
            } // choose randomly for n-queens
            else {
                state = tier_list[0];
                for (auto i:tier_list){
                    if (state->V_<i->V_ || (state->V_==i->V_ && state->W_>i->W_ )){
                        state = i;
                    }
                }
                cout<<"choose(sideways): "<<state->print()<<endl;
                tier_list.clear();
                continue;
            } //choose higher value followed by lower weight for knapsack
        }
        else if (restart_left>0){
            restart_left--;
            state = state->restart();
            tier_list.clear();
            visited_list.clear();
            sideway_left = sideways;
            cout<<"restarting with: "<<state->print()<<endl;
            continue;
        }
        else {
            cout<<"not found"<<endl;
            return nullptr;
        }
    }
}
