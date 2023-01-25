#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<unistd.h>
#include<sys/wait.h>

int main(int argc, char *argv[]) {

	const int SIZE = atoi(argv[1]) * sizeof(double);
	const char *name = "lab4q3";
	const char *in = "in";
	const char *out = "out";
	int shm_fd, shm_in, shm_out;
	double *ptr;
	int *pin, *pout;
	
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	shm_in = shm_open(in, O_CREAT | O_RDWR, 0666);
	shm_out = shm_open(out, O_CREAT | O_RDWR, 0666);
	ftruncate(shm_fd, SIZE);
	ftruncate(shm_in, sizeof(int));
	ftruncate(shm_out, sizeof(int));
	ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	pin = mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_in, 0);
	pout = mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_out, 0);
	*pin = 0;
	*pout = 0;
	
	pid_t pid = fork();
	
	if(pid == 0) {
		while(*pin<atoi(argv[1])) {
			printf("Generating %dth number...\n", (*pin)+1);
			ptr[*pin] = (0.5 * *pin) * (0.5 * *pin);
			(*pin)++;
			sleep((float)rand()/(float)(RAND_MAX) * 4.99);
		}
	} else {
		while(*pout<atoi(argv[1])) {
			if(*pin>*pout) {
				printf("The generated %dth number is: %f\n", (*pout)+1, ptr[(*pout)]);
				(*pout)++;
			}
			sleep(0.001);
		}
	}
	
	while(*pin<atoi(argv[1])) {
		printf("%d\n", *pin);
		printf("%d\n", *pout);
		int id = fork();
		if(id == 0) {
			//printf("c");
			ptr[*pin] = (0.5 * *pin) * (0.5 * *pin);
			*pin++;
			//sleep((float)rand()/(float)(RAND_MAX) * 4.99);
			return 0;
		} else {
			if(*pin>*pout) {
				//printf("p");
				printf("%f\n", ptr[(*pout)++]);
			}
		}
	} 
	
	return 0;
}
