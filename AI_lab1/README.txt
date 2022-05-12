How to compile:
(1) g++ -c main.cpp -std=c++11
(2) g++ main.o -o a.out
(3) ./a.out [OPTIONS HERE]

Options available:
-N (int), int can be replaced by any integer to solve n-queens problem. e.g ./a.out -N5 to solve 5-queens problem
[filename], filename can replaced to parse a json to solve knapsack problem e.g ./a.out knapsack1.txt
-sideways (int)/ -s (int),int can be replaced to any integer, this is the max sideways allowed, default is 0 e.g ./a.out -N 5 -sideways 5
-restarts (int)/ -r (int),int can be replaced to any integer, this is the max restarts allowed, default is 0 e.g ./a.out -N 5 -restarts 5
-v, add this option to enable verbose.
 
