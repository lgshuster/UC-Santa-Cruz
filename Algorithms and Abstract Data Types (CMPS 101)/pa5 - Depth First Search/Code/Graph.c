// Name: Leonid Shuster, ID #1616143
// CruzID: lshuster
// Assignment name: pa5
// Role: Creates graph that uses arrays to store info about elements, also has various methods to manipulate graph

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Graph.h"

#define WHITE 1
#define GRAY 2
#define BLACK 3

// Struct for graph, contains arrays with adjacent elements, color, parent, and distance from source, as well as order, size, and source of graph
typedef struct GraphObj {
    List* adjLists;
    int* elementColor;
    int* elementParent;
    int* discoverTime;
    int* finishTime;
    int graphOrder;
    int graphSize;

} GraphObj;

// Allocates memory for graph and initializes data of elements
Graph newGraph(int n) {
    Graph graph = malloc(sizeof(GraphObj));
    assert(graph != NULL);

    graph->adjLists = malloc((n+1) * sizeof(List));
    graph->elementColor = malloc((n+1) * sizeof(int));
    graph->elementParent = malloc((n+1) * sizeof(int));
    graph->discoverTime = malloc((n+1) * sizeof(int));
    graph->finishTime = malloc((n+1) * sizeof(int));

    for (int i = 1; i < n + 1; i++) { // Loops through every element's adjacent list initializing its data
        graph->adjLists[i] = newList();
        graph->elementColor[i] =  WHITE;
        graph->elementParent[i] = NIL;
        graph->discoverTime[i] = UNDEF;
        graph->finishTime[i] = UNDEF;
    }
    graph->graphOrder = n;
    graph->graphSize = 0;

    return graph;
}

