#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <semaphore.h>

#define NUM_THREADS 4
#define NUM_TRIALS 1000000
int n;
int nums_hits = 0;
sem_t lock;
// check if there are two numbers the smae in the list
bool checkSame(int* list,int n){
    for(int i=0;i<n-1;i++){
        for(int j=i+1;j<n;j++){
            if(list[i]==list[j])
                return 1;
        }
    }
    return 0;
}

void* WorkerThread(void* pvar) {
    /* Adding to a rand number to get a local random number for each thread */
    unsigned int rand_state = (unsigned int) time(NULL) + pthread_self();

    for (int i = 0; i < NUM_TRIALS; i++) {
        /* Use rand_r instead of rand since rand is not thread safe */
        int l[n];
        for(int i=0;i<n;i++){
            l[i] = rand_r(&rand_state)%365;
        }
        if(checkSame(l,n)){
            sem_wait(&lock);
            ++nums_hits;
            sem_post(&lock);
        }
    }
}

//check the format of the argument, only digit char allowed
int checkDigit(char *str){
    for(int i = 0;i<strlen(str);i++){
        if(!isdigit(str[i]))
            return 0;
    }
    return 1;
}


int main(int argc, char *argv[])
{
    
    // validate the input parameter n
    if(argc != 2){
        printf("usage: ./lab5_part3 <n> \n");
        return -1;
    }

    // validate and obtain the input
    if(!checkDigit(argv[1])){
        printf("integer parameters only!\n");
        return -1;
    }
    n = atoi(argv[1]);
    
    /* Seed for the pseudo-random number */
    srand((unsigned)time(NULL));

    /* Array of threads */
    pthread_t *threads = malloc(NUM_THREADS * sizeof(pthread_t));

    pthread_attr_t attr;
    pthread_attr_init(&attr);

    //Inititalize the semaphore
    sem_init(&lock, 0, 1);

    /* Create the threads */
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], &attr, WorkerThread, (void *) NULL);
    }

    /* Wait for threads to exit */
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&lock);
    free(threads);

    printf("Possibility: %f\n", (double)nums_hits /(4*NUM_TRIALS));

    return 0;
}

