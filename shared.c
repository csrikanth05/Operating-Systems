#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define SHM_SIZE 1024

int main() {
    int shm_id;
    key_t key = 1234;
    char *shm_ptr;
    pid_t pid;

    
    if ((shm_id = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        shm_ptr = (char *)shmat(shm_id, NULL, 0);
        if (shm_ptr == (char *)-1) {
            perror("shmat");
            exit(1);
        }
        char message[] = "Hello from the writer process!";
        strcpy(shm_ptr, message);
        printf("Writer: Data written to shared memory.\n");

        shm_ptr[strlen(message)] = '*';  
        shmdt(shm_ptr);
    } else {
        wait(NULL);

        shm_ptr = (char *)shmat(shm_id, NULL, 0);
        if (shm_ptr == (char *)-1) {
            perror("shmat");
            exit(1);
        }

        while (shm_ptr[strlen(shm_ptr) - 1] != '*') {
        }

        printf("Reader: Data read from shared memory: %s\n", shm_ptr);

        shmdt(shm_ptr);
        shmctl(shm_id, IPC_RMID, NULL);
    }

    return 0;
}