// Frees graph and all its arrays/lists
void freeGraph(Graph* pG) {
    if (pG == NULL || *pG == NULL) {
        printf("Graph Error: calling freeGraph() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 1; i < (*pG)->graphOrder + 1; i++) { // Loops through every element's adjacent list freeing the list
        freeList(&((*pG)->adjLists[i]));
    }
    free((*pG)->adjLists);
    free((*pG)->elementColor);
    free((*pG)->elementParent);
    free((*pG)->discoverTime);
    free((*pG)->finishTime);
    free(*pG);

    *pG = NULL;
}

// Returns order of graph (number of vertices)
int getOrder(Graph G) {
    if (G == NULL) {
        printf("Graph Error: calling getOrder() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    return G->graphOrder;
}

// Returns size of graph (number of edges/arcs)
int getSize(Graph G) {
    if (G == NULL) {
        printf("Graph Error: calling getSize() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    return G->graphSize;
}

// Returns parent of element, or NIL if BFS not run yet
int getParent(Graph G, int u) {
    if (G == NULL) {
        printf("Graph Error: calling getParent() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    else if (u < 1 || u > getOrder(G)) {
        printf("Graph Error: calling getParent() on invalid vertex\n");
        exit(EXIT_FAILURE);
    }
    return G->elementParent[u];
}

int getDiscover(Graph G, int u) {
    if (G == NULL) {
        printf("Graph Error: calling getDiscover() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    else if (u < 1 || u > getOrder(G)) {
        printf("Graph Error: calling getDiscover() on invalid vertex\n");
        exit(EXIT_FAILURE);
    }
    return G->discoverTime[u];
}

int getFinish(Graph G, int u) {
    if (G == NULL) {
        printf("Graph Error: calling getFinish() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    else if (u < 1 || u > getOrder(G)) {
        printf("Graph Error: calling getFinish() on invalid vertex\n");
        exit(EXIT_FAILURE);
    }
    return G->finishTime[u];
}

// Helper function for addEdge to help sort new edge into adjacency list
void insert(List L, int u) {
    while (index(L) != -1) { // Keep looping until cursor falls off the back of the list
        if (u < get(L)) { // If current word comes before word found at cursor, insert before
            insertBefore(L, u);
            break;
        }
        else { // If current word does not come before word found at cursor, keep moving cursor forward until correct position is found
            moveNext(L);
        }
    }
    if (index(L) == -1) { // If cursor does fall off, just add to the back
        append(L, u);
    }
    moveFront(L); // Bring cursor to the front of the list after every insertion
}

// // Adds arc between two elements, adds only one element to another's adjacency lists
void addArc(Graph G, int u, int v) {
    if (G == NULL) {
        printf("Graph Error: calling addArc() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    else if (u < 1 || u > getOrder(G) || v < 1 || v > getOrder(G)) {
        printf("Graph Error: calling addArc() on invalid vertex\n");
        exit(EXIT_FAILURE);
    }
    insert(G->adjLists[u], v); // Add v to u's adjacency list

    G->graphSize++;
}

// Adds edge between two elements, adds each element to each other's adjacency lists
void addEdge(Graph G, int u, int v) {
    if (G == NULL) {
        printf("Graph Error: calling addEdge() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    else if (u < 1 || u > getOrder(G) || v < 1 || v > getOrder(G)) {
        printf("Graph Error: calling addEdge() on invalid vertex\n");
        exit(EXIT_FAILURE);
    }
    insert(G->adjLists[u], v); // Add v to u's adjacency list
    insert(G->adjLists[v], u); // Add u to v's adjacency list

    G->graphSize++;
}

// Helper function for DFS to visit an element
void Visit(Graph G, List S, int x, int* t) {
    G->discoverTime[x] = ++(*t);
    G->elementColor[x] = GRAY;

    List currentList = G->adjLists[x];
    while (index(currentList) != -1) { // Keep looping until cursor falls off the back of the stack
        int y = get(currentList);

        // If element is undiscovered, make element gray, set parent, and visit again
        if (G->elementColor[y] == WHITE) {
            G->elementParent[y] = x;
            Visit(G, S, y, t);
        }
        moveNext(currentList);
    }
    moveFront(currentList);
    G->elementColor[x] = BLACK;
    G->finishTime[x] = ++(*t);
    prepend(S, x);
}

// Runs DFS on the graph, and organizes list of vertices by decreasing finish times
void DFS(Graph G, List S) {
    if (G == NULL) {
        printf("Graph Error: calling DFS() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    else if (S == NULL) {
        printf("Graph Error: calling DFS() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    // WHITE = undiscovered element
    // GRAY = discovered element but neighbors are not
    // BLACK = discovered element and neighbors are also

    if (length(S) == getOrder(G)) {
        for (int x = 1; x < G->graphOrder + 1; x++) { // Loops through every element's adjacent list initializing their data
            G->elementColor[x] = WHITE;
            G->elementParent[x] = NIL;
            G->discoverTime[x] = UNDEF;
            G->finishTime[x] = UNDEF;
        }
        // Copy the old stack and clear it to create a new stack
        List tempList = copyList(S);
        clear(S);

        int time = 0;
        while (index(tempList) != -1) { // Keep looping until cursor falls off the back of the stack
            int x = get(tempList);

            // If element is undiscovered, visit it and set parents and discover and finish time
            if (G->elementColor[x] == WHITE) {
                Visit(G, S, x, &time);
            }
            moveNext(tempList);
        }
        moveFront(tempList);
        freeList(&tempList);
    }
}

// Returns transposed graph
Graph transpose(Graph G) {
    if (G == NULL) {
        printf("Graph Error: calling transpose() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }

    Graph graph = newGraph(G->graphOrder);

    // Loops through every adjacent list of the old graph and adds an arc between the current element of a list and the list of the element being traversed
    for (int i = 1; i < getOrder(G) + 1; i++) {
        List currentList = G->adjLists[i];
        while (index(currentList) != -1) { // Keep looping until cursor falls off the back of the list
            int currentNumber = get(currentList);
            addArc(graph, currentNumber, i);
            moveNext(currentList);
        }
        moveFront(currentList);
    }
    return graph;
}

Graph copyGraph(Graph G) {
    Graph graph = newGraph(G->graphOrder);

    for (int i = 1; i < getOrder(G) + 1; i++) {
        List currentList = G->adjLists[i];
        while (index(currentList) != -1) { // Keep looping until cursor falls off the back of the list
            int currentNumber = get(currentList);
            addArc(graph, i, currentNumber);
            moveNext(currentList);
        }
        moveFront(currentList);
    }

    return graph;
}

// Print graph
void printGraph(FILE* out, Graph G) {
    for (int i = 1; i < getOrder(G) + 1; i++) { // Loops through every element's adjacent list printing it
        List currentList = G->adjLists[i];
        fprintf(out, "%d: ", i);
        printList(out, currentList);
        fprintf(out, "\n");
    }
}