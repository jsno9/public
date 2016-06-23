#include <stdio.h>  
#include <string.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <syslog.h>  
#include <errno.h>  
#include <stdlib.h>   
#define SERVER_PORT 1234  
int main(int argc , char* argv[])  
{  
	int sock_fd=0;
	char recvbuf[100]={0};
	char sendbuf[100]={0};
	int recvlen=0;
	int sendlen=0;
	char c;
	struct sockaddr_in ser_addr;
	memset(&ser_addr,0,sizeof(ser_addr));
	ser_addr.sin_family=AF_INET;
	ser_addr.sin_port=htons(SERVER_PORT);	
	ser_addr.sin_addr.s_addr= inet_addr("127.0.0.1");
	sock_fd=socket(AF_INET,SOCK_STREAM,0);
	if(sock_fd<0)
	{
		printf("socket failed\n");
		exit(1);
	}
	if(connect(sock_fd,(struct sockaddr *)&ser_addr,sizeof(ser_addr))<0)
	{
		printf("connect failed\n");
		exit(1);
	}
	sprintf(sendbuf,"he21321asd\n");
	sendlen=strlen(sendbuf)+1;
	sendbuf[sendlen]='\0';
	printf("%d\n",sendlen);
	send(sock_fd,sendbuf,sendlen,0);

	c=1;
	while(c!='\n')
	{		
		recv(sock_fd,&c,1,0);
		recvbuf[recvlen]=c;
		recvlen++;
	}
	recvbuf[recvlen]='\0';
	printf("%s",recvbuf);
	
	
}  
