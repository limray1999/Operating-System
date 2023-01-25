#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include<string.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<unistd.h>
#include<sys/wait.h>

#define NUM_TRIALS 100000
#define PROCESS_NUM 4


void* Function(sem_t* sem, int n, int* nhits) {
	unsigned int rand_state = (unsigned int) time(NULL) + pthread_self();
	int random;
	
	for(int j=0; j<NUM_TRIALS; j++) {
		int bd[365] = {0};
		for(int i=0; i<n; i++) {
			random = rand_r(&rand_state) % 365;
			bd[random]++;
			if(bd[random]>1) {
				sem_wait(sem);
				(*nhits)++;
				sem_post(sem);
				break;
			}
		}
	}
	
}

int main(int argc, void* argv[]) {
	
	if(argc < 2) {
		printf("Please provide the number of students when you invocate me.\n");
		return 0;
	}
	
	int n;
	int *nhits;
	sem_t *sem;
	const char *name_nhits = "nhits";
	const char *name_sem = "semaphore";
	int shm_nhits, shm_sem;
	shm_nhits = shm_open(name_nhits, O_CREAT | O_RDWR, 0666);
	shm_sem = shm_open(name_sem, O_CREAT | O_RDWR, 0666);
	ftruncate(shm_nhits, sizeof(int));
	ftruncate(shm_sem, sizeof(sem_t));
	nhits = mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_nhits, 0);
	sem = mmap(0, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_sem, 0);
	
	n = atoi(argv[1]);
	sem_init(sem, 0, 1);
	*nhits = 0;
	
	pid_t pid1 = fork();
	if(pid1 == 0) {
		Function(sem, n, nhits);
	} else {
		wait(NULL);
		pid_t pid2 = fork();
		if(pid2 == 0) {
			Function(sem, n, nhits);
		} else {
			wait(NULL);
			pid_t pid3 = fork();
			if(pid3 == 0) {
				Function(sem, n, nhits);
			} else {
				wait(NULL);
				Function(sem, n, nhits);
				double prob = (double)(*nhits)/(PROCESS_NUM*NUM_TRIALS);
				printf("The probability of two students in a %d people class having the same birthday is %0.2f%%\n", n, prob*100);
				sem_destroy(sem);
				shm_unlink(name_nhits);
				shm_unlink(name_sem);
			}
		}
	}
	
	return 0;
}