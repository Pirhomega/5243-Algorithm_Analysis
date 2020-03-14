/*
	5243 - Adv. Algorithm Analysis
	Dr. Halverson, Spring 20
	Hashing Experiment - Test Run
	Corbin Matamoros
	This initial version provides a proof of concept of hashing, demonstrating a hashing 
	algorithm with a hash table implemented as a dynamically allocated array. Values are read 
	in from a file and inserted into two hash tables, one with a linear probing CRP and the 
	other with double hashing. 
*/

#include<iostream>
#include<fstream>
#include<utility>
#include<string>
#include<time.h>
#include<vector>
#include "hash_table.h"

// Function prototypes. Function definitions are below main
void OpenFiles(std::ofstream&, std::ifstream&);
void GenerateRandomValues(std::ofstream&);
void Print_Tables(std::ofstream&, std::vector<Hash>&, std::vector<std::pair<int, int>>&, int[]);

int main() {
    // key is the value being hashed
    // probe_count_linear and probe_count_double will accumulate the table's probes over all insertions
    // linear_probe_66 - double_80 are hash tables. Linear_* CRP is linear probing and double_* is double hashing
    // table_array will store all the hash tables for ease of use
    //  NOTE: table_array will become more useful in the full program. That's why it's use is pointless
    //          in this initial version turned in on March 10th.
    int key, N, orig_loc, counter = 0, probe_count_linear = 0, probe_count_double = 0, load_percentage[2] = {66, 80};
    Hash linear_probe_66(311, 205), linear_probe_80(311, 250), double_66(311, 205), double_80(311, 250), temp;
    std::vector<Hash> table_array, print_array;
    std::vector<std::pair<int, int>> probe_count;
    std::ifstream infile; std::ofstream outfile;

    // push both test hash tables onto table_array
    table_array.push_back(linear_probe_66);
    table_array.push_back(linear_probe_80);
    table_array.push_back(double_66);
    table_array.push_back(double_80);

    print_array.resize(8);

    GenerateRandomValues(outfile);
    OpenFiles(outfile, infile);
    for (int i = 0; i < 2; ++i) {
        for (int i = 0; i < 2; ++i) {
            // read from input file and insert all values read to the two test hash tables
            // all while accumulating probes for each insertion
            while(infile >> key) {
                probe_count_linear += table_array[i].InsertinTable(key, true);
                probe_count_double += table_array[i+2].InsertinTable(key, false);
            }
            probe_count.push_back(std::make_pair(probe_count_linear/table_array[i].load_factor, probe_count_double/table_array[i+2].load_factor));
        }
        // push all tables to a vector of hash tables and reset the old hash tables for 
        // the next iteration of hashing
        for (int i = 0; i < 4; ++i) {
            print_array[counter] = table_array[i];
            table_array[i].Reset_Table();
            counter++;
        }
    }
    Print_Tables(outfile, print_array, probe_count, load_percentage);
    infile.close();
    outfile.close();
}


void GenerateRandomValues(std::ofstream& out) {
    // htr in htrvalues.dat stands for hash table random
    std::cout << "311 values in range (1, 10,000) will be randomly generated and dumped into file (.htrvalues.dat).\n";
    srand(time(NULL));
    out.open(".htrvalues.dat");
    for (int i = 0; i != 311; ++i)
        out << rand() % 10000 << '\n';
    out.close();
}

// OpenFiles is hardcoded to open the test input values file given in the program assignment
// and output file for ease
void OpenFiles(std::ofstream& ofile, std::ifstream& ifile) {
    std::string ofile_name, ifile_name;
    char response;
    std::cout << "Using .htrvalues.dat as input file. Would you like to use a different file (y/n): "; 
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

// Print_Tables will call each hash table's print function as well as print headers and average probes
// NOTE: "Avg. Probes #2" is not utilized in this early version so its value will always be zero.
//          Therefore, "Avg. of 2 runs" will be the same value as "Avg. Probes #1"
void Print_Tables(std::ofstream& outf, std::vector<Hash>& tables, std::vector<std::pair<int, int>>& probes, int load_percents[]) {
    outf << std::setprecision(3) << std::fixed << std::setw(31) << "Description" << std::setw(20) << "Avg. Probes #1" << std::setw(20) << "Avg. Probes #2" << std::setw(20) << "Avg. of 2 runs\n" 
        << "--------------------------------------------------------------------------------\n";
        for (int i = 0; i < 2; ++i) {
            outf << std::setw(16) << "Linear Probing, " << std::setw(2) << load_percents[i] << std::setw(12) << " load factor" << std::setw(20) << probes[i].first << std::setw(20) << probes[i+2].first << std::setw(20) << (probes[i].first + probes[i+2].first)/2 << '\n'
                 << std::setw(16) << "Double Hashing, " << std::setw(2) << load_percents[i] << std::setw(12) << " load factor" << std::setw(20) << probes[i].second << std::setw(20) << probes[i+2].second << std::setw(20) << (probes[i].second + probes[i+2].second)/2 << '\n';
        }
        for (auto table : tables) {
            outf << "--------------------------------------------------------------------------------\n\n"
            << "Hash Table: Linear Probing\n"
            << "--------------------------------------------------------------------------------\n"
            << "|Index|Value|\n"
            << "|-----|-----|\n";

            // Print the  hash table's contents
            table.Print_Table(outf);
        }
}
