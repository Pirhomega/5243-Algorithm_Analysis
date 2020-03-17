/*
	5243 - Adv. Algorithm Analysis
	Dr. Halverson, Spring 20
	Hashing Experiment - Head to head: linear probing versus double hashing
	Corbin Matamoros 
*/

#include "hash_table.h"

// default constructor
Hash::Hash() {
    hash_table_size, load_factor_count, n = 0;
}

// Parameterized constructor for a hash table dynamically allocates
// an array of integers of size hash_table_size. All indices are
// initialized to zero. 
Hash::Hash(int size, int lf) {
    hash_table_size = size;
    load_factor_count = lf;
    n = 0;
    hash_table = new int[hash_table_size];
    // initialize entire hash table to 0
    for (int i = 0; i < hash_table_size; ++i)
        hash_table[i] = 0;
}

// copy constructor, which allows hash table object to be
// declared with the same state of another
Hash::Hash(const Hash& other) {
    hash_table_size = other.hash_table_size;
    load_factor_count = other.load_factor_count;
    n = other.n;
    hash_table = new int[hash_table_size];
    for (int i = 0; i < hash_table_size; ++i) {
        hash_table[i] = other.hash_table[i];
    }
}

// assignment operator, which allows Hash objects
// to be assigned to others with the '='. Without this
// function, if one object to assigned to the other,
// pointers in both will point to one set of data.
// If one object were to delete that data set, the other object
// would no longer have access to it, and its pointers dangle.
Hash& Hash::operator=(const Hash& other) {
    hash_table_size = other.hash_table_size;
    load_factor_count = other.load_factor_count;
    n = other.n;
    hash_table = new int[hash_table_size];
    for (int i = 0; i < hash_table_size; ++i) {
        hash_table[i] = other.hash_table[i];
    }
    return *this;
}

// Deconstructor de-allocates the hash table and makes it
// uncallable by pointing it to nullptr
Hash::~Hash() {
    delete [] hash_table;
    hash_table = nullptr;
}

// InsertinTable will insert a value into a hash table with a CRP
// of linear probing or double hashing, depending on the value of 
// 'probe'
int Hash::InsertinTable(int key_value, bool probe) {
    int location = key_value % hash_table_size, probeCount = 0;
    if (n < load_factor_count) {
        probeCount++;
        // if probe is true, linear probing will be used as the CRP.
        // Else, double hashing will be used
        if (probe) {
            // since all hash table indices were initialized to zero,
            // hash_table[location] will either be false (0) or true (when
            // it has a key
            while (hash_table[location]) {
            // modding by hash_table_size loops location back to the beginning
            // of the hash_table to prevent out-of-bounds indexing
                location = ++location % hash_table_size;
                probeCount++;
            }
        } else {
            while (hash_table[location]) {
                // this instruction is a combo of the following two:
                //    increment = (last digit of key) + 1
                //    location += increment
                location = (location + (key_value - (key_value / 10) * 10) + 1) % hash_table_size;
                probeCount++;
            }
        }
        // map key to its spot
        hash_table[location] = key_value;
        n++;
    }
    return probeCount;
}

// Print_Table prints a hash table's contents by printing
// each index and its corresponding value
void Hash::Print_Table(std::ofstream& outf) {
    for (int i = 0 ; i < hash_table_size ; ++i)
        outf <<  "| " << std::setw(3) << i <<  " |" << std::setw(5) << hash_table[i] << "|\n";
}

// resets all spots in a hash table to the value zero and zeros
// out number of keys in table
void Hash::Reset_Table() {
    n = 0;
    for (int i = 0; i < hash_table_size; ++i) {
        hash_table[i] = 0;
    }
}
