How to compile:
(1) g++ -c main.cpp -std=c++11
(2) g++ main.o -o a.out
(3) ./a.out [OPTIONS HERE]

Options available:
--train [filename], [filename] need to be replaced to parse a train_file.
--test [filename], [filename] need to be replaced to parse a test_file.
--K [int], [int] need to be replaced by any integer to set K for kNN, if K>0 uses kNN, defaults to 0(Naive Bayes).
--C [int], [int] need to be replaced by any integer to set Laplacian correction, defaults to 0.


Example usages:
./a.out --train Data/knn2_train.csv --test Data/knn2_test.csv --K 3
./a.out --train Data/ex1_train.csv --test Data/ex1_test.csv --C 2
./a.out --train Data/ex1_train.csv --test Data/ex1_test.csv

Note:
K and C cannot be both greater than 0.
Need to use -- for options, not -.

