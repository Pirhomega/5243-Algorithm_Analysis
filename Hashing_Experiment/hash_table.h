#pragma once
#include<iostream>
#include<iomanip>
#include<fstream>
#include<utility>

// Hash is an object representing a hash table
class Hash {
    // hash_table_size is the size of the hash table
    // hash_table is a pointer to an array of pairs where each pair
    // is the value in the hash table and a boolean. The boolean will
    // be true if that location has had a value deleted from it,
    // false otherwise. This boolean is not utilized in this program.
    int hash_table_size;
    std::pair<int, bool>* hash_table = NULL;
public:
    Hash(int);
    ~Hash();
    int InsertinTable(int, bool);
    void Print_Table(std::ofstream&);
};