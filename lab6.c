// Floyd-Warshall all-pairs shortest paths with successors (Notes 16)
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define oo (INT_MAX) // Infinity

int n;              // Number of vertices
int **dist, **succ;  // Distance and successor matrices

void readInput() {
	// Read weighted edges into adjacency matrix.
	int i, j, k;
	scanf("%d", &n);
	dist = (int**)malloc(sizeof(int*) * n);
	succ = (int**)malloc(sizeof(int*) * n);
	if (!dist || !succ) {
		printf("dist or succ malloc failed %d\n", __LINE__);
		exit(0);
	}
	for (i = 0; i < n; i++) {
		dist[i] = (int*)malloc(sizeof(int) * n);
		succ[i] = (int*)malloc(sizeof(int) * n);
		if (!dist[i] || !succ[i]) {
			printf("dist or succ row malloc failed %d\n", __LINE__);
			exit(0);
		}
	}
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			dist[i][j] = oo;
			succ[i][j] = j;
		}
	}
	while (1) {
		scanf("%d %d %d", &i, &j, &k);
		if (i == -1) {
			break;
		}
		dist[i][j] = k;
	}
}

void printMatrix() {
	int i, k;
	printf("    ");
	for (i = 0;i < n; i++) {
		printf("%3d   ", i);
	}
	printf("\n");
	for (i = 0; i < n; i++) {
		printf("%3d ", i);
		for (k = 0; k < n; k++) {
			if (dist[i][k] < oo) {
				printf("%3d %d ", dist[i][k], succ[i][k]);
			}
			else {
				printf(" oo   ");
			}
		}
		printf("\n");
	}
}

void floydWarshall() {
	int i, j, k;
	int newDist;
	printf("----------------------------------\n");
	/* Floyd-Warshall */
	for (j = 0; j < n; j++) {
		for (i = 0; i < n; i++) {
			if (dist[i][j] < oo) {
				for (k = 0; k < n; k++) {
					if (dist[j][k] < oo) {
						newDist = dist[i][j] + dist[j][k];
						if (newDist < dist[i][k]) {
							dist[i][k] = newDist;
							succ[i][k] = succ[i][j];
						}
					}
				}
			}
		}
		printMatrix();
		printf("----------------------------------\n");
	}
}

void printPaths() {
	int i, j, k;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			if (dist[i][j] == oo) {
				printf("No path from %d to %d\n", i, j);
			}
			else {
				printf("Distance %d for %d ", dist[i][j], i);
				for (k = succ[i][j]; k != j; k = succ[k][j]) {
					printf("%d ", k);
				}
				printf("%d\n", j);
			}
		}
    }
}

void deallocate() {
	int i;
	for (i = 0; i < n; i++) {
		free(dist[i]);
		free(succ[i]);
	}
	free(dist);
	free(succ);
}

int main() {
	readInput();
	printMatrix();
	floydWarshall();
	printPaths();
	deallocate();
}
