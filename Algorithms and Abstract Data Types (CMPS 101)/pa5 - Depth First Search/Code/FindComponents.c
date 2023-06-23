// Name: Leonid Shuster, ID #1616143
// CruzID: lshuster
// Assignment name: pa5
// Role: Reads in file and creates graph, adds arcs, runs DFS on a graph, and prints out all strongly connected components

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
        addArc(graph, u, v); // Create arc between two vertices
    }

    // Print out graph
    fprintf(out, "Adjacency list representation of G:\n");
    printGraph(out, graph);
    fprintf(out, "\n");

    List stack = newList(); // List for stack

    // Adds vertices to stack to make it the same size as the order of the graph
    for (int i = 1; i < getOrder(graph) + 1; i++) {
        append(stack, i);
    }

    DFS(graph, stack); // Run DFS on graph, return stack

    Graph transposedGraph = transpose(graph); // Transpose graph

    DFS(transposedGraph, stack); // Run DFS on transposed graph, uses same stack and organizes the stack by vertices of decreasing finish times

    // Get the number of strongly connected components by counting number of elements with NIL parents
    int scc = 0;
    for (int i = 1; i < getOrder(transposedGraph) + 1; i++) {
        if (getParent(transposedGraph, i) == NIL){
            scc++;
        }
    }
    fprintf(out, "G contains %d strongly connected components:\n", scc);

    List tempList = newList(); // Temporary list that holds a single strongly connected component to be printed

    // Get each strongly connected component by starting from the back of the stack and adding elements into a temporary list (popping from the stack) until an element has no parent
    moveBack(stack);
    int component = 1;
    while (index(stack) != -1) {
        prepend(tempList, get(stack)); // Add to temporary list

        // Keep adding elements until an element has no parent, then clear the temporary list and continue adding elements of a new strongly connected component
        if (getParent(transposedGraph, get(stack)) == NIL) {
            fprintf(out, "Component %d: ", component);
            printList(out, tempList);
            fprintf(out,"\n");
            clear(tempList);
            component++;
        }
        movePrev(stack);
    }
    moveFront(stack);

    // Free memory
    freeGraph(&graph);
    graph = NULL;

    freeGraph(&transposedGraph);
    transposedGraph = NULL;

    freeList(&stack);
    stack = NULL;

    freeList(&tempList);
    tempList = NULL;



    // close input and output files
    fclose(in);
    fclose(out);

    return(EXIT_SUCCESS);


}