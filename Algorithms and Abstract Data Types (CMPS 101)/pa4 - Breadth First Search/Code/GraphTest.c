// Name: Leonid Shuster, ID #1616143
// CruzID: lshuster
// Assignment name: pa4
// Role: Test client for graph ADT

#include<stdio.h>
#include<stdlib.h>
#include"Graph.h"

int main(int argc, char* argv[]) {
    Graph A = newGraph(6);
    printGraph(stdout, A);

    List testList = newList();

    fprintf(stdout, "Order: %d\n", getOrder(A));
    fprintf(stdout, "Size: %d\n", getSize(A));
    fprintf(stdout, "Source: %d\n", getSource(A));
    fprintf(stdout, "Parent of %d: %d\n", 1, getParent(A, 1));
    fprintf(stdout, "Distance from %d to %d: %d\n", getSource(A), 2, getDist(A, 2));
    fprintf(stdout, "\n");

    addEdge(A, 1, 2);
    addEdge(A, 1, 3);
    addEdge(A, 2, 4);
    addEdge(A, 2, 5);
    addEdge(A, 2, 6);
    addEdge(A, 3, 4);
    addEdge(A, 4, 5);
    addEdge(A, 5, 6);

    printGraph(stdout, A);

    BFS(A, 3);

    fprintf(stdout, "Source: %d\n", getSource(A));

    fprintf(stdout, "Size: %d\n", getSize(A));
    fprintf(stdout, "Parent of %d: %d\n", 5, getParent(A, 5));
    fprintf(stdout, "Distance from %d to %d: %d\n", getSource(A), 2, getDist(A, 2));
    fprintf(stdout, "Path from %d to %d: ", getSource(A), 6);
    getPath(testList, A, 6);
    printList(stdout, testList);
    fprintf(stdout, "\n");
    clear(testList);
    fprintf(stdout, "\n");

    addArc(A, 1, 6);
    printGraph(stdout, A);

    makeNull(A);
    printGraph(stdout, A);

    freeList(&testList);

    return (0);
}