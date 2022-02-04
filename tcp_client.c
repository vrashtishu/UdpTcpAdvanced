#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<string.h>
int main(int argc,char *argv[])
{
	if(argc<3)
	{
		printf("Please Send Message for sending and port Number\n");
		exit(1);	
	}
	int socket_desc,valread,bytes;
	int datalen=strlen(argv[2]);
	
	struct sockaddr_in server;
	
	//create a socket
	socket_desc=socket(AF_INET,SOCK_STREAM,0);
	if(socket_desc==-1)
	{
		printf("Could Not Create Scoket");
		exit(1);
	}
	//Bind address and port no to socket with sockaddr_in structure
	server.sin_family=AF_INET;
	server.sin_port=htons(atoi(argv[1]));
	server.sin_addr.s_addr=inet_addr("10.0.2.15");

	if(connect(socket_desc, (struct sockaddr *)&server, sizeof(server))<0)
	{
		printf("Connection Failed");
		exit(1);		
	}
	

	//now send size from client
	bytes=send(socket_desc,(char *)&datalen,sizeof(datalen),0);
	if(bytes<0)
	{
		printf("Error Sending size of message\n");
		return 1;
	}
	//now send actual data to server
	bytes=send(socket_desc,argv[2],datalen,0);	
	if(bytes<0)
	{
		printf("Error Sending data\n");
		return 1;
	}
	printf("%d bytes written\n",bytes);
	
	return 0;	
}
