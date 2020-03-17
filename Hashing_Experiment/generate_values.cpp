#include<iostream>
#include<fstream>
#include<time.h>
void main() {
    std::ofstream out;
    // htr in htrvalues.dat stands for hash table random
    srand(time(NULL));
    out.open(".htrvalues.dat");
    for (int i = 0; i != 1000; ++i)
        out << rand() % 10000 << '\n';
    out.close();
}