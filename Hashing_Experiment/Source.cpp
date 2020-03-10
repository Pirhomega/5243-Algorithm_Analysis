#include<iostream>
#include<fstream>
#include<string>
#include<time.h>
#include<vector>
#include "hash_table.h"

// Function prototypes. Function definitions are below main
void OpenFiles(std::ofstream&, std::ifstream&);
void Print_Tables(std::ofstream&, std::vector<Hash>&, double, double);

int main() {
    // key is the value being hashed
    // probe_count_linear and probe_count_double will accumulate the table's probes over all insertions
    // test1 and test2 are two hash tables. Test1's CRP is linear probing and test2's is double hashing
    // table_array will store all the hash tables for ease of use
    //  NOTE: table_array will become more useful in the full program. That's why it's use is pointless
    //          in this initial version turned in on March 10th.
    int key, probe_count_linear = 0, probe_count_double = 0;
    Hash test1(31), test2(31);
    std::vector<Hash> table_array;
    std::ifstream infile;
    std::ofstream outfile;

    // push both test hash tables onto table_array
    table_array.push_back(test1);
    table_array.push_back(test2);

    OpenFiles(outfile, infile);
    // read from input file and insert all values read to the two test hash tables
    // all while accumulating probes for each insertion
    while(infile >> key) {
        probe_count_linear += table_array[0].InsertinTable(key, true);
        probe_count_double += table_array[1].InsertinTable(key, false);
    }
    Print_Tables(outfile, table_array, probe_count_linear/20.0, probe_count_double/20.0);
}

// OpenFiles is hardcoded to open the test input values file given in the program assignment and output file
// for ease
void OpenFiles(std::ofstream& ofile, std::ifstream& ifile) {
    ifile.open("test_input.txt");
    ofile.open("output.txt");
}

// Print_Tables will call each hash table's print function as well as print headers and average probes
// NOTE: "Avg. Probes #2" is not utilized in this early version so its value will always be zero.
//          Therefore, "Avg. of 2 runs" will be the same value as "Avg. Probes #1"
void Print_Tables(std::ofstream& outf, std::vector<Hash>& tables, double lin_avg, double doub_avg) {
    outf << std::setprecision(3) << std::fixed << std::setw(20) << "Description" << std::setw(20) << "Avg. Probes #1" << std::setw(20) << "Avg. Probes #2" << std::setw(20) << "Avg. of 2 runs\n" 
        << "--------------------------------------------------------------------------------\n"
        << std::setw(20) << "Linear Probing" << std::setw(20) <<  lin_avg << std::setw(20) << 0 << std::setw(20) << lin_avg << '\n'
        << std::setw(20) << "Double Hashing" << std::setw(20) <<  doub_avg << std::setw(20) << 0 << std::setw(20) << doub_avg << '\n'
        << "--------------------------------------------------------------------------------\n\n"
        << "Hash Table: Linear Probing\n"
        << "--------------------------------------------------------------------------------\n"
        << "|Index|Value|\n"
        << "|-----|-----|\n";
    // Print the first hash table's contents
    tables[0].Print_Table(outf);
    outf << "\n--------------------------------------------------------------------------------\n" 
        << "Hash Table: Double Hashing\n"
        << "--------------------------------------------------------------------------------\n"
        << "|Index|Value|\n"
        << "|-----|-----|\n";
    // Print the second hash table's contents
    tables[1].Print_Table(outf);
}