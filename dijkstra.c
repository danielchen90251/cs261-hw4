/*
 * In this file, you'll implement Dijkstra's algorithm to find the least
 * expensive paths in the graph defined in `airports.dat`.  Don't forget to
 * include your name and @oregonstate.edu email address below.
 *
 * Name: Chiu-Chun, Chen
 * Email: chenchiu@oregonstate.edu
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_FILE "airports.dat"
#define START_NODE 0
#define N 10
#define INF 9999


void strrev(char path[]) {
	char str[20];
	int len = strlen(str);
	for (int i = len -1; i >= 0; i--){
		printf("%c", str[i]);
	}
}

int dijkstra(int source, int target, int minCost[][N + 1]) {
	int cost[N + 1], prev[N + 1], selected[N + 1] = {0};
	int i, m, min, start, newMinCost;

	// Initialization
	for (i = 1; i <= N; i++){
        cost[i] = INF;
        prev[i] = -1;
    }

	// Q = new priority queue
	start = source;
	selected[start] = 1;
	// insert N_start into Q with priority 0
	cost[start] = 0;

	while (selected[target] == 0){
		min = INF;
        m = 0;
		for (i = 1; i <= N; i++){
			newMinCost = cost[start] + minCost[start][i];
			if (newMinCost < cost[i] && selected[i] == 0){
				cost[i] = newMinCost;
                prev[i] = start;
			}
			if (min > cost[i] && selected[i] == 0){
				min = cost[i];
                m = i;
			}
		}
		start = m;
        selected[start] = 1;
	}
	start = target;
    return cost[target];
}

int main(int argc, char const *argv[]) {
	FILE *fp = fopen("airports.dat", "r");
	int num_nodes;
	int num_edges;
	int node_i, node_j, minCost_i_j;
	fscanf(fp, "%d\n", &num_nodes);
	printf("%d\n", num_nodes);

	fscanf(fp, "%d\n", &num_edges);
	printf("%d\n", num_edges);

	int minCost[N + 1][N + 1], i, j, w, ch, co;
    int source, target, x, y;

	for (i = 1; i <= N; i++){
		for (j = 1; j<= N; j++){
  			minCost[i][j] = INF;
		}
	}
	
	while (fscanf(fp, "%d %d %d\n", &node_i, &node_j, &minCost_i_j) != EOF){
		printf("%d %d %d\n", node_i, node_j, minCost_i_j);
		minCost[node_i+1][node_j+1] = minCost_i_j;
	}

	for (target = 1; target <= 10; target++){
		int d = dijkstra(1, target, minCost);
		printf("Min cost from node 0 to node %d is: %d\n", target - 1, d);
	}

	return 0;
}
