#pragma once
#include<iostream>
#include<fstream>
#include<utility>

class Hash {
    // cell will be a two-part structure of which the hash table,
    // 'hash_table', will be constructed
    // struct cell {
    //     int key;
    //     bool erased;

    //     cell() {
    //         key = 0;
    //         erased = false;
    //     }
    // };
public:
    int hash_table_size;
    std::pair<int, bool>* hash_table = NULL;
public:
    Hash(int);
    ~Hash();
    int InsertinTable(int, bool);
    // int Mod_Hash(int, int);
    // int Lin_Probe(int, int, int);
    // int Double_Probe(int, int, int);
    void Print_Table(std::ofstream&);
};