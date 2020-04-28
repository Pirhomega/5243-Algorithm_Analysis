/*
    Author: Corbin Matamoros
    Class: 5243 Adv Algorithms
    Professor: Dr. Halverson
    Date: 4/21/2020
    Program Description:
        Program will process an undirected graph in the form of its set of edges and
        represent it as an adjacency list and matrix. Edges must be organized
        as "vertex1 vertex2", meaning there is an edge between 'vertex1' and
        'vertex2'. For ease of reading, the adjacency matrix has had all zeros
        replaced with '-'.

        Note:   The code will place variables within the scope of their use if possible,
                meaning plenty of declarations will be within for loops. Although this
                may seem pointless, arguments can be made that it improves program
                compilation and resource-usage efficiency 
                (https://stackoverflow.com/questions/7959573/declaring-variables-inside-loops-good-practice-or-bad-practice)
*/

#include<iostream>
#include<iomanip>
#include<fstream>
#include<string>
#include<vector>
#include<utility>
#include<unordered_map>
#include<algorithm>
#include<time.h>

using namespace std;

int findNumVerts(vector<pair<string, string> >&, unordered_map<string,int>&, vector<string>&, string);
void resizeAndInitialize(vector<vector<string> >&, vector<vector<string> >&, int);

void processList(vector<vector<string> >&, vector<pair<string, string> >&, unordered_map<string,int>&);
void processMatrix(vector<vector<string> >&, vector<pair<string, string> >&, unordered_map<string,int>&);

void printList(vector<vector<string> >&, vector<string>&, unordered_map<string,int>&, ofstream&, string);
void printMatrix(vector<vector<string> >&, vector<string>&, unordered_map<string,int>&, ofstream&, string);

int main() {
    int inCount = 0;
    ofstream outList, outMatrix;
    outList.open("../Output_Files/OutputAdjLists.txt"); outMatrix.open("../Output_Files/OutputAdjMatrix.txt");

    cout << "State number of input sets to run: "; cin >> inCount;
    // allow the program to run 'inCount' times for user's input sets
    for (int i = 0; i < inCount; ++i) {
        cout << '\n';
        // 'adjList' and 'adjMatrix' are a 2D-vector adjacency list and matrix, respectively.
        // 'file_contents' is a vectors of pairs designed to hold the entirety of an input file's
        //          edge list (i.e. an edge between vertex u and v will be stored as 'make_pair(u,v)')
        // 'verts' will hold all unique vertices from the input file. Its size is the number of vertices in the graph
        //      Now, 'verts' will unordered_map strings to integers, and the integers will provide means of indexing the list
        //      and matrix (vectors need to be indexed by integers).
        // 'indices' will be a list of unique vertices in the graph
        // 'infile_name', 'outfile_name', and 'outfile' will be used for file stream input and output
        vector<vector<string> > adjList, adjMatrix;
        vector<pair<string, string> > file_contents;
        unordered_map<string,int > verts;
        vector<string> indices;
        string infile_name;

        cout << "Type infile and outfile names separated by one space (e.g. input.txt output.txt).\nThen, press enter to confirm: ";
        cin >> infile_name;

        // configure list and matrix sizes to fit # of vertices in a graph
        // We add 1 to the return value of 'findNumVerts' so the list and matrix
        //      will be the correct size. Since 'count' in 'findNumVerts' is initialized at 0
        //      the size of the list and matrix needs to be one larger than the largest index
        //      in both (i.e. an array with indices 0, 1, and 2 is size 3)
        resizeAndInitialize(adjList, adjMatrix, findNumVerts(file_contents, verts, indices, infile_name)+1);

        // load the edges into the list and matrix
        processList(adjList, file_contents, verts);
        processMatrix(adjMatrix, file_contents, verts);

        // sort the list of unique vertices for use in printing the adjacency list in order
        // To print in lexographical order, we sort the vector 'indices'. Then, iterate through 'indices'.
        // At each element in 'indices', we look at the vertex 'u' stored there and find its MAPPED index in 'verts'.
        // We take that mapped index and iterate through that row in adjList (and adjMatrix). This then prints
        // the list and matrix in order
        sort(indices.begin(),indices.end());

        // print out the contents of the list and matrix (not recommended for large graphs)
        printList(adjList, indices, verts, outList, infile_name);
        printMatrix(adjMatrix, indices, verts, outMatrix, infile_name);
    }
    outList.close();
    outMatrix.close();
    return 0;
}

/*
    findNumVerts will read from an input file a set of edges in a graph,
    represented as pairs of vertices separated by whitespace (e.g. an edge 
    from vertex 1 to 2 will look like this in the input file: "1 2"). 
    As the edges are read in, the largest vertex is tracked. In the end,
    the largest vertex will be sent back to main to be used to size the 
    adjacency list and matrix. 
*/
int findNumVerts(vector<pair<string, string> >& edges, unordered_map<string,int>& verts, 
                vector<string>& index, string infileName) {
    // 'u' and 'v' are just containers that will hold the two vertices in an edge
    // 'infile' will open 'infileName' and read from it
    string u, v;
    ifstream infile("../Input_Files/" + infileName);
    int count = -1;
    while (infile >> u >> v) {
        // we push the edges to 'edges' so we don't have to re-read
        // from the input file when we call 'processGraph'
        edges.push_back(make_pair(u, v));
        if (verts.find(u) == verts.end()) {
            count++;
            verts[u] = count;
            index.push_back(u);
        }
        if (verts.find(v) == verts.end()) {
            count++;
            verts[v] = count;
            index.push_back(v);
        }
    }
    infile.close();
    return count;
}

