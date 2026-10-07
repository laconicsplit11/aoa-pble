#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LOCATIONS 20
#define NAME_LEN 50
#define INF 999999

int numLocations = 0;
char locationNames[MAX_LOCATIONS][NAME_LEN];
int adjMatrix[MAX_LOCATIONS][MAX_LOCATIONS];
int dist[MAX_LOCATIONS];
int parent[MAX_LOCATIONS];
int visited[MAX_LOCATIONS];
int sourceVertex = -1;
int dijkstraExecuted = 0;

int isWhitespaceChar(char c) {
    return (c == ' ' || c == '\t' || c == '\n');
}

int isValidLocationName(char *str) {
    if (str == NULL || str[0] == '\0') {
        return 0;
    }
    while (*str) {
        if (!isWhitespaceChar(*str)) {
            return 1;
        }
        str++;
    }
    return 0;
}

void enterCampusGraph() {
    printf("\nEnter number of campus locations: ");
    if (scanf("%d", &numLocations) != 1 || numLocations <= 0 || numLocations > MAX_LOCATIONS) {
        printf("Invalid number of locations (Max: %d).\n", MAX_LOCATIONS);
        numLocations = 0;
        while (getchar() != '\n');
        return;
    }

    while (getchar() != '\n');

    for (int i = 0; i < numLocations; i++) {
        while (1) {
            printf("Enter name for Location %d: ", i + 1);
            if (fgets(locationNames[i], NAME_LEN, stdin) == NULL) {
                printf("Error reading input. Please try again.\n");
                continue;
            }

            locationNames[i][strcspn(locationNames[i], "\r\n")] = '\0';

            if (isValidLocationName(locationNames[i])) {
                break;
            }

            printf("Error: Location name cannot be empty. Please enter a valid name.\n");
        }
    }

    // initialize adjacency matrix
    for (int i = 0; i < numLocations; i++) {
        for (int j = 0; j < numLocations; j++) {
            if (i == j) {
                adjMatrix[i][j] = 0;
            } else {
                adjMatrix[i][j] = INF;
            }
        }
    }

    int edges;
    printf("\nEnter number of roads/connections: ");
    if (scanf("%d", &edges) != 1 || edges < 0) {
        printf("Invalid edge count.\n");
        while (getchar() != '\n');
        return;
    }

    printf("Enter road details as (Src_No Dest_No Distance), e.g., '1 2 4':\n");
    for (int k = 0; k < edges; k++) {
        int u, v, weight;
        printf("Road %d: ", k + 1);
        if (scanf("%d %d %d", &u, &v, &weight) != 3) {
            printf("Invalid format. Skipping road.\n");
            while (getchar() != '\n');
            continue;
        }

        if (u >= 1 && u <= numLocations && v >= 1 && v <= numLocations && weight >= 0) {
            adjMatrix[u - 1][v - 1] = weight;
            adjMatrix[v - 1][u - 1] = weight;
        } else {
            printf("Invalid location indices or negative distance. Skipping road.\n");
        }
    }

    sourceVertex = -1;
    dijkstraExecuted = 0;
    printf("\nGraph successfully entered.\n");
}

void displayAdjacencyMatrix() {
    if (numLocations == 0) {
        printf("\nPlease enter graph data first.\n");
        return;
    }

    printf("\n--- Adjacency Matrix ---\n\t");
    for (int i = 0; i < numLocations; i++) {
        printf("[L%d]\t", i + 1);
    }
    printf("\n");

    for (int i = 0; i < numLocations; i++) {
        printf("[L%d]\t", i + 1);
        for (int j = 0; j < numLocations; j++) {
            if (adjMatrix[i][j] == INF) {
                printf("INF\t");
            } else {
                printf("%d\t", adjMatrix[i][j]);
            }
        }
        printf("(%s)\n", locationNames[i]);
    }
}

void selectSourceLocation() {
    if (numLocations == 0) {
        printf("\nPlease enter graph data first.\n");
        return;
    }

    printf("\nAvailable Locations:\n");
    for (int i = 0; i < numLocations; i++) {
        printf("%d. %s\n", i + 1, locationNames[i]);
    }

    int choice;
    printf("Select Source Location (1-%d): ", numLocations);
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        return;
    }

    if (choice >= 1 && choice <= numLocations) {
        sourceVertex = choice - 1;
        dijkstraExecuted = 0;
        printf("Selected Source: %s\n", locationNames[sourceVertex]);
    } else {
        printf("Invalid location selected.\n");
    }
}

