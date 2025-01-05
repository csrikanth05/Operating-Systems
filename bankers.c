#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_P 20  // Maximum number of processes
#define MAX_RES 20  // Maximum number of resources

int m, n;  // m = resources, n = processes
int total[MAX_RES];
int available[MAX_RES];
int max[MAX_P][MAX_RES];
int allocation[MAX_P][MAX_RES];
int need[MAX_P][MAX_RES];
int safe[100][MAX_P];  // Array to store all safe sequences
int count = 0;  // Count of safe sequences

void calculate_need_matrix() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }
}

bool check_safe(int proc) {
    for (int j = 0; j < m; j++) {
        if (need[proc][j] > available[j]) {
            return false;
        }
    }
    return true;
}

void find_all_safe_sequences(bool visited[], int safe_seq[], int level) {
    if (level == n) {
        for (int i = 0; i < n; i++) {
            safe[count][i] = safe_seq[i];
        }
        count++;
        return;
    }

    for (int i = 0; i < n; i++) {
        if (!visited[i] && check_safe(i)) {
            visited[i] = true;
            for (int j = 0; j < m; j++) {
                available[j] += allocation[i][j];
            }
            safe_seq[level] = i;
            find_all_safe_sequences(visited, safe_seq, level + 1);

            for (int j = 0; j < m; j++) {
                available[j] -= allocation[i][j];
            }
            visited[i] = false;
        }
    }
}

void display_safe_sequences() {
    printf("Number of safe sequences: %d\n", count);
    if (count == 0) {
        printf("No safe sequences found.\n");
    } else {
        for (int i = 0; i < count; i++) {
            for (int j = 0; j < n; j++) {
                printf("%d ", safe[i][j]);
            }
            printf("\n");
        }
    }
}

int main() {
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        printf("Error: Unable to open file.\n");
        return 1;
    }

    fscanf(file, "%d", &m);  // Number of resources
    fscanf(file, "%d", &n);  // Number of processes

    // Read total instances of each resource
    for (int i = 0; i < m; i++) {
        fscanf(file, "%d", &total[i]);
    }

    // Read maximum matrix
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            fscanf(file, "%d", &max[i][j]);
        }
    }

    // Read allocation matrix
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            fscanf(file, "%d", &allocation[i][j]);
        }
    }

    fclose(file);

    // Calculate available resources
    for (int j = 0; j < m; j++) {
        int allocated = 0;
        for (int i = 0; i < n; i++) {
            allocated += allocation[i][j];
        }
        available[j] = total[j] - allocated;
    }

    calculate_need_matrix();

    bool visited[MAX_P] = {0};
    int safe_seq[MAX_P];
    find_all_safe_sequences(visited, safe_seq, 0);

    display_safe_sequences();

    return 0;
}
