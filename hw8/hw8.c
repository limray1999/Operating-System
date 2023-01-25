#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int n;
int k;

int FIFO(int pageTrace[], int frameSize) {
	int frame[frameSize];
	for(int i=0; i<frameSize; i++) {
	    frame[i] = -1;
	}
	int pullIndex = 0;
	int pageFault = 0;
	for(int i=0; i<n; i++) {
		int j;
		for(j=0; j<frameSize; j++) {
			if(frame[j]==pageTrace[i]) {
				break;
			}
		}
		if(j==frameSize) {
			if(pullIndex>=frameSize) pageFault++;
			frame[pullIndex%frameSize] = pageTrace[i];
			pullIndex++;
		}
	}
	return pageFault;
}

int main(int argc, void* argv[]) {
	n = atoi(argv[1]);
	k = atoi(argv[2]);
	int pageTrace[n];
	unsigned int rand_state = (unsigned int) time(NULL);
	int random = rand_r(&rand_state) % k;
	pageTrace[0] = random;
	for(int i=1; i<n; i++) {
		while(1) {
			random = rand_r(&rand_state) % k;
			if(random != pageTrace[i-1]) {
				pageTrace[i] = random;
				break;
			}
		}
	}
	//for(int i=0; i<n; i++) {
	//	printf("%d ", pageTrace[i]);
	//}
	//printf("\n");
	int pageFault[k];
	for(int i=1; i<=k; i++) {
		pageFault[i-1] = FIFO(pageTrace, i);
	}
	printf("Number of page fault (frame size from 1 to 16): ");
	for(int i=0; i<k; i++) {
		printf("%d ", pageFault[i]);
	}
	printf("\n");
	return 0;
}