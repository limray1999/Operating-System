#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
int main() {
	pid_t pid1=33, pid2=22, pid3=11;
	pid1=fork();
	if(pid1>0) pid2=fork();
	if(pid2==0) pid3=fork();
	printf("%d, %d, %d\n", pid1, pid2, pid3);
	return 0;
}
