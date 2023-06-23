// Name: Leonid Shuster, ID #1616143
// CruzID: lshuster
// Assignment name: pa4
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
    int* elementDist;
    int graphOrder;
    int graphSize;
    int graphSource;
} GraphObj;

// Allocates memory for graph and initializes data of elements
Graph newGraph(int n) {
    Graph graph = malloc(sizeof(GraphObj));
    assert(graph != NULL);

    graph->adjLists = malloc((n+1) * sizeof(List));
    graph->elementColor = malloc((n+1) * sizeof(int));
    graph->elementParent = malloc((n+1) * sizeof(int));
    graph->elementDist = malloc((n+1) * sizeof(int));

    for (int i = 1; i < n + 1; i++) { // Loops through every element's adjacent list initializing its data
        graph->adjLists[i] = newList();
        graph->elementColor[i] =  WHITE;
        graph->elementParent[i] = NIL;
        graph->elementDist[i] = INF;
    }
    graph->graphOrder = n;
    graph->graphSize = 0;
    graph->graphSource = NIL;

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
    free((*pG)->elementDist);
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

// Returns source of graph, NIL if BFS not run yet
int getSource(Graph G) {
    if (G == NULL) {
        printf("Graph Error: calling getSource() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    return G->graphSource;
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

// Returns distance from source to element, INF if BFS not run yet
int getDist(Graph G, int u) {
    if (G == NULL) {
        printf("Graph Error: calling getDist() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    else if (u < 1 || u > getOrder(G)) {
        printf("Graph Error: calling getDist() on invalid vertex\n");
        exit(EXIT_FAILURE);
    }
    return G->elementDist[u];
}

// Appends to list path from source to element, NIL if no path exists, uses recursion
void getPath(List L, Graph G, int u) {
    if (L == NULL) {
        printf("Graph Error: calling getPath() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    else if (G == NULL) {
        printf("Graph Error: calling getPath() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    else if (u < 1 || u > getOrder(G)) {
        printf("Graph Error: calling getPath() on invalid vertex\n");
        exit(EXIT_FAILURE);
    }
    int source = getSource(G);
    int parent = G->elementParent[u];

    // Recursively moves from source to element through parents
    if (source != NIL) { // BFS has to have been called
        if (u == source) { // If current element is the source
            append(L, source);
        } else if (parent == NIL) { // If no path is found, append NIL
            append(L, NIL);
        } else { // Move onto next parent
            getPath(L, G, parent);
            append(L, u);
        }
    }
}

// Deletes all edges of graph
void makeNull(Graph G) {
    if (G == NULL) {
        printf("Graph Error: calling makeNull() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 1; i < G->graphOrder + 1; i++) { // Loops through every element's adjacent list clearing the list
        clear(G->adjLists[i]);
    }
    G->graphSize = 0;
}

// Helper function for addEdge to help sort new edge into adjacency list
void insert(List L, int u) {
    moveFront(L);
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
    //moveFront(L); // Bring cursor to the front of the list after every insertion}
}

// Adds edge between two elements, adds each element to each other's adjacency lists
void addEdge(Graph G, int u, int v) {
    if (G == NULL) {
        printf("Graph Error: calling addEdge() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    else if (u < 1 || u > getOrder(G)) {
        printf("Graph Error: calling addEdge() on invalid vertex\n");
        exit(EXIT_FAILURE);
    }
    insert(G->adjLists[u], v); // Add v to u's adjacency list
    insert(G->adjLists[v], u); // Add u to v's adjacency list

    G->graphSize++;
}

// // Adds arc between two elements, adds only one element to another's adjacency lists
void addArc(Graph G, int u, int v) {
    if (G == NULL) {
        printf("Graph Error: calling addArc() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    else if (u < 1 || u > getOrder(G)) {
        printf("Graph Error: calling addEdge() on invalid vertex\n");
        exit(EXIT_FAILURE);
    }
    insert(G->adjLists[u], v); // Add v to u's adjacency list

    G->graphSize++;
}

// Runs BFS on the graph, setting a source and passing by each element and assigning parents and distance
void BFS(Graph G, int s) {
    if (G == NULL) {
        printf("Graph Error: calling addArc() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    // WHITE = undiscovered element
    // GRAY = discovered element but neighbors are not
    // BLACK = discovered element and neighbors are also

    for (int x = 1; x < G->graphOrder + 1; x++) { // Loops through every element's adjacent list initializing their data
        G->elementColor[x] = WHITE;
        G->elementParent[x] = NIL;
        G->elementDist[x] = INF;
    }
    // Pass source element
    G->graphSource = s;
    G->elementColor[s] = GRAY;
    G->elementParent[s] = NIL;
    G->elementDist[s] = 0;

    List Queue = newList();
    append(Queue, s);

    while (length(Queue) != 0) { // Keep looping until queue is empty
        int x = front(Queue); // Get first element of queue
        deleteFront(Queue);

        List currentList = G->adjLists[x];
        while (index(currentList) != -1) { // Keep looping until cursor falls off the back of the list
            int y = get(currentList);
            int color = G->elementColor[y];

            // If element is undiscovered, make element gray, set parent, and set distance
            if (color == WHITE) {
                G->elementColor[y] = GRAY;
                G->elementParent[y] = x;
                G->elementDist[y] = G->elementDist[x] + 1;
                append(Queue, y);
            }
            moveNext(currentList);
        }
        G->elementColor[x] = BLACK;
        moveFront(currentList);
    }

    freeList(&Queue);
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