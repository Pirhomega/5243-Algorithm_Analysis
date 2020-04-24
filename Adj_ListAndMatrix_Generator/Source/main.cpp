/*
    Author: Corbin Matamoros
    Class: 5243 Adv Algorithms
    Professor: Dr. Halverson
    Date: 4/21/2020
    Program Description:
        Program will process an undirected graph in the form of its set of edges and
        represent it as an adjacency list and matrix. Edges must be organized
        as "vertex1 vertex2", meaning there is an edge between 'vertex1' and
        'vertex2'.

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
#include<time.h>

int findNumVerts(std::vector<std::pair<std::string, std::string> >&, std::unordered_map<std::string,int>&, std::unordered_map<int,std::string >&, std::string);
void resizeAndInitialize(std::vector<std::vector<std::string> >&, std::vector<std::vector<int> >&, int);
void processGraph(std::vector<std::vector<std::string> >&, std::vector<std::vector<int> >&, 
                    std::vector<std::pair<std::string, std::string> >&, std::unordered_map<std::string,int>&);
void printList(std::vector<std::vector<std::string> >&, std::unordered_map<int,std::string >&, std::ofstream&);
void printMatrix(std::vector<std::vector<int> >&, std::unordered_map<int,std::string >&, std::ofstream&);

int main() {
    int inCount = 0;
    std::cout << "State number of input sets to run: "; std::cin >> inCount;
    // allow the program to run 'inCount' times for user's input sets
    for (int i = 0; i < inCount; ++i) {
        std::cout << '\n';
        // 'adjList' and 'adjMatrix' are a 2D-std::vector adjacency list and matrix, respectively.
        // 'file_contents' is a vectors of pairs designed to hold the entirety of an input file's
        //          edge list (i.e. an edge between vertex u and v will be stored as 'std::make_pair(u,v)')
        // 'verts' will hold all unique vertices from the input file. Its size is the number of vertices in the graph
        //      Now, 'verts' will map strings to integers, and the integers will provide means of indexing the list
        //      and matrix (vectors need to be indexed by integers).
        // 'indices' is exactly the same  as 'verts', but flipped. This is used in the printing functions.
        // 'infile_name', 'outfile_name', and 'outfile' will be used for file stream input and output
        std::vector<std::vector<std::string> > adjList; 
        std::vector<std::vector<int> > adjMatrix;
        std::vector<std::pair<std::string, std::string> > file_contents;
        std::unordered_map<std::string,int > verts;
        std::unordered_map<int,std::string > indices;
        std::string infile_name, outfile_name; std::ofstream outfile;

        std::cout << "Type infile and outfile names separated by one space (e.g. input.txt output.txt).\nThen, press enter to confirm: ";
        std::cin >> infile_name >> outfile_name; outfile.open(outfile_name);

        // configure list and matrix sizes to fit # of vertices in a graph
        resizeAndInitialize(adjList, adjMatrix, findNumVerts(file_contents, verts, indices, infile_name));
        for (auto& i : adjList) {
            for (auto& j : i)
                std::cout << j << ' ';
        }

        // load the edges into the list and matrix
        processGraph(adjList, adjMatrix, file_contents, verts);

        // print out the contents of the list and matrix (not recommended for large graphs)
        printList(adjList, indices, outfile);
        printMatrix(adjMatrix, indices, outfile);

        outfile.close();
    }
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
int findNumVerts(std::vector<std::pair<std::string, std::string> >& edges, std::unordered_map<std::string,int>& verts, 
                std::unordered_map<int,std::string >& index, std::string infileName) {
    // 'u' and 'v' are just containers that will hold the two vertices in an edge
    // 'infile' will open 'infileName' and read from it
    std::string u, v;
    std::ifstream infile(infileName);
    int count = 1;
    while (infile >> u >> v) {
        // we push the edges to 'edges' so we don't have to re-read
        // from the input file when we call 'processGraph'
        edges.push_back(std::make_pair(u, v));
        if (!verts[u]) {
            verts[u] = count;
            index[count] = u;
            std::cout << "Found " << u << '\n';
            count++;
        }
        if (!verts[v]) {
            verts[v] = count;
            index[count] = v;
            std::cout << "Found " << v << '\n';
            count++;
        }
    }
    infile.close();
    std::cout << count << '\n';
    return count;
}

/*
    This simple function resizes the adjacency list and matrix to the largest vertex number
    found in 'findNumVerts'.
*/
void resizeAndInitialize(std::vector<std::vector<std::string> >& list, std::vector<std::vector<int> >& matrix, int size) {
    list.resize(size, std::vector<std::string>(0));
    matrix.resize(size, std::vector<int>(size));
    for (auto& i : matrix)
        for (auto& j : i)
           j = 0;
}

