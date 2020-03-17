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
void Print_Tables(std::ofstream&, std::vector<Hash>&, std::vector<std::pair<float, float>>&, int[]);

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
    std::vector<std::pair<float, float>> probe_count;
    std::ifstream infile; std::ofstream outfile;

    // push both test hash tables onto table_array
    table_array.push_back(linear_probe_66);
    table_array.push_back(linear_probe_80);
    table_array.push_back(double_66);
    table_array.push_back(double_80);

    print_array.resize(8);

    GenerateRandomValues(outfile);
    OpenFiles(outfile, infile);
    // controls how many runs of the program we do
    for (int i = 0; i < 2; ++i) {
        // controls what hash tables we insert to
        for (int j = 0, m = 2; j < 2; ++j, ++m) {
            // read from input file and insert all values read to the two test hash tables
            // all while accumulating probes for each insertion
            for (int k = 0; k < table_array[j].load_factor; ++k) {
                infile >> key;
                probe_count_linear += table_array[j].InsertinTable(key, true);
                probe_count_double += table_array[m].InsertinTable(key, false);
            }
            probe_count.push_back(std::make_pair(probe_count_linear/float(table_array[i].load_factor), probe_count_double/float(table_array[i+2].load_factor)));
            probe_count_linear = 0;
            probe_count_double = 0;
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
    srand(time(NULL));
    out.open(".htrvalues.dat");
    for (int i = 0; i != 1000; ++i)
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
void Print_Tables(std::ofstream& outf, std::vector<Hash>& tables, std::vector<std::pair<float, float>>& probes, int load_percents[]) {
    outf << std::setprecision(3) << std::fixed << std::setw(21) << "Description" << std::setw(33) << "Avg. Probes #1" << std::setw(20) << "Avg. Probes #2" << std::setw(20) << "Avg. of 2 runs\n" 
        << "----------------------------------------------------------------------------------------------\n";
        for (int i = 0; i < 2; ++i) {
            outf << std::setw(16) << "Linear Probing, " << std::setw(2) << load_percents[i] << std::setw(12) << " load factor" << std::setw(20) << probes[i].first << std::setw(20) << probes[i+2].first << std::setw(20) << (probes[i].first + probes[i+2].first)/2 << '\n'
                 << std::setw(16) << "Double Hashing, " << std::setw(2) << load_percents[i] << std::setw(12) << " load factor" << std::setw(20) << probes[i].second << std::setw(20) << probes[i+2].second << std::setw(20) << (probes[i].second + probes[i+2].second)/2 << '\n';
        }
        outf << "\n\n***********************************************************************************************\n"
            << "                                            Run 1                                              \n"
            << "***********************************************************************************************\n";
        for (int i = 0; i < 8; i+=4) {
            for (int j = i, m = 0; j < 8, m < 2; ++j, ++m) {
                outf << "----------------------------------------------------------------------------------------------n\n"
                << "Hash Table: Linear Probing, " << load_percents[m] << " load factor\n"
                << "----------------------------------------------------------------------------------------------\n"
                << "|Index|Value|\n"
                << "|-----|-----|\n";

                // Print the  hash table's contents
                tables[j].Print_Table(outf);
            }
            for (int k = i+2, m = 0; k < 8, m < 2; ++k, ++m) {
                outf << "----------------------------------------------------------------------------------------------\n\n"
                << "Hash Table: Double Hashing, " << load_percents[m] << " load factor\n"
                << "----------------------------------------------------------------------------------------------\n"
                << "|Index|Value|\n"
                << "|-----|-----|\n";

                // Print the  hash table's contents
                tables[k].Print_Table(outf);
            }
        outf << "***********************************************************************************************\n"
            << "                                             Run 2                                            \n"
            << "***********************************************************************************************\n";
        }
}
