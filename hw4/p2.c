#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
int main() {
	int id = fork();
	if(id != 0) {
		printf("The parent id is %d\n", getpid());
	}
	return 0;
}
