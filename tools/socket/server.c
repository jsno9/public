#include <stdio.h>  
#include <string.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <stdlib.h>  
#include <syslog.h>  
#include <errno.h> 

#define LISTEN_PORT 1234

void accept_request(int client_sock)
{
	unsigned char sendbuf[100];
	unsigned char recvbuf[100];
	unsigned char c;
	int recvlen,sendlen;
	recvlen=0;
	c=1;		
	while(c!='\n')
	{		
		recv(client_sock,&c,1,0);
		recvbuf[recvlen]=c;
		recvlen++;
	}
	recvbuf[recvlen]='\0';
	printf("%s",recvbuf);
	sprintf(sendbuf,"host has got %s\n",recvbuf);
	
	sendlen=strlen(sendbuf)+1;
	sendbuf[sendlen]='\0';
	printf("%d\n",sendlen);
	send(client_sock,sendbuf,sendlen,0);
	
}

int main(int argc , char* argv[])  
{
	int listen_sock=0;
	int client_sock=0;
	
	pthread_t athread;	
	int ret;
	struct sockaddr_in hostaddr;
	struct sockaddr_in clientaddr;
	int socklen=sizeof(clientaddr);
	memset((void *)&hostaddr,0,sizeof(hostaddr));
 	memset((void *)&clientaddr, 0, sizeof(clientaddr));  
	hostaddr.sin_family=AF_INET;
	hostaddr.sin_port=htons(LISTEN_PORT);
	hostaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	listen_sock=socket(AF_INET,SOCK_STREAM,0);
	if(listen_sock<0)
	{
		printf("creat socket failed\n");
		exit(1);
	}
	if(bind(listen_sock,(struct sockaddr *)&hostaddr,sizeof(hostaddr))<0)
	{
		printf("bind socket failed\n");
		exit(1);
	}
	if(listen(listen_sock,5)<0)
	{
		printf("listen failed\n");
		exit(1);
	}	

	while(1)
	{
	
		client_sock=accept(listen_sock,(struct sockaddr *)&clientaddr,&socklen);
		if(client_sock<0)
		{
			printf("accept failed\n");
			exit(1);
		}
		pthread_create(&athread,NULL,(void *)accept_request,client_sock);
	

	}
	//return 0;

}



