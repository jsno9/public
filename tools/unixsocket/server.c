#include <stdio.h>  
#include <string.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <stdlib.h>  
#include <syslog.h>  
#include <errno.h> 
#include <sys/un.h> 
#include <stddef.h>

#define UNIX_DOMAIN "/home/jason/tmp/server.sock" 
#define MAX_connect 3

void accept_request(int client_sock)
{
	unsigned char sendbuf[100];
	unsigned char recvbuf[100];
	unsigned char c;
	int recvlen,sendlen;

    while(1)
    {
	    recvlen=0;
	    c=1;		
	    while(c!='\0')
	    {		
		    recv(client_sock,&c,1,0);
		    recvbuf[recvlen]=c;
		    recvlen++;
            //printf("%c\n",c);
	    }
	    //recvbuf[recvlen]='\0';
	    printf("%s\n",recvbuf);
	    sprintf(sendbuf,"host has got %s",recvbuf);
	
	    sendlen=strlen(sendbuf)+1;
	    //sendbuf[sendlen]='\0';
	    
	    send(client_sock,sendbuf,sendlen,0);
        printf("%s\n",sendbuf);
    }
	
}

int main(int argc , char* argv[])  
{
	int listen_sock=0;
	int client_sock=0;
	int len;
	pthread_t athread;	
	int ret;
    struct sockaddr_un un;  

	listen_sock=socket(AF_UNIX, SOCK_STREAM, 0);
	if(listen_sock<0)
	{
		printf("creat socket failed\n");
		exit(1);
	}

    unlink(UNIX_DOMAIN);
    memset(&un, 0, sizeof(un));   
    un.sun_family = AF_UNIX;   
    strcpy(un.sun_path, UNIX_DOMAIN);   
    len = offsetof(struct sockaddr_un, sun_path) + strlen(UNIX_DOMAIN);

	if(bind(listen_sock,(struct sockaddr *)&un, len)<0)
	{
		printf("bind socket failed\n");
		exit(1);
	}

	if(listen(listen_sock,MAX_connect)<0)
	{
		printf("listen failed\n");
		exit(1);
	}	

	while(1)
	{
	
		client_sock=accept(listen_sock, (struct sockaddr *)&un, &len);
		if(client_sock<0)
		{
			printf("accept failed\n");
			exit(1);
		}
		pthread_create(&athread,NULL,(void *)accept_request,client_sock);
	

	}
	//return 0;

}

