/*
	5243 - Adv. Algorithm Analysis
	Dr. Halverson, Spring 20
	Hashing Experiment - Head to head: linear probing versus double hashing
	Corbin Matamoros

    This program will generate 1000 random values between 0 and 10,000
    all while avoiding duplicates.
*/

#include<iostream>
#include<fstream>
#include<string>
#include<time.h>

int main() {
    std::ofstream out;
    std::string option;
    short int a[10000] = {0}, value, count = 0;
    // htr in htrvalues.dat stands for hash table random
    srand(time(NULL));
    out.open(".htrvalues.dat");
    for (short int i = 0; i < 1000; ++i) {
        value = rand() % 10000;
        // using the counting method to avoid duplicates
        // For every value generated, mark the corresponding element
        // in the 'a' array. If the value gets generated again, 
        // the 'a' element will not be zero, and the value will not be outputed
        if (a[value] == 0) {
            ++a[value];
            out << value << '\n';
        }
    }
    out.close();
    return 0;
}