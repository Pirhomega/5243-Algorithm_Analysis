/*
	5243 - Adv. Algorithm Analysis
	Dr. Halverson, Spring 20
	Hashing Experiment - Head to head: linear probing versus double hashing
	Corbin Matamoros 
*/

#pragma once
#include<iostream>
#include<iomanip>
#include<fstream>
#include<utility>

// Hash is an object representing a hash table
class Hash {
public:
    // hash_table_size is the size of the hash table
    // load_factor_count is the number of keys able to be mapped
    //      to the table. It is derived from the load factor
    //      equation: load_factor = load_factor_count / table_size
    // n is the number of keys mapped in the hash table at any given moment
    // hash_table is a pointer to an array of integers
    // 
    int hash_table_size, load_factor_count, n;
    int* hash_table = nullptr;
public:
    Hash();
    Hash(int, int);
    Hash(const Hash&);
    Hash& operator=(const Hash&);
    ~Hash();
    int InsertinTable(int, bool);
    void Print_Table(std::ofstream&);
    void Reset_Table();
};