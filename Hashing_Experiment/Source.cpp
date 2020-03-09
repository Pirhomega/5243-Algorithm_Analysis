#include<iostream>
#include<fstream>
#include<string>
#include<time.h>
#include "hash_table.h"

void OpenFiles(std::ofstream&, std::ifstream&);
void GenerateRandomValues(std::ofstream&);

int main() {
    // key is the value being hashed
    // N is the value mod-ded by in the hash function (key % N)
    // table_size is the hash table size (ideally it should be a prime number)
    // orig_loc is some value i have no clue about
    int key, N, table_size = 311, orig_loc, probe_count = 0;
    std::ifstream infile;
    std::ofstream outfile;

    // std::cout << "Input hash table size: ";
    // std::cin >> table_size;
    Hash test1(31), linear_probe_66(table_size), linear_probe_80(table_size), double_66(table_size), double_80(table_size);

    // GenerateRandomValues(outfile);
    OpenFiles(outfile, infile);
    std::cout << "Finished opening files.\n";
    for (int table = 0; table < 1; ++table) {
        while(infile >> key) {
            probe_count += test1.InsertinTable(key, true);
            // linear_probe_66.InsertinTable(key, true);
            // linear_probe_80.InsertinTable(key, true);
            // double_66.InsertinTable(key, false);
            // double_80.InsertinTable(key, false);
        }
    }
    test1.Print_Table(outfile);
}

void GenerateRandomValues(std::ofstream& out) {
    // htr in htrvalues.dat stands for hash table random
    std::cout << "311 values in range (1, 10,000) will be randomly generated and dumped into file (.htrvalues.dat).";
    srand(time(NULL));
    out.open(".htrvalues.dat");
    for (int i = 0; i != 311; ++i)
        out << rand() % 10000;
}

void OpenFiles(std::ofstream& ofile, std::ifstream& ifile) {
    std::string ofile_name, ifile_name;
    char response;
    // std::cout << "Using .htrvalues.dat as input file. Would you like to use a different file (y/n): "; 
    std::cin >> response;
    if (response == 'n') {
        ifile_name = ".htrvalues.dat";
        std::cout << "Specify output file including the file extension: ";
        std::cin >> ofile_name;
    } else if (response == 'y') {
        std::cout << "Specify input and output file including the file extension: ";
        std::cin >> ifile_name >> ofile_name;
    }
    ifile.open(ifile_name);
    ofile.open(ofile_name);
}