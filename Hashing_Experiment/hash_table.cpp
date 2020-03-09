#include "hash_table.h"

Hash::Hash(int size) {
    hash_table_size = size;
    hash_table = new std::pair<int, bool>[hash_table_size];
    for (int i = 0; i < hash_table_size; ++i)
        hash_table[i] = std::make_pair(0,false);
}

Hash::~Hash() {
    delete [] hash_table;
    hash_table = NULL;
}

int Hash::InsertinTable(int key_value, bool probe) {
    int location = key_value % hash_table_size, probeCount = 1;
    // if probe is true, linear probing will be used as the CRP.
    // If probe is false, double hashing will be used
    if (probe) {
        while (hash_table[location].first) {
        // modding by hash_table_size loops location back to the beginning
        // of the hash_table to prevent out-of-bounds indexing
            location = ++location % hash_table_size;
            probeCount++;
        }
    } else {
        while (hash_table[location].first) {
            // this instruction is a combo of the following two:
            //    increment = (last digit of key) + 1
            //    location += increment
            location = (location + (key_value - (key_value / 10) * 10) + 1) % hash_table_size;
            probeCount++;
        }
    }
    hash_table[location].first = key_value;
    return probeCount;
}

// int Hash::Mod_Hash(int key_value, int sim_table_size) {
    
// }

// int Hash::Lin_Probe(int key_value, int original_location, int table_size) {

// }

// int Hash::Double_Probe(int key_value, int original_location, int table_size) {
    
// }
void Hash::Print_Table(std::ofstream& outf) {
    for (int i = 0;i < hash_table_size;++i)
        outf << "Index: " << i << "   Value: " << hash_table[i].first << '\n';
}