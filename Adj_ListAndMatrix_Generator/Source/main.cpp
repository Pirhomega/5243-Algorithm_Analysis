#include<iostream>
#include<iomanip>
#include<fstream>
#include<string>
#include<vector>
#include<time.h>

// DON'T FORGET TO REMOVE THIS LINE
using namespace std;

int findNumVerts(vector<pair<int, int>>&, string);
void resizeAndInitialize(vector<vector<int>>&, vector<vector<int>>&, int);
void processGraph(vector<vector<int>>&, vector<vector<int>>&, vector<pair<int, int>>&);
void printList(vector<vector<int>>&, string);
void printMatrix(vector<vector<int>>& structure, string outputFile);

const char buff[4] = {'|', '/', '-', '\\'};

int main() {
    vector<vector<int>> adjList, adjMatrix;
    vector<pair<int, int>> file_contents;
    string infile_name = "Input1.txt", outfile_name = "Output.txt";
    // for (int i = 0; i < 1; ++i) {
        // cout << "Type infile and outfile names separated by one space (e.g. input.txt output.txt).\nThen, press enter to confirm: ";
        // cin >> infile_name >> outfile_name;
        resizeAndInitialize(adjList, adjMatrix, findNumVerts(file_contents, infile_name)+1);
        cout << "Adjacency List and Matrix ready\n";
        // for (int i = 0; i < 51; ++i)
        //     cout << file_contents[i].first << " " << file_contents[i].second << '\n';
        processGraph(adjList, adjMatrix, file_contents);
        cout << "Adjacency List and Matrix populated\nPrinting.";
        // printList(adjList, outfile_name);
        printMatrix(adjMatrix, outfile_name);
    // }
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
int findNumVerts(vector<pair<int, int>>& edges, string infileName) {
    int vert1, vert2, larg=-1;
    ifstream infile(infileName);
    while (infile >> vert1 >> vert2) {
        // we push the edges to 'edges' so we don't have to re-read
        // from the input file when we perform the adjacency list- / matrix-forming
        // algorithm in main
        edges.push_back(make_pair(vert1, vert2));
        if (vert1 > larg)
            larg = vert1;
        if (vert2 > larg)
            larg = vert2;
    }
        infile.close();
    return larg;
}

/*
    This simple function resizes the adjacency list and matrix to the largest vertex number
    found in 'findNumVerts'. Size has been increased by 1 (in the function call) so our 
    vertices can be labelled from 1 to N instead of 0 to N-1
*/
void resizeAndInitialize(vector<vector<int>>& list, vector<vector<int>>& matrix, int size) {
    list.resize(size);
    matrix.resize(size, vector<int>(size));
    for (auto& i : matrix)
        for (auto& j : i)
           j = 0;
}

/*
    This function will read one edge at a time from the edge vector 'edges' and
    input each edge into the adjacency list and matrix.
*/
void processGraph(vector<vector<int>>& list, vector<vector<int>>& matrix, vector<pair<int, int>>& edges) {
    // remember that each edges is represented as two vertices (e.g. 1 2 is an edge between vertex 1 and 2)
    for (auto& edge : edges) {
        // push to the edge.first vertex the edge.second vertex. For example, if the edge is 1 2,
        // this will push to row 1 of 'list' the vertex 2
        list[edge.first].push_back(edge.second);
        // Since the graph is undirected, we need to push the inverse of the above instruction (i.e.
        // push vertex 1 to row 2 of 'list') since an edge from vertex1 to vertex2 is also an edge
        // from vertex2 to vertex1.
        list[edge.second].push_back(edge.first);
        // do the exact same as the above two instructions for the adjacency matrix
        matrix[edge.first][edge.second] = matrix[edge.second][edge.first] = 1;
    }
}

/*
    Prints the adjacency list to an output file
*/
void printList(vector<vector<int>>& structure, string outputFile) {
    int vertex = 0;
    ofstream outfile(outputFile);
    outfile << "Vertices     Adjacent\n";
    for (auto& i : structure) {
        outfile << setw(6) << vertex << "       ";
        for (auto& j : i)
           outfile << j << ' ';
        outfile << '\n';
        vertex++;
    }
    outfile.close();
}

/*
    Prints the adjacency matrix to an output file
*/
void printMatrix(vector<vector<int>>& structure, string outputFile) {
    int row = 0, count = 0, size = structure.size(), size2 = size * size;
    clock_t t1, t2, ela = 0, time;
    ofstream outfile(outputFile);

    outfile << setw(6) << ' ';
    for (int i = 0; i < size; ++i)
        outfile << setw(6) << i;
    outfile << '\n';

    for (auto& i : structure) {
        outfile << setw(6) << row;
        for (auto& j : i) {
            t1 = clock();
            outfile << setw(6) << j;
            t2 = clock()-t1;
            ela += t2;
            time = (t2 * size2) - ela;
            if (!(count % 100000))
                cout << '\r' << buff[count%4]<< ' ' << (float(row+1)/size2)*10 << "% complete    Time left: " << time/1000 << flush;
            count++;
        }
        outfile << '\n';
        row++;
    }
    outfile.close();
}