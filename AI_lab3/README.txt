How to compile:
(1) g++ -c main.cpp -std=c++11
(2) g++ main.o -o a.out
(3) ./a.out [OPTIONS HERE]

Options available:
--df [float], [float] can be replaced by any float between 0 and 1, to set discount factor, defaults to 1.0.
--min, add this option to use minimize values as costs.
--tol [float], [float] can be replaced by any float to set tolerance, defaults to 0.01.
--iter [int], [int] can be replaced by any integer to set cutoff for value iteration, defaults to 100.


Example usages:
./a.out --df 0.9 --min tests/test1.txt
./a.out --iter 150 tests/test1.txt
./a.out --tol 0.001 --df 0.5 --min --iter 140 tests/test2.txt

Note:
Need to use -- for options, not -.

