/*
	5243 - Adv. Algorithm Analysis
	Dr. Halverson, Spring 20
	Hashing Experiment - Head to head: linear probing versus double hashing
	Corbin Matamoros
	This experiment will be used to compare the efficiencies of linear probing
    and double hashing. Hash tables will be implemented as dynamically allocated
    arrays of size 311 and load factors of either 0.66 or 0.8. Keys will be inserted
    into each table and the number of probes to insert will be documented. The sum
    of all probes will then be used to compare the two CRPs methodologies. The winner
    gets a lollipop and a case of beer from a certain Mexican company whose name
    bears no resemblence to a virus.

    To speed things up a bit, we will combine two runs into one program. This means
    the vector holding the four hash tables to be tested on will be formatted
    for the second run. Details are in the code.
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
    // counter will be used to index print_array when saving the states of a run's hash tables
    // probe_count_linear and probe_count_double will accumulate the table's probes over all insertions
    //          for the linear probing and double hashing CRPs, respectively.
    // linear_probe_66 through double_80 are hash tables. Linear_* CRP is linear probing and double_* is double hashing
    // table_array will store hash tables for ease of access
    // print_array will store all FULL hash tables. Once all tables have been filled, print_array will be sent
    //          to a function for hash table printing
    // probe_count will hold the average number of probes for each hash table. Data will be stored like this:
    //                       run 1  (probe_average_linear_66, probe_average_double_66)
    //                              (probe_average_linear_80, probe_average_double_80)
    //                       run 2  (probe_average_linear_66, probe_average_double_66)
    //                              (probe_average_linear_80, probe_average_double_80)
    int key, counter = 0, probe_count_linear = 0, probe_count_double = 0, load_percentage[2] = {66, 80};
    Hash linear_probe_66(311, 205), linear_probe_80(311, 250), double_66(311, 205), double_80(311, 250);
    std::vector<Hash> table_array, print_array;
    std::vector<std::pair<float, float>> probe_count;
    std::ifstream infile; std::ofstream outfile;

    // push list of hash tables to table_array
    // Two runs will be performed on these hash tables, and in between runs, 
    // these hash tables will be emptied of keys.
    table_array.push_back(linear_probe_66);
    table_array.push_back(linear_probe_80);
    table_array.push_back(double_66);
    table_array.push_back(double_80);

    print_array.resize(8);

    OpenFiles(outfile, infile);
    // controls how many runs of the program we do
    for (int i = 0; i < 2; ++i) {
        // controls what hash tables we insert to
        // j will let us access the 0.66 load factor hash tables, and
        // k, the 0.8. This way, we are inserting keys into two same-sized
        // tables with different CRPs.
        for (int j = 0, m = 2; j < 2; ++j, ++m) {
            // read from input file and insert all values read to the two hash tables
            // all while accumulating probes for each insertion
            for (int k = 0; k < table_array[j].load_factor_count; ++k) {
                infile >> key;
                probe_count_linear += table_array[j].InsertinTable(key, true);
                probe_count_double += table_array[m].InsertinTable(key, false);
            }
            // save and reset the state of the probe_count variables for the next pair of hash tables
            probe_count.push_back(std::make_pair(probe_count_linear/float(table_array[i].load_factor_count), probe_count_double/float(table_array[i+2].load_factor_count)));
            probe_count_linear = 0;
            probe_count_double = 0;
        }
        // push all tables to a vector of hash tables and reset the old hash tables for 
        // the next run
        for (int i = 0; i < 4; ++i) {
            print_array[counter] = table_array[i];
            table_array[i].Reset_Table();
            counter++;
        }
    }
    Print_Tables(outfile, print_array, probe_count, load_percentage);
    infile.close(); outfile.close();
    return 0;
}

// OpenFiles is the typical input- / output-opening function. However, in this case, the programmer
// can choose to use the file of data values provided in .htrvalues.dat or use a 
// different file
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
// At the top of the output file, the programmer's name and a table of probe averages per hash table will be
// displayed. Both runs' average probes will be averaged for each hash table, as well.
void Print_Tables(std::ofstream& outf, std::vector<Hash>& tables, std::vector<std::pair<float, float>>& probes, int load_percents[]) {
    outf << "Corbin Matamoros\n\n" << 
        std::setprecision(3) << std::fixed << std::setw(21) << "Description" << std::setw(33) << "Avg. Probes #1" << std::setw(20) << "Avg. Probes #2" << std::setw(20) << "Avg. of 2 runs\n" 
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
                outf << "----------------------------------------------------------------------------------------------\n"
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

                tables[k].Print_Table(outf);
            }
        outf << "***********************************************************************************************\n"
            << "                                             Run 2                                            \n"
            << "***********************************************************************************************\n";
        }
}