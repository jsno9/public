#include <stdio.h>  
#include <string.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <syslog.h>  
#include <errno.h>  
#include <stdlib.h>   
#include <sys/un.h> 
#include <stddef.h>

#define UNIX_DOMAIN "/home/jason/tmp/server.sock" 

int main(int argc , char* argv[])  
{  
	int sock_fd=0;
	char recvbuf[100]={0};
	char sendbuf[100]={0};
	int recvlen=0;
	int sendlen=0;
	char c;
    int len;
/*
	struct sockaddr_in ser_addr;
	memset(&ser_addr,0,sizeof(ser_addr));
	ser_addr.sin_family=AF_INET;
	ser_addr.sin_port=htons(SERVER_PORT);	
	ser_addr.sin_addr.s_addr= inet_addr("127.0.0.1");*/

    struct sockaddr_un un;            
    memset(&un, 0, sizeof(un));            /* fill socket address structure with our address */  
    un.sun_family = AF_UNIX;   
    strcpy(un.sun_path, UNIX_DOMAIN);   
    len = offsetof(struct sockaddr_un, sun_path) + strlen(UNIX_DOMAIN);   
  //  unlink(un.sun_path);               /* in case it already exists */   

	sock_fd= socket(AF_UNIX, SOCK_STREAM, 0);
	if(sock_fd<0)
	{
		printf("socket failed\n");
		exit(1);
	}
	if(connect(sock_fd,(struct sockaddr *)&un, len)<0)
	{
		printf("connect failed\n");
        close( sock_fd); 
		exit(1);
	}
    sprintf(sendbuf, "scktmp%05d", getpid());  
	//sprintf(sendbuf,"he21321asd\n");
	sendlen=strlen(sendbuf)+1;
	sendbuf[sendlen]='\0';
	//printf("%d\n",sendlen);
    while(1)
    {
	    send(sock_fd,sendbuf,sendlen,0);
        printf("%s\n",sendbuf);
	    c=1;
        recvlen=0;
	    while(c!='\0')
	    {		
            //send(sock_fd,sendbuf,sendlen,0);
		    recv(sock_fd,&c,1,0);
		    recvbuf[recvlen]=c;
		    recvlen++;
	    }
	    //recvbuf[recvlen]='\0';
	    printf("%s\n",recvbuf);
        sleep(2);
    }
	
	
}  