void runDijkstra() {
    if (numLocations == 0) {
        printf("\nPlease enter graph data first.\n");
        return;
    }
    if (sourceVertex == -1) {
        printf("\nPlease select a source location first.\n");
        return;
    }

    for (int i = 0; i < numLocations; i++) {
        dist[i] = INF;
        visited[i] = 0;
        parent[i] = -1;
    }

    dist[sourceVertex] = 0;

    for (int count = 0; count < numLocations - 1; count++) {
        int minDist = INF;
        int u = -1; //current minimum distance vertex

        for (int v = 0; v < numLocations; v++) {
            if (!visited[v] && dist[v] < minDist) {
                minDist = dist[v];
                u = v;
            }
        }

        if (u == -1) {
            break; //remaining vertices are unreachable
        }

        visited[u] = 1;

        //relax adjacent neighbors
        for (int v = 0; v < numLocations; v++) {
            if (!visited[v] && adjMatrix[u][v] != INF && dist[u] + adjMatrix[u][v] < dist[v]) {
                dist[v] = dist[u] + adjMatrix[u][v];
                parent[v] = u;
            }
        }
    }

    dijkstraExecuted = 1;
    printf("\nShortest paths computed successfully using Dijkstra's Algorithm.\n");
}

void printPath(int current) {
    if (current == -1) return;
    if (parent[current] != -1) {
        printPath(parent[current]);
        printf(" -> ");
    }
    printf("%s", locationNames[current]);
}

void displayShortestPaths() {
    if (!dijkstraExecuted) {
        printf("\nPlease run 'Find Shortest Distance' first.\n");
        return;
    }

    printf("\n--- Shortest Paths from Source: %s ---\n", locationNames[sourceVertex]);
    for (int i = 0; i < numLocations; i++) {
        if (i == sourceVertex) continue;

        printf("To %-20s : ", locationNames[i]);
        if (dist[i] == INF) {
            printf("No Path Available\n");
        } else {
            printPath(i);
            printf("\n");
        }
    }
}

void displayDistanceTable() {
    if (!dijkstraExecuted) {
        printf("\nPlease run 'Find Shortest Distance' first.\n");
        return;
    }

    printf("\nSource Location: %s\n", locationNames[sourceVertex]);
    printf("----------------------------------------------------------------------------------------------------\n");
    printf("%-25s | %-18s | %-50s\n", "Destination", "Shortest Distance", "Shortest Path");
    printf("----------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < numLocations; i++) {
        if (i == sourceVertex) continue;

        printf("%-25s | ", locationNames[i]);
        if (dist[i] == INF) {
            printf("%-18s | No Path\n", "INF");
        } else {
            printf("%-18d | ", dist[i]);
            printPath(i);
            printf("\n");
        }
    }
    printf("----------------------------------------------------------------------------------------------------\n");
}

void displaySingleDestination() {
    if (!dijkstraExecuted) {
        printf("\nPlease run 'Find Shortest Distance' first.\n");
        return;
    }

    printf("\nAvailable Destinations:\n");
    for (int i = 0; i < numLocations; i++) {
        if (i != sourceVertex) {
            printf("%d. %s\n", i + 1, locationNames[i]);
        }
    }

    int destChoice;
    printf("Select Destination Location (1-%d): ", numLocations);
    if (scanf("%d", &destChoice) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        return;
    }

    int dest = destChoice - 1;
    if (dest < 0 || dest >= numLocations) {
        printf("Invalid destination selected.\n");
        return;
    }

    if (dest == sourceVertex) {
        printf("\nThe selected location is the source itself (%s). Distance: 0\n", locationNames[sourceVertex]);
        return;
    }

    printf("\n------------------------------------------------------------\n");
    printf("Source      : %s\n", locationNames[sourceVertex]);
    printf("Destination : %s\n", locationNames[dest]);
    if (dist[dest] == INF) {
        printf("Distance    : INF (Unreachable)\n");
        printf("Path        : No direct or indirect path available\n");
    } else {
        printf("Distance    : %d\n", dist[dest]);
        printf("Path        : ");
        printPath(dest);
        printf("\n");
    }
    printf("------------------------------------------------------------\n");
}

int main() {
    int choice;

    while (1) {
        printf("\n============================================\n");
        printf("  Campus Shortest Route Finder (Dijkstra)   \n");
        printf("============================================\n");
        printf("1. Enter Campus Graph\n");
        printf("2. Display Adjacency Matrix\n");
        printf("3. Select Source Location\n");
        printf("4. Find Shortest Distance\n");
        printf("5. Display Shortest Paths to All Locations\n");
        printf("6. Display Distance Table (Source to All Locations)\n");
        printf("7. Find Distance and Path to Specific Destination\n");
        printf("8. Exit\n");
        printf("Enter your choice (1-8): ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Exiting.\n");
            break;
        }

        switch (choice) {
            case 1:
                enterCampusGraph();
                break;
            case 2:
                displayAdjacencyMatrix();
                break;
            case 3:
                selectSourceLocation();
                break;
            case 4:
                runDijkstra();
                break;
            case 5:
                displayShortestPaths();
                break;
            case 6:
                displayDistanceTable();
                break;
            case 7:
                displaySingleDestination();
                break;
            case 8:
                printf("\nExiting Campus Shortest Route Finder.\n");
                exit(0);
            default:
                printf("\nInvalid choice! Please select an option from 1 to 8.\n");
        }
    }

    return 0;
}