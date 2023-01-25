#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_TRIALS 100000
#define THREAD_NUM 4

int nhits;
int n;
sem_t sem;

void* WorkerThread(void* pvar) {
	unsigned int rand_state = (unsigned int) time(NULL) + pthread_self();
	int random;
	
	for(int j=0; j<NUM_TRIALS; j++) {
		int bd[365] = {0};
		for(int i=0; i<n; i++) {
			random = rand_r(&rand_state) % 365;
			bd[random]++;
			if(bd[random]>1) {
				sem_wait(&sem);
				nhits++;
				sem_post(&sem);
				break;
			}
		}
	}
	
}

int main(int argc, void* argv[]) {
	nhits = 0;
	n = atoi(argv[1]);
	pthread_t th[THREAD_NUM];
	sem_init(&sem, 0, 1);
	
	for(int i=0; i<THREAD_NUM; i++) {
		if(pthread_create(&th[i], NULL, &WorkerThread, NULL) != 0) {
			printf("Failed to create thread");
		}
	}
	
	for(int i=0; i<THREAD_NUM; i++) {
		if (pthread_join(th[i], NULL) != 0) {
            printf("Failed to join thread");
        }
	}
	
	double prob = (double)nhits/(THREAD_NUM*NUM_TRIALS);
	printf("The probability of two students in a %d people class having the same birthday is %0.2f%%\n", n, prob*100);
	sem_destroy(&sem);
	return 0;
}