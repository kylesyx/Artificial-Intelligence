#include <iostream>
#include <fstream>
#include <getopt.h>
#include "n_queens.cpp"
#include "knapsack.cpp"

using namespace nlohmann;
using namespace std;

int main(int argc, char* argv[]) {
    int opt = 0;
    string input;
    string output;

    char* puzzle_type= "knapsack";
    int index = 0;
    int num_queens = 0;
    int sideways = 0;
    int restarts = 0;
    bool verbose = false;
    const struct option longopts[]={
            {"sideways",1,0,'s'},
            {"restarts",1,0,'r'},
            {"verbose",0,0,'v'},
            {NULL,0,NULL,0}
    };
    while((opt = getopt_long(argc, argv, "N:s:r:v",longopts,&index)) != EOF){

        switch(opt){
            case 'N':
                puzzle_type = "n_queens";
                num_queens = atoi(optarg);
                break;
            case 's':
                sideways = atoi(optarg);
                sideways = max(sideways,0);
                break;
            case 'r':
                restarts = atoi(optarg);
                restarts = max(restarts,0);
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
    if (puzzle_type == "knapsack"){
        input = argv[argc-1];
    }

    if (puzzle_type=="n_queens"){
        n_queens n(num_queens);
        hill_climbing(&n,sideways,restarts,verbose,'n');
    }
    else{
        knapsack k;
        k.read_json(input);
        knapsack k2;
        hill_climbing(&k2,sideways,restarts,verbose,'k');
    }

    return 0;
}
