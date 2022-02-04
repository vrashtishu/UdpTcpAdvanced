#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>

int main(int argc,char *argv[])
{
	int socket_desc,n,valread;
	char *buffer;
	struct sockaddr_in server,client;
	
	if(argc<3)
	{
		printf("Please Send Message for sending and port Number\n");
		exit(1);	
	}
	//create a socket
	socket_desc=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if(socket_desc==-1)
	{
		printf("Could Not Create Scoket");
		exit(1);
	}
	//Bind address and port no to socket with sockaddr_in structure
	server.sin_family=AF_INET;
	server.sin_port=htons(atoi(argv[1]));
	server.sin_addr.s_addr=inet_addr("10.0.2.15");
	
	n=sizeof(server);
	
	buffer=argv[2];
	valread=sendto(socket_desc,(const char *)buffer,strlen(buffer),0,(const struct sockaddr *) &server,n);
	if(valread<0)
	{
		printf("Error in Sending\n");
		exit(1);
	}
	printf("Message Sending Successfully\n");
		
	close(socket_desc);
	return 0;	
}
