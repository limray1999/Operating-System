#include<stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
	srand(time(NULL));
	printf("Hello world! This is CS6233 Spring 2022\n");
	printf("%d\n", rand() % 200);
	printf("\n");
	return 0;
}
