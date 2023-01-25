#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include<string.h>
#include<ctype.h>
#include<time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <wait.h>
#include <math.h>

#include <sys/mman.h>

#define IN (pshared->in)
#define OUT (pshared->out)
#define BUF_LEN 10

//structure definition
typedef struct {
    int in;
    int out;
    float buf[BUF_LEN];
} shared_struct;

//check the format of the argument, only digit char allowed
int checkDigit(char *str){
    for(int i = 0;i<strlen(str);i++){
        if(!isdigit(str[i]))
            return 0;
    }
    return 1;
}

// Main routine
int main(int argc, char *argv[]){
    int n;
    
    // validate the input parameter n
    if(argc != 2){
        printf("usage: ./lab5_part1 <n> \n");
        return -1;
    }

    // validate and obtain the input
    if(!checkDigit(argv[1])){
        printf("integer parameters only!\n");
        return -1;
    }    
    n = atoi(argv[1]);

    // create the shared buffer
    shared_struct* pshared = (shared_struct*) mmap(NULL, sizeof(shared_struct), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    printf("size of the buffer_struct is %d - this is static\n", BUF_LEN);
    
    // initialize indices
    IN = 0;
    OUT = 0;


    // spawn the child process
    pid_t pid = fork();    
    if (pid < 0) {
        perror("Error ");
        return -1;
    }


    // child process
    else if (pid == 0){
        // child process field
        int ii = 0, interval;
        
        // set the seed
        srand((unsigned) time(NULL));
        
        while(ii < n){
             // wait for a random interval of time (0 to 4.999 seconds)
            interval = rand() % 5000;
            usleep(interval * 1000);

            // wait till buffer is not full
            while((IN + 1) % BUF_LEN == OUT);
            
            // produce an item
            pshared->buf[IN] = pow(ii * 0.5,2);
            
            //increment the index using module arithmatic
            IN = (IN + 1) % BUF_LEN;
           
            ii++;
        }
    }
        
    // parent process    
    else{
        // parent process field
        int ii = 0;
        
        while(ii < n){
            // wait till buffer is not empty
            while(IN == OUT);
            
            // consume an item
            printf("%f ", pshared->buf[OUT]);
            fflush(stdout);

            // increment the read index using modulo arithmatic
            OUT = (OUT + 1) % BUF_LEN;
            
            ii++;
        }
        
        printf("\n");
        fflush(stdout);


        //wait the child to finish
        wait(&pid);

        // remove the shared buffer
        munmap(pshared,sizeof(shared_struct));
    

    }

   
    return 0;
}

