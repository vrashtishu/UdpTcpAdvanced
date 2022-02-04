#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<string.h>

int main(int argc,char *argv[])
{
	if(argc<3)
	{
		printf("Please Give 2 Port Numbers");
		exit(1);
	}
	int socket_desc1,socket_desc2,socket_desc3,socket_desc4,n,valread,ret;
	int new_socket1,new_socket2;
	char buffer[1024]={0};
	char *buffer1;
	struct sockaddr_in server1,server2,server3,server4,client;
	struct timeval tval;
	fd_set read_set,write_set;
	int maxfd;
	int bytes,buflen;
	
	//create four socket
	socket_desc1=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if(socket_desc1==-1)
	{
		printf("Could Not Create Socket 1");
		exit(1);
	}
	socket_desc2=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if(socket_desc2==-1)
	{
		printf("Could Not Create Socket 2");
		exit(1);
	}
	socket_desc3=socket(AF_INET,SOCK_STREAM,0);
	if(socket_desc3==-1)
	{
		printf("Could Not Create Socket 3");
		exit(1);
	}
	socket_desc4=socket(AF_INET,SOCK_STREAM,0);
	if(socket_desc4==-1)
	{
		printf("Could Not Create Socket 4");
		exit(1);
	}
	//Bind address and port no to socket with sockaddr_in structure
	server1.sin_family=AF_INET;
	server1.sin_port=htons(atoi(argv[1]));
	server1.sin_addr.s_addr=INADDR_ANY;

	server2.sin_family=AF_INET;
	server2.sin_port=htons(atoi(argv[2]));
	server2.sin_addr.s_addr=INADDR_ANY;

	server3.sin_family=AF_INET;
	server3.sin_port=htons(atoi(argv[1]));
	server3.sin_addr.s_addr=INADDR_ANY;

	server4.sin_family=AF_INET;
	server4.sin_port=htons(atoi(argv[2]));
	server4.sin_addr.s_addr=INADDR_ANY;


	if(bind(socket_desc1, (struct sockaddr *)&server1, sizeof(server1))<0)
	{
		printf("Bind Failed for socket 1");
		exit(1);		
	}

	if(bind(socket_desc2, (struct sockaddr *)&server2, sizeof(server2))<0)
	{
		printf("Bind Failed for socket 2");
		exit(1);		
	}
	if(bind(socket_desc3, (struct sockaddr *)&server3, sizeof(server3))<0)
	{
		printf("Bind Failed for socket 3");
		exit(1);		
	}

	if(bind(socket_desc4, (struct sockaddr *)&server4, sizeof(server4))<0)
	{
		printf("Bind Failed for socket 4");
		exit(1);		
	}	
	//Start Listening for connections
	if(listen(socket_desc3,3)<0)
	{
		printf("Listen Failed\n");
		exit(1);		

	}
	if(listen(socket_desc4,3)<0)
	{
		printf("Listen Failed\n");
		exit(1);		

	}
	
	n=sizeof(client);
	
	if(socket_desc1>socket_desc2)
		maxfd=socket_desc1;
	else if(socket_desc2>socket_desc1)
		maxfd=socket_desc2;

	if(maxfd<socket_desc3)
		maxfd=socket_desc3;
	if(maxfd<socket_desc4)
		maxfd=socket_desc4;
	
	while(1)
	{
		
		FD_ZERO(&read_set);
		FD_ZERO(&write_set);

		FD_SET(socket_desc1, &read_set);
		FD_SET(socket_desc2, &read_set);
		FD_SET(socket_desc3, &read_set);
		FD_SET(socket_desc4, &read_set);

		FD_SET(socket_desc1, &write_set);
		FD_SET(socket_desc2, &write_set);
		FD_SET(socket_desc3, &write_set);
		FD_SET(socket_desc4, &write_set);

		tval.tv_sec=0;
		ret=select(maxfd+1,&read_set,&write_set,NULL,&tval);
		if(ret<0)
		{
			printf("Select Thrown an Exception\n");
			exit(1);
		}		
		else if(FD_ISSET(socket_desc1,&read_set))
		{
			valread=recvfrom(socket_desc1,(char *)buffer,1024,0,(struct sockaddr *)&client,&n);
			if(valread<0)
			{
				printf("Error in Receiving\n");
				exit(1);
			}
			buffer[valread]='\0';
			printf("Received a UDP datagram in port %s:%s\n\n",argv[1],buffer);
		}
		else if(FD_ISSET(socket_desc2,&read_set))
		{
			valread=recvfrom(socket_desc2,(char *)buffer,1024,0,(struct sockaddr *)&client,&n);
			if(valread<0)
			{
				printf("Error in Receiving\n");
				exit(1);
			}
			buffer[valread]='\0';
			printf("Received a UDP datagram in port %s:%s\n\n",argv[2],buffer);
		}
		else if(FD_ISSET(socket_desc3,&read_set))
		{
			new_socket1=accept(socket_desc3,(struct sockaddr *)&client,(socklen_t *)&n);
			if(new_socket1<0)
			{
				printf("Accept Failed");
				exit(1);
			}
			bytes=recv(new_socket1,(char *)&buflen,sizeof(buflen),0);
			if(bytes<0)
			{
				printf("Error in Receiving size\n");
				exit(1);
			}
			printf("In TCP %d bytes still left to be read\n",buflen);
			buffer1=(char *)malloc(buflen+1);
			bytes=recv(new_socket1,buffer1,buflen,0);
			if(bytes<0)
			{
				printf("Error in Receiving data\n");
				exit(1);
			}
			buffer1[buflen]='\0';
			printf("Received a TCP Packet in %s of %d bytes:%s\n\n",argv[1],buflen,buffer1);
		}
		else if(FD_ISSET(socket_desc4,&read_set))
		{
			new_socket2=accept(socket_desc4,(struct sockaddr *)&client,(socklen_t *)&n);
			if(new_socket2<0)
			{
				printf("Accept Failed");
				exit(1);
			}
			bytes=recv(new_socket2,(char *)&buflen,sizeof(buflen),0);
			if(bytes<0)
			{
				printf("Error in Receiving size\n");
				exit(1);
			}
			printf("In TCP %d bytes still left to be read\n",buflen);
			buffer1=(char *)malloc(buflen+1);
			bytes=recv(new_socket2,buffer1,buflen,0);
			if(bytes<0)
			{
				printf("Error in Receiving data\n");
				exit(1);
			}
			buffer1[buflen]='\0';
			printf("Received a TCP Packet in %s of %d bytes:%s\n\n",argv[2],buflen,buffer1);
		}
	}
	close(socket_desc1);
	close(socket_desc2);
	return 0;	
}
