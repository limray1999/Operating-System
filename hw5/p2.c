#include<sys/types.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

int main( int argc, char *argv[]) {
	
	
	int n = atoi(argv[1]) ;
	int fd [2];
	pid_t pid;
	
	/* create the pipe */
	if ( pipe(fd) == -1) {
		fprintf(stderr, "Pipe failed");
		return -1;
	}
	
	pid = fork();
	
	if(pid<0) {
		fprintf(stderr, "Fork error");
		return 1;
	} else if(pid==0) {
		int i = 0;
		double w;
		close(fd[READ_END]);
		while(i<n) {
			w = (0.5*i)*(0.5*i);
			printf("Generating %dth number...\n", ++i);
			write(fd[WRITE_END], &w, sizeof(double));
			sleep((float)rand()/(float)(RAND_MAX) * 4.99);
		}
		close(fd[WRITE_END]);
	} else {
		int i = 0;
		double r;
		close(fd[WRITE_END]);
		while(i<n) {
			if(read(fd[READ_END], &r, sizeof(double))>0) {
				printf("The generated %dth number is: %f\n", ++i, r);
			}
			sleep(0.01);
		}
		close(fd[READ_END]);
	}
	
	return 0;
}

