// Name: Leonid Shuster, ID #1616143
// CruzID: lshuster
// Assignment name: pa4
// Role: Reads in file and creates graph, adds edges, and prints out distance and path from source to element

// ./FindPath <input file> <output file>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Graph.h"

#define MAX 257

int main(int argc, char* argv[]){

    FILE* in;  // file handle for input
    FILE* out; // file handle for output

    char input[MAX];

    // check command line for correct number of arguments
    if(argc != 3) {
        printf("Usage: %s <input file> <output file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // open input file for reading
    in = fopen(argv[1], "r");
    if(in == NULL) {
        printf("Unable to read from file %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // open output file for writing
    out = fopen(argv[2], "w");
    if(out == NULL) {
        printf("Unable to write to file %s\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    fgets(input, MAX, in); // Get first line for order of graph

    Graph graph = newGraph(atoi(input));

    while (fgets(input, MAX, in) != NULL) { // Loop first part of file to get edges, go until 0 0
        int u = atoi(strtok(input, " ")); // Get first vertex, convert to number
        int v = atoi(strtok(NULL, " ")); // Get second vertex, convert to number

        if (u == 0 && v == 0) { // Loop until 0 0
            break;
        }
        addEdge(graph, u, v); // Create edge between two vertices
    }
    printGraph(out, graph);
    fprintf(out, "\n");

    List list = newList(); // List for getPath
    while (fgets(input, MAX, in) != NULL) { // Loop until second part of file to get source and destination, go until 0 0
        int source = atoi(strtok(input, " ")); // Get source, convert to number
        int dest = atoi(strtok(NULL, " ")); // Get destination, convert to number

        if (source == 0 && dest == 0) { // Loop until 0 0
            break;
        }
        BFS(graph, source); // Run BFS to get a new source, parents, and distance every time

        
    }

    // Free lists
    freeList(&list);
    list = NULL;

    freeGraph(&graph);
    graph = NULL;

    // close input and output files
    fclose(in);
    fclose(out);

    return(EXIT_SUCCESS);


}