#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/wait.h>
#define NUM_PROCS 5
#define RD_END 0
#define WR_END 1
#define MAX_MESS 100

struct message_t {
	int dest;
	char mess_str[MAX_MESS];
};

int get_user_message(struct message_t *pmess) {
	// wait 1 second (allows printouts from nodes to flush)
	sleep(1);
	// ask user for input and scan it
	printf("Enter destination and message string: ");

	scanf("%d %s", &pmess->dest, pmess->mess_str);
	if(pmess->dest<0 || pmess->dest>=NUM_PROCS) {
		return -1;
	}
	return 0;
}

int main(int argc,char *argv[]) {
	struct message_t mess;
	mess.dest = 0;
	for (int ii=0;ii<MAX_MESS;ii++) mess.mess_str[ii]=0;
	// create the ordinary pipes
	// HINT: This is created before any of the processes and is
	// thus visible to all of them
	int fd0[2], fd1[2], fd2[2], fd3[2], fd4[2];
	if(pipe(fd0)) perror("Pipe0 failed!");
	if(pipe(fd1)) perror("Pipe1 failed!");
	if(pipe(fd2)) perror("Pipe2 failed!");
	if(pipe(fd3)) perror("Pipe3 failed!");
	if(pipe(fd4)) perror("Pipe4 failed!");
	
	// create the child processes
	// Each process MUST have an ID that's between 0 and 4
	// HINT: you may create an ID variable and set its value before fork()
	pid_t pid0=0, pid1=1, pid2=2, pid3=3, pid4=4;
	pid0 = fork();
	if(pid0>0) pid1 = fork();
	if(pid0>0 && pid1>0) pid2 = fork();
	if(pid0>0 && pid1>0 && pid2>0) pid3 = fork();
	if(pid0>0 && pid1>0 && pid2>0 && pid3>0) pid4 = fork();
	
	// child process code
	if(pid0==0 && pid1>0 && pid2>0 && pid3>0 && pid4>0) {
		//child process 0
		//printf("child0");
		close(fd0[WR_END]);
		close(fd1[RD_END]);
		while(read(fd0[RD_END], &mess, sizeof(mess)<=0)) {}
		if(mess.dest == 0) {
			printf("Node %d, Message: %s", mess.dest, mess.mess_str);
		} else {
			write(fd1[WR_END], &mess, sizeof(mess));
		}
		close(fd0[RD_END]);
		close(fd1[WR_END]);
	} else if(pid0==0 && pid1==0 && pid2>0 && pid3>0 && pid4>0){
		//child process 1
		//printf("child1");
		close(fd1[WR_END]);
		close(fd2[RD_END]);
		while(read(fd1[RD_END], &mess, sizeof(mess)<=0)) {}
		if(mess.dest == 1) {
			printf("Node %d, Message: %s", mess.dest, mess.mess_str);
		} else {
			write(fd2[WR_END], &mess, sizeof(mess));
		}
		close(fd1[RD_END]);
		close(fd2[WR_END]);
	} else if(pid0==0 && pid1==0 && pid2==0 && pid3>0 && pid4>0){
		//child process 2
		//printf("child2");
		close(fd2[WR_END]);
		close(fd3[RD_END]);
		while(read(fd2[RD_END], &mess, sizeof(mess)<=0)) {}
		if(mess.dest == 2) {
			printf("Node %d, Message: %s", mess.dest, mess.mess_str);
		} else {
			write(fd3[WR_END], &mess, sizeof(mess));
		}
		close(fd2[RD_END]);
		close(fd3[WR_END]);
	} else if(pid0==0 && pid1==0 && pid2==0 && pid3==0 && pid4>0){
		//child process 3
		//printf("child3");
		close(fd3[WR_END]);
		close(fd4[RD_END]);
		while(read(fd3[RD_END], &mess, sizeof(mess)<=0)) {}
		if(mess.dest == 3) {
			printf("Node %d, Message: %s", mess.dest, mess.mess_str);
		} else {
			write(fd4[WR_END], &mess, sizeof(mess));
		}
		close(fd3[RD_END]);
		close(fd4[WR_END]);
	} else if(pid0==0 && pid1==0 && pid2==0 && pid3==0 && pid4==0){
		//child process 4
		//printf("child4");
		close(fd4[WR_END]);
		while(read(fd4[RD_END], &mess, sizeof(mess)<=0)) {}
		if(mess.dest == 4) {
			printf("Node %d, Message: %s", mess.dest, mess.mess_str);
		} else {
			
		}
		close(fd4[RD_END]);
	} 
	
	// parent code
	if(pid0>0 && pid1>0 && pid2>0 && pid3>0 && pid4>0) {
		get_user_message(&mess);
		close(fd0[RD_END]);
		write(fd0[WR_END], &mess, sizeof(mess));
		close(fd0[WR_END]);
		// parent waits for all children to exit
		wait(NULL);
		// parent closes all pipes
		printf("Parent exiting\n");
		return 0;
	}

	
	
	
}