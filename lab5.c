// Rebecca B.
// Lab 5 - Depth-First Search
// Code baselined from Dr. Weems

// Compilation:
// gcc rbb7716_lab5.c
// ./a.out<file_name.txt

#include <stdio.h>
#include <stdlib.h>

#define WHITE 0
#define GRAY 1
#define BLACK 2

#define TREE 0
#define BACK 1
#define CROSS 2
#define FORWARD 3

int n, e, time, count = 0;

struct edge {
	int tail;
	int head;
	int type;
	int dist;
};

typedef struct edge edgeType;
edgeType *edgeTab;
int *firstEdge;
int *discovery, *finish, *predecessor, *successor, *longest, *vertexStatus, *topological;

void outputTable() {
	int i, j, length;
	
	// Printing all data tables.
	printf("Vertex  Discovery  Finish  Predecessor  Successor  Longest\n");
	printf("------  ---------  ------  -----------  ---------  -------\n");
	for (i = 0; i < n; i++) {
		printf(" %3d      %3d       %3d       %3d         %3d       %3d\n", i, discovery[i], finish[i], predecessor[i], successor[i], longest[i]);
	}
	printf("\nEdge\tTail\tHead\tType\tDist\n");
	printf("----\t----\t----\t----\t----\n");
	for (i = 0; i < e; i++) {
		printf("%d\t%d\t%d\t", i, edgeTab[i].tail, edgeTab[i].head);
		switch(edgeTab[i].type) {
			case TREE:    printf("T\t"); break;
			case CROSS:   printf("C\t"); break;
			case FORWARD: printf("F\t"); break;
		}
		printf("%d\n", edgeTab[i].dist);
	}

	// Printing the longest path's vertices and their edge distances.
	printf("Vertex %d has longest distance of %d\n", topological[n-1], longest[topological[n-1]]);
	i = topological[n-1];
	while (successor[i] != -1) {
		for (j = 0; j < e; j++) {
			if (edgeTab[j].tail == i && edgeTab[j].head == successor[i]) {
				length = edgeTab[j].dist;
			}
		}
		printf("%d (%d) ", i, length);
		i = successor[i];
	}
	printf("%d\n", i);
}

// Function for sorting edges.
int tailThenHead(const void* xin, const void* yin) {
	int result;
	edgeType *x, *y;
	x = (edgeType*) xin;
	y = (edgeType*) yin;
	result = x->tail - y->tail;
	if (result != 0) {
		return result;
	}
	else {
		return (x->head - y->head);
	}
}

// Storing all data from input file, most importantly, the edges' tails, heads, and distances.
void read_input_file() {
	int a, b, c, i, j;
	edgeType work;
	edgeType *ptr;

	scanf("%d %d", &n, &e);
	edgeTab = (edgeType*)malloc(sizeof(edgeType) * e);
	if (!edgeTab) {
		printf("edgeTab malloc failed %d\n",__LINE__);
		exit(0);
	}
	for (i = 0; i < e; i++) {
		scanf("%d %d %d", &a, &b, &c);
		if (a < 0 || a >= n || b < 0 || b >= n || c < 0) {
			printf("Invalid input %d %d %d at %d\n", a, b, c, __LINE__);
			exit(0);
		}
		edgeTab[i].tail = a;
		edgeTab[i].head = b;
		edgeTab[i].dist = c;
	}

	// Sort edges.
	qsort(edgeTab, e, sizeof(edgeType), tailThenHead);

	// Coalesce duplicates into a single edge.
	j = 0;
	for (i = 1; i < e; i++) {
		if (edgeTab[j].tail == edgeTab[i].tail && edgeTab[j].head == edgeTab[i].head) {
			;
		} 
		else {
			j++;
			edgeTab[j].tail = edgeTab[i].tail;
			edgeTab[j].head = edgeTab[i].head;
		}
	}
	e = j+1;

	// For each vertex as a tail, determine first in range of edgeTab entries.
	firstEdge = (int*)malloc(sizeof(int) * (n+1));
	if (!firstEdge) {
		printf("malloc failed %d\n", __LINE__);
		exit(0);
	}
	j = 0;
	for (i = 0; i < n; i++) {
		firstEdge[i] = j;
		for (; j < e && edgeTab[j].tail == i; j++) {
			;
		}
	}
	firstEdge[n] = e;
	printf("\n");
}

// Finding the longest path.
//		u = black vertex and the next value in the 'topological' array.
void longestPath(int u) {
	int i;
	// Navigating edge table for all vertices pointing to u.
	for (i = 0; i < e; i++) {
		if (edgeTab[i].head == u) {
			// If the paths of vertices pointing to u and u plus the edge between the two are equal...
			if (longest[edgeTab[i].tail] <= (longest[u] + edgeTab[i].dist)) {
				// ... then the successor will be set to the smaller vertex.
				if (longest[edgeTab[i].tail] == (longest[u] + edgeTab[i].dist)) {
					if (u < successor[edgeTab[i].tail]) {
						successor[edgeTab[i].tail] = u;
					}
				}
				// Vertices' paths will be updated if smaller than u's path plus the edge's distance
				// between the two.
				else {
					longest[edgeTab[i].tail] = edgeTab[i].dist + longest[u];
					successor[edgeTab[i].tail] = u;
				}
			}
		}
	}
}

// Performing DFS for discovery/finish times.
//		u = the next largest white vertex
void DFSvisit(int u) {
	int i, v;
	
	vertexStatus[u] = GRAY;
	discovery[u] = ++time;

	for (i = firstEdge[u]; i < firstEdge[u+1]; i++) {
		v = edgeTab[i].head;
		
		if (vertexStatus[v] == WHITE) {
			edgeTab[i].type = TREE;
			predecessor[v] = u;
			DFSvisit(v);
		}
		else if (vertexStatus[v] == GRAY) {
			edgeTab[i].type = BACK;
			printf("graph is cyclic\n");
			exit(0);
		}
		else if (discovery[u] < discovery[v]) {
			edgeTab[i].type = FORWARD;
		}
		else {
			edgeTab[i].type = CROSS;
		} 
	}
	
	vertexStatus[u] = BLACK;
	finish[u] = ++time;
	topological[count++] = u;
	longestPath(u);
}

int main () {
	int u, i, j, k;

	// Getting initial graph data.
	read_input_file();
	discovery = (int*)malloc(sizeof(int) * n);
	finish = (int*)malloc(sizeof(int) * n);
	predecessor = (int*)malloc(sizeof(int) * n);
	successor = (int*)malloc(sizeof(int) * n);
	longest = (int*)malloc(sizeof(int) * n);
	vertexStatus = (int*)malloc(sizeof(int) * n);
	topological = (int*)malloc(sizeof(int) * n);
	if (!discovery || !finish || !predecessor || !successor || !vertexStatus) {
		printf("malloc failed\n");
		exit(0);
	}

	// Initializing each vertex's color, predecessor, and successor.
	for (u = 0; u < n; u++) {
		vertexStatus[u] = WHITE;
		predecessor[u] = -1;
		successor[u] = -1;
		longest[u] = 0;
	}

	time = 0;

	// Looking for the next unprocessed vertex to run DFS from.
	for (u = 0; u < n; u++) {
		if (vertexStatus[u] == WHITE) {
			DFSvisit(u);
		}
	}

	// Printing end results.
	outputTable();

	free(edgeTab);
	free(firstEdge);
	free(discovery);
	free(finish);
	free(predecessor);
	free(successor);
	free(topological);
	free(longest);
	free(vertexStatus);
	return 0;
}


