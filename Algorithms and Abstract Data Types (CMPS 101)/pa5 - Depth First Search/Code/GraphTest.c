// Name: Leonid Shuster, ID #1616143
// CruzID: lshuster
// Assignment name: pa5
// Role: Test client for graph ADT

#include<stdio.h>
#include<stdlib.h>
#include"Graph.h"

int main(int argc, char* argv[]) {
    Graph A = newGraph(8);

    fprintf(stdout, "Empty Graph: \n");
    printGraph(stdout, A);


    fprintf(stdout, "Order: %d\n", getOrder(A));
    fprintf(stdout, "Size: %d\n", getSize(A));
    fprintf(stdout, "Parent of %d: %d\n", 1, getParent(A, 1));
    fprintf(stdout, "Discover time of %d: %d\n", 1, getDiscover(A, 1));
    fprintf(stdout, "Finish time of %d: %d\n", 1, getFinish(A, 1));
    fprintf(stdout, "\n");

    addArc(A, 1, 2);
    addArc(A, 2, 3);
    addArc(A, 2, 5);
    addArc(A, 2, 6);
    addArc(A, 3, 4);
    addArc(A, 3, 7);
    addArc(A, 4, 3);
    addArc(A, 4, 8);
    addArc(A, 5, 1);
    addArc(A, 5, 6);
    addArc(A, 6, 7);
    addArc(A, 7, 6);
    addArc(A, 7, 8);
    addArc(A, 8, 8);

    fprintf(stdout, "Filled in Graph: \n");
    printGraph(stdout, A);

    List testStack = newList();

    for (int i = 1; i < getOrder(A) + 1; i++) {
        append(testStack, i);
    }

    DFS(A, testStack);

    fprintf(stdout, "Size: %d\n", getSize(A));
    fprintf(stdout, "Parent of %d: %d\n", 5, getParent(A, 5));
    fprintf(stdout, "Discover time of %d: %d\n", 3, getDiscover(A, 3));
    fprintf(stdout, "Finish time of %d: %d\n", 5, getFinish(A, 5));
    fprintf(stdout, "\n");

    Graph T = transpose(A);

    fprintf(stdout, "Transposed Graph: \n");
    printGraph(stdout, T);
    fprintf(stdout, "\n");

    DFS(T, testStack);

    List tempList = newList();

    moveBack(testStack);
    int component = 1;
    while (index(testStack) != -1) {
        prepend(tempList, get(testStack));

        if (getParent(T, get(testStack)) == NIL) {
            fprintf(stdout, "Component %d: ", component);
            printList(stdout, tempList);
            fprintf(stdout,"\n");
            clear(tempList);
            component++;
        }
        movePrev(testStack);
    }
    moveFront(testStack);
    fprintf(stdout, "\n");


    Graph copy = copyGraph(A);

    fprintf(stdout, "Copied Graph: \n");
    printGraph(stdout, copy);

    freeGraph(&A);
    A = NULL;

    freeGraph(&copy);
    copy = NULL;

    freeGraph(&T);
    T = NULL;

    freeList(&testStack);
    testStack = NULL;

    freeList(&tempList);
    tempList = NULL;




    return (0);
}