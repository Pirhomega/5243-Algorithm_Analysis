#include<iostream>
#include<fstream>

using namespace std;

int main() {
    ifstream infile;
    int value, a[1000] = {0};
    infile.open(".htrvalues.dat");
    for (int i = 0; i < 1000; ++i) {
        infile >> value;
        ++a[value];
    }
    for (int i = 0; i < 1000; ++i)
        if (a[i] > 1)
            cout << "found one\n";
}