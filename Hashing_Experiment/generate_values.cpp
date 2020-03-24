#include<iostream>
#include<fstream>
#include<string>
#include<time.h>

void dupchecker(std::string& file_name) {
    std::ifstream infile;
    int value, a[1000] = {0};
    infile.open(file_name);
    for (int i = 0; i < 1000; ++i) {
        infile >> value;
        ++a[value];
    }
    for (int i = 0; i < 1000; ++i)
        if (a[i] > 1)
            std::cout << "found one\n";
}

int main() {
    std::ofstream out;
    std::string option;
    // htr in htrvalues.dat stands for hash table random
    srand(time(NULL));
    std::cin >> option;
    out.open(option);
    for (int i = 0; i != 1000; ++i)
        out << rand() % 10000 << '\n';
    out.close();
    dupchecker(option);
    return 0;
}