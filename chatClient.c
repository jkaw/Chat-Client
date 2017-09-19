#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include "/usr/local/unp/lib/unp.h"

int main(int argc, char **argv ) {
	int n; //the return number from write
	int sockfd = socket(AF_INET, SOCK_STREAM, 0); //the socket field descriptor
	struct sockaddr_in servaddr; 
	char buffer[256]; //The incoming and outgoing char array containing the messages

	//Setting up the socket and connection abd error checking
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port=htons(49153);
	inet_pton(AF_INET, "pilot.westmont.edu", &servaddr.sin_addr);  
	if (sockfd == -1) {
        	fprintf(stderr, "Socket Failure!!\n");
        	exit(0);
   		 }

	/* Connect to the port */
	//connect(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr));
	int connectStatus = connect(sockfd,(struct sockaddr*) &servaddr, sizeof(servaddr));
	if(connectStatus < 0) {
		printf("Error: Connection failed %s\n", strerror(errno));
		exit(0);
	}
	
	/* This is where the message set up happens */
	char usrname[16]; //the char array storing the username
	printf("Type in your username of up to 15 characters: ");
	fgets(usrname, 15, stdin); //storing the username from the input
	printf("Welcome, %s \n",(char *) &usrname);
	write(sockfd, usrname, strlen(usrname)); //Sending the username to the server

	// This is where the messaging happens
	while(1){
		bzero(buffer, 256); //clear the buffer
		fgets(buffer, 255, stdin);	//get the typing from the terminal
		if(buffer[0] == '\n'){         //check if the entry is empty and if it is, goes into read mode
			bzero(buffer, 256);
			read(sockfd, buffer, 255);
			printf("%s\n",(char*) & buffer);
		}
		else{ //sends the message then reads then loops
			bzero(buffer, 256);
			n = write(sockfd, buffer, strlen(buffer));
			 if(n == 2  && buffer[0] == 'q'){   //checks if q was sent and quits if it was
                       	 exit(0);
               		 }
			bzero(buffer, 256);
			n = read(sockfd, buffer, 255);
			if(n < 0) {
				printf("Error: No message received");
			}
			printf("%s\n",(char*) & buffer);	
			}
		}	
	close(sockfd);
	exit(0);
	
}	