/*
    This simple function resizes the adjacency list and matrix to the largest vertex number
    found in 'findNumVerts'.
*/
void resizeAndInitialize(vector<vector<string> >& list, vector<vector<string> >& matrix, int size) {
    list.resize(size, vector<string>(0));
    matrix.resize(size, vector<string>(size));
    for (auto& i : matrix)
        for (auto& j : i)
           j = '-';
}

/*
    This function will read one edge at a time from the edge vector 'edges' and
    input each edge into the adjacency list and matrix.
*/
void processList(vector<vector<string> >& list, vector<pair<string, string> >& edges, unordered_map<string,int>& verts) {
    // remember that each edge is represented as two vertices (e.g. 1 2 is an edge between vertex 1 and 2)
    for (auto edge : edges) {
        string u = edge.first, v = edge.second;
        // push to the edge.first vertex the edge.second vertex. For example, if the edge (u,v) is (1,2),
        // this will push to row 1 of 'list' the vertex 2
        list[verts[u]].push_back(v);
        // Since the graph is undirected, we need to push the inverse of the above instruction (i.e.
        // push vertex 1 to row 2 of 'list') since an edge from vertex1 to vertex2 is also an edge
        // from vertex2 to vertex1.
        list[verts[v]].push_back(u);
    }
}

/*
    This function will read one edge at a time from the edge vector 'edges' and
    input each edge into the adjacency list and matrix.
*/
void processMatrix(vector<vector<string> >& matrix, vector<pair<string, string> >& edges, unordered_map<string,int>& verts) {
    // remember that each edge is represented as two vertices (e.g. 1 2 is an edge between vertex 1 and 2)
    for (auto edge : edges) {
        string u = edge.first, v = edge.second;
        // do the exact same as the above two instructions for the adjacency matrix
        matrix[verts[u]][verts[v]] = matrix[verts[v]][verts[u]] = '1';
    }
}

/*
    Prints the adjacency list to an output file
*/
void printList(vector<vector<string> >& list, vector<string>& index, unordered_map<string,int>& verts, ofstream& outfile, string infile_name) {
    outfile << infile_name << "\n-----------------------------------\nVertices     Adjacent\n";
    // 'row' will iterate "down" the adjacency list
    for (int i = 0; i < list.size(); ++i) {
        // verts[index[i]] returns the row in 'list' the vertex lies in
        outfile << setw(6) << index[i] << "       ";
        // 'adj' iterates through index[i]'s MAPPED index, found in 'verts'
        for (auto adj : list[verts[index[i]]])
           outfile << adj << ' ';
        outfile << '\n';
    }
    outfile << "\n-----------------------------------\n";
}

/*
    Prints the adjacency matrix to an output file
*/
void printMatrix(vector<vector<string> >& matrix, vector<string>& index, unordered_map<string,int>& verts, ofstream& outfile, string infile_name) {
    // 't0', 't1', 't2', 'ela', 'numVerts', and 'seconds' are used to track printing progress by printing
    //          percentage of print job completed and approximate time left till completed.
    //          It's something experimental I wanted to try out, and it works well for 
    //          large graphs (|V| > 5,000)
    // 'numVerts' is also used to print the matrix column labels in output file
    clock_t t0;
    int numVerts = matrix.size(); 

    outfile << infile_name << "\n--------------------------------------------------------------\n";

    outfile << setw(7) << ' ';
    for (auto& i : index)
        outfile << setw(6) << i;
    outfile << '\n' << setw(7) << ' ';
    for (int i = 0; i < numVerts*6; ++i)
        outfile << '-';
    outfile << '\n';

    t0 = clock();

    // begin printing matrix content
    for (int i = 0; i < numVerts; ++i) {
        clock_t t1 = clock(), ela = t1 - t0, t2;
        double seconds;

        // Actual printing part of the 'for' block.
        // The rest is "how long till program 
        //        finishes" code
        /*****************************************/
        outfile << setw(6) << index[i] << setw(1) << '|';
        for (int j = 0; j < numVerts; ++j)
            outfile << setw(6) << matrix[verts[index[i]]][verts[index[j]]];
        outfile << '\n';
        /*****************************************/
        // Time remaining is [(time to process one row) times (total number of rows)] minus [elapsed time]
        t2 = clock() - t1;
        seconds = (t2 * numVerts) - ela;
        // update the percentage complete and time remaining once every 500 rows
        if (!(i % 500)) 
            cout << '\r' << setw(6) << setprecision(2) << fixed << (i*100.0)/numVerts << setw(10) << "% complete" << setw(13)
                    << "Time left: " << setw(6) << float(seconds)/CLOCKS_PER_SEC << setw(8) << " seconds" << flush;
    }
    cout << "\nTime to complete: " << (float(clock()) - float(t0))/1000 << " seconds\n";
    outfile << "\n--------------------------------------------------------------\n";
}