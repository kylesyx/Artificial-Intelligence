#include "hill_climbing.cpp"
#include <algorithm>
#include <random>

using namespace std;

class n_queens: public hill_climbing_state{
public:
    vector<int> board;
    vector<vector<int>> temp_arr;
    vector<int> temp;
    vector<n_queens> n_arr;

    n_queens(int size){
        for (int i = 0;i<size;i++){
            board.push_back(i);
        }
    }

    int value() override{
        int count = 0;
        for (int i=0;i<board.size();i++){
            for (int j=i+1;j<board.size();j++){
                if (board[i] == board[j] || abs(board[i]-board[j])==abs(i-j))
                    count++;
            }
        }
        return count;
    }

    vector<hill_climbing_state*> next() override{
        vector<hill_climbing_state*> neighbors;

        temp_arr.clear();
        temp.clear();
        n_arr.clear();

        for (int i=0;i<board.size();i++){
            for (int j=i+1;j<board.size();j++){
                temp= board;
                swap(temp[i],temp[j]);
                temp_arr.push_back(temp);
            }
        }

        for (int i=0;i<temp_arr.size();i++){
            n_queens n(temp_arr[i].size());
            n.board = temp_arr[i];
            n_arr.push_back(n);
        }
        for (int i=0;i<n_arr.size();i++){
            neighbors.push_back(&n_arr[i]);
        }
        return neighbors;
    };

    hill_climbing_state* restart() override{
        vector<int> new_board = board;
        random_shuffle(new_board.begin(), new_board.end(),[&](int i) {return rand() % i;
        });
        n_queens n(new_board.size());
        //resize(new_board.size());
        n.board = new_board;
        n_arr.push_back(n);
        return &n_arr.back();
    };

    string print() override{
        string c;
        string s="";
        s.push_back('[');
        for( int i : board ) {
            c = to_string(i);
            s.append(c);
            s.push_back(' ');
        }
        if (s.length()>1)
        s.pop_back();
        s.append("] = ");
        s.append(to_string(value()));
        return s;
    };
};


