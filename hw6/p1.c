#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define BUFFER_SIZE 256

int main( int argc, char *argv[]) {
	
	pid_t pid;
	
	pid = fork();
	
	if(pid<0) {
		fprintf(stderr, "Fork error");
		return 1;
	} else if(pid==0) { // child process, server
		printf("SERVER: This is server\n");
		int sockfd, newsockfd, portno;
		socklen_t clilen;
		char buffer[BUFFER_SIZE] = "Hello client, I am server, here is a msg for you.\n";
		struct sockaddr_in serv_addr, cli_addr;
		int n;
		if (argc < 2) {
			fprintf(stderr,"SERVER: ERROR, no port provided\n");
			exit(1);
		}
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0) 
			fprintf(stderr, "SERVER: ERROR opening socket\n");
		bzero((char *) &serv_addr, sizeof(serv_addr));
		portno = atoi(argv[2]);
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = INADDR_ANY;
		serv_addr.sin_port = htons(portno);
		if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
			fprintf(stderr, "SERVER: ERROR on binding\n");
		}
		printf("SERVER: Sleeping...\n");
		sleep((float)rand()/(float)(RAND_MAX) * 4.99);
		printf("SERVER: This is server again!\n");
		listen(sockfd,5);
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0) {
			fprintf(stderr, "SERVER: ERROR on accept\n");
		}
		n = write(newsockfd, buffer, BUFFER_SIZE);
		if (n < 0) {
			fprintf(stderr, "SERVER: ERROR writing to socket\n");
		}
		bzero(buffer, BUFFER_SIZE);
		n = read(newsockfd, buffer, BUFFER_SIZE);
		if(n<0) {
			fprintf(stderr, "SERVER: ERROR reading from socket\n");
		}
		printf("SERVER: %s\n", buffer);
		close(newsockfd);
		close(sockfd);
		
	} else { // parent process, client 
		//sleep(1);
		printf("CLIENT: This is client\n");
		int sockfd, portno, n;
		struct sockaddr_in serv_addr;
		struct hostent *server;
		char buffer[BUFFER_SIZE];
		if (argc < 3) {
			fprintf(stderr,"CLIENT: usage %s hostname port\n", argv[0]);
			exit(0);
		}
		portno = atoi(argv[2]);
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0) {
			fprintf(stderr, "CLIENT: ERROR opening socket\n");
		}
		server = gethostbyname(argv[1]);
		if (server == NULL) {
			fprintf(stderr, "CLIENT: ERROR, no such host\n");
			exit(0);
		}
		bzero((char *) &serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
		serv_addr.sin_port = htons(portno);
		printf("CLIENT: Trying to connect to server...\n");
		while (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {sleep(0.1);}
		printf("CLIENT: Connection succeeded!\n");
		n = read(sockfd, buffer, BUFFER_SIZE);
		if(n<0) {
			fprintf(stderr, "CLIENT: ERROR reading from socket\n");
		}
		printf("CLIENT: %s\n", buffer);
		bzero(buffer, BUFFER_SIZE);
		strcpy(buffer, "Hello server, I am client, I have got the msg. Thank you!\n");
		n = write(sockfd, buffer, BUFFER_SIZE);
		if(n<0) {
			fprintf(stderr, "CLIENT: ERROR wirte to socket\n");
		}
		close(sockfd);
		//wait(NULL);
	}
	
	return 0;
}