/*
    This function will read one edge at a time from the edge std::vector 'edges' and
    input each edge into the adjacency list and matrix.
*/
void processGraph(std::vector<std::vector<std::string> >& list, std::vector<std::vector<int> >& matrix, 
        std::vector<std::pair<std::string, std::string> >& edges, std::unordered_map<std::string,int>& verts) {
    // remember that each edge is represented as two vertices (e.g. 1 2 is an edge between vertex 1 and 2)
    for (auto edge : edges) {
        std::string u = edge.first, v = edge.second;
        // push to the edge.first vertex the edge.second vertex. For example, if the edge (u,v) is (1,2),
        // this will push to row 1 of 'list' the vertex 2
        list[verts[u]].push_back(v);
        // Since the graph is undirected, we need to push the inverse of the above instruction (i.e.
        // push vertex 1 to row 2 of 'list') since an edge from vertex1 to vertex2 is also an edge
        // from vertex2 to vertex1.
        list[verts[v]].push_back(u);
        // do the exact same as the above two instructions for the adjacency matrix
        matrix[verts[u]][verts[v]] = matrix[verts[v]][verts[u]] = 1;
    }
}

/*
    Prints the adjacency list to an output file
*/
void printList(std::vector<std::vector<std::string> >& list, std::unordered_map<int,std::string >& index, std::ofstream& outfile) {
    // 'vertex' is a generic counter used to label the 'Vertices' column in output file
    int vertex = 0;
    outfile << "Vertices     Adjacent\n";
    // 'row' will iterate "down" the adjacency list
    for (auto& row : list) {
        outfile << std::setw(6) << index[vertex] << "       ";
        // 'adj' iterates through contects of each 'row' for all
        //      vertices adjacent to vertex represented by 'row'
        for (auto& adj : row)
           outfile << adj << ' ';
        outfile << '\n';
        vertex++;
    }
}

/*
    Prints the adjacency matrix to an output file
*/
void printMatrix(std::vector<std::vector<int> >& matrix, std::unordered_map<int,std::string >& index, std::ofstream& outfile) {
    // 't0', 't1', 't2', 'ela', 'numVerts', and 'seconds' are used to track printing progress by printing
    //          percentage of print job completed and approximate time left till completed.
    //          It's something experimental I wanted to try out, and it works well for 
    //          large graphs (|V| > 5,000)
    // 'vertex' is a generic counter used to label the matrix rows in output file
    // 'numVerts' is also used to print the matrix column labels in output file
    clock_t t0;
    int vertex = 0, numVerts = matrix.size(); 

    outfile << std::setw(6) << ' ';
    for (int i = 0; i < numVerts; ++i)
        outfile << std::setw(6) << index[i];
    outfile << '\n';

    t0 = clock();

    for (auto& row : matrix) {
        clock_t t1 = clock(), ela = t1 - t0, t2;
        double seconds;

        // Actual printing part of the 'for' block.
        // The rest is "how long till program 
        //        finishes" code
        /*****************************************/
        outfile << std::setw(6) << index[vertex];
        for (auto& col : row) {
            outfile << std::setw(6) << col;
        }
        outfile << '\n';
        vertex++;
        /*****************************************/
        // Time remaining is [(time to process one row) times (total number of rows)] minus [elapsed time]
        t2 = clock() - t1;
        seconds = (t2 * numVerts) - ela;
        // update the percentage complete and time remaining once every 500 rows
        if (!(vertex % 500))
            std::cout << '\r' << std::setw(6) << std::setprecision(2) << std::fixed << (vertex*100.0)/numVerts << std::setw(10) << "% complete" << std::setw(13)
                    << "Time left: " << std::setw(6) << float(seconds)/CLOCKS_PER_SEC << std::setw(8) << " seconds" << std::flush;
    }
    outfile.close();
}