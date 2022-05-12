How to compile:
(1) g++ -c main.cpp -std=c++11
(2) g++ main.o -o a.out
(3) ./a.out [OPTIONS HERE]

Options available:
-m [mode], [mode] can be replaced by cnf/dpll/solver 
-i [filename], [filename] can replaced to parse an input file
-v, add this option to enable verbose.

Example usages:
./a.out -m dpll -i tests/test4_cnf.txt -v
./a.out -m cnf -i tests/test2_bnf.txt -v
./a.out -m solver -i tests/test2_bnf.txt

