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
#include<fcntl.h>

#include <sys/mman.h>

#define IN (ptr->in)
#define OUT (ptr->out)
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

int n = -1;

// Main routine
int main(int argc, char *argv[]){
    
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
	int shm_fd;
	shared_struct* ptr;
	shm_fd = shm_open("shared_memory", O_CREAT | O_RDWR, 0666);
	ftruncate(shm_fd, sizeof(shared_struct));
	ptr = mmap(0, sizeof(shared_struct), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    printf("Address of shared buffer: %p\n", ptr->buf);
	printf("Address of n: %p\n", &n);
    
    // initialize indices
    IN = 0;
    OUT = 0;

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
        ptr->buf[IN] = pow(ii * 0.5,2);
            
        //increment the index using module arithmatic
        IN = (IN + 1) % BUF_LEN;
           
        ii++;
    }
	
	munmap(ptr, sizeof(shared_struct));
	close(shm_fd);
	shm_unlink("shared_memory");
   
    return 0;
}

