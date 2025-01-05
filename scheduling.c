#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSES 100

typedef struct {
    int pid;        
    int arrival;    
    int burst;      
    int waiting;    
    int turnaround; 
    int response;   
    int priority;   
} Process;

void calculateFCFS(Process p[], int n);
void calculateSJF(Process p[], int n);
void calculateSRTF(Process p[], int n);
void calculatePriority(Process p[], int n);
void calculateRR(Process p[], int n, int quantum);
void printResults(Process p[], int n);

int main() {
    int algorithm, n;
    Process processes[MAX_PROCESSES];

    
    scanf("%d", &algorithm);
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &processes[i].arrival);
    }
    for (int i = 0; i < n; i++) {
        scanf("%d", &processes[i].burst);
        processes[i].pid = i + 1;
    }

    if (algorithm == 4) {
        for (int i = 0; i < n; i++) {
            scanf("%d", &processes[i].priority);
        }
        calculatePriority(processes, n);
    } else if (algorithm == 5) {
        int quantum;
        scanf("%d", &quantum);
        calculateRR(processes, n, quantum);
    } else if (algorithm == 3) {
        calculateSRTF(processes, n);
    } else if (algorithm == 2) {
        calculateSJF(processes, n);
    } else if (algorithm == 1) {
        calculateFCFS(processes, n);
    } else {
        printf("Invalid algorithm choice!\n");
        return 1;
    }

    printResults(processes, n);

    return 0;
}

void calculateFCFS(Process p[], int n) {
    int total_time = 0;
    for (int i = 0; i < n; i++) {
        if (total_time < p[i].arrival) {
            total_time = p[i].arrival; 
        }
        p[i].waiting = total_time - p[i].arrival;
        total_time += p[i].burst;
        p[i].turnaround = p[i].waiting + p[i].burst;
    }
}

void calculateSJF(Process p[], int n) {
    int total_time = 0;
    int completed = 0;
    int min_burst = 0, min_index = -1;

    while (completed < n) {
        min_burst = 999999; 
        for (int i = 0; i < n; i++) {
            if (p[i].arrival <= total_time && p[i].burst < min_burst && p[i].turnaround == 0) {
                min_burst = p[i].burst;
                min_index = i;
            }
        }

        if (min_index != -1) {
            total_time += p[min_index].burst;
            p[min_index].waiting = total_time - p[min_index].arrival - p[min_index].burst;
            p[min_index].turnaround = total_time - p[min_index].arrival;
            completed++;
        } else {
            total_time++;
        }
    }
}

void calculateSRTF(Process p[], int n) {
    int total_time = 0;
    int completed = 0;
    int min_index = -1;

    while (completed < n) {
        int min_burst = 999999; 
        for (int i = 0; i < n; i++) {
            if (p[i].arrival <= total_time && p[i].burst < min_burst) {
                min_burst = p[i].burst;
                min_index = i;
            }
        }

        if (min_index != -1) {
            p[min_index].burst--;
            if (p[min_index].burst == 0) {
                p[min_index].turnaround = total_time + 1 - p[min_index].arrival;
                p[min_index].waiting = p[min_index].turnaround - p[min_index].burst;
                completed++;
            }
        }
        total_time++;
    }
}

void calculatePriority(Process p[], int n) {
    int total_time = 0;
    int completed = 0;

    while (completed < n) {
        int highest_priority = 999999;
        int min_index = -1;

        for (int i = 0; i < n; i++) {
            if (p[i].arrival <= total_time && p[i].priority < highest_priority && p[i].turnaround == 0) {
                highest_priority = p[i].priority;
                min_index = i;
            }
        }

        if (min_index != -1) {
            total_time += p[min_index].burst;
            p[min_index].waiting = total_time - p[min_index].arrival - p[min_index].burst;
            p[min_index].turnaround = total_time - p[min_index].arrival;
            completed++;
        } else {
            total_time++;
        }
    }
}

void calculateRR(Process p[], int n, int quantum) {
    int total_time = 0;
    int remaining[MAX_PROCESSES];
    for (int i = 0; i < n; i++) {
        remaining[i] = p[i].burst; 
    }

    while (1) {
        int done = 1;
        for (int i = 0; i < n; i++) {
            if (remaining[i] > 0) {
                done = 0; 
                if (p[i].arrival <= total_time) {
                    if (remaining[i] > quantum) {
                        total_time += quantum;
                        remaining[i] -= quantum;
                    } else {
                        total_time += remaining[i];
                        p[i].waiting = total_time - p[i].arrival - p[i].burst;
                        p[i].turnaround = total_time - p[i].arrival;
                        remaining[i] = 0;
                    }
                }
            }
        }
        if (done) break; 
    }
}

void printResults(Process p[], int n) {
    int total_turnaround = 0, total_waiting = 0;
    for (int i = 0; i < n; i++) {
        total_waiting += p[i].waiting;
        total_turnaround += p[i].turnaround;
        p[i].response = p[i].waiting + p[i].burst; 
    }

    for (int i = 0; i < n; i++) {
        printf("%d ", p[i].waiting);
    }
    printf("\n");

    for (int i = 0; i < n; i++) {
        printf("%d ", p[i].response - p[i].burst);
    }
    printf("\n");

    for (int i = 0; i < n; i++) {
        printf("%d ", p[i].turnaround);
    }
    printf("\n");

    float throughput = (float)n / total_turnaround;
    printf("%.2f\n", throughput);
}
