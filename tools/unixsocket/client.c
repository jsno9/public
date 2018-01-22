#include <stdio.h>  
#include <string.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <syslog.h>  
#include <errno.h>  
#include <stdlib.h>   
#include <sys/un.h> 
#include <stddef.h>
#include <signal.h>

#define UNIX_DOMAIN "server.sock" 


unsigned char sendparameter[]={0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                               0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                               0x00,0x00,0x00,0xff,0x04,0x5d,0x00,0x04,
                               0x00,0xff,0xff,0xff,0xff};
void signalcheck(int signo)
{
    printf("ok=%d\n",signo);
}
typedef struct Rtk_Socket_Data
{
    unsigned char            type;      //hci,other,inner
    //unsigned char        opcode;
    unsigned char        opcodeh;
    unsigned char        opcodel;
    unsigned char         parameter_len;
    unsigned char         parameter[0];
}Rtk_Socket_Data;
int main(int argc , char* argv[])  
{  
	int sock_fd=0;
	char recvbuf[100]={0};
	char sendbuf[100]={0};
	int recvlen=0;
	int sendlen=0;
	char c;
    int len;
    int ret;
    int optval;
    int optlen=sizeof(optval);
    int i;
    
    Rtk_Socket_Data *p_buf = (Rtk_Socket_Data *)malloc(sizeof(Rtk_Socket_Data)+29*sizeof(char));
    
    p_buf->parameter_len=sizeof(sendparameter);
    //p_buf->parameter_len=20;
    printf("p_buf->parameter_len=%d\n",p_buf->parameter_len);
    
    p_buf->type=0x01;
    p_buf->opcodeh=0xfc;
    p_buf->opcodel=0x77;
    //p_buf->opcode=0xfc77;
    
    memcpy(p_buf->parameter,sendparameter,p_buf->parameter_len);
    printf("%x\n",p_buf[0]);
    //p_buf->parameter[1]=0xfe;
    //p_buf->end='\0';
    sendlen=sizeof(Rtk_Socket_Data)+29*sizeof(char);
    printf("sendlen=%d\n",sendlen);
    i=0;
    //p_buf = (unsigned char *)p_buf;
  /* while(p_buf[i]!='\0')
    {
        printf("0x%x,\n",p_buf[i]);
        i++;
    }*/
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
    //sprintf(sendbuf, "scktmp%05d", getpid());  
    signal(SIGPIPE,signalcheck);
	//sendlen=strlen(sendbuf)+1;
	//sendbuf[sendlen]='\0';
    /*
    sendbuf[0]=0x01;
    sendbuf[1]=0x03;
    sendbuf[2]=0xaa;
    sendbuf[3]=0x55;
    sendbuf[4]=0x55;
    sendbuf[5]='\0';
    sendlen=6;
    */
	//printf("%d\n",sendlen);
    //while(1)
    //{
        printf("send sock_fd=%d\n",sock_fd);
       // ret=recv(sock_fd,&c,1,MSG_PEEK|MSG_DONTWAIT);
       // printf("check ret1 = %d\n",ret);
       //sendlen=sizeof(Rtk_Socket_Data)+;
	    ret=write(sock_fd,p_buf,sendlen,0);
        printf("send ret = %d\n",ret);
        //printf("%s\n",sendbuf);
	    c=1;
        recvlen=0;
        
	    while(c!='\0')
	    {		
            //send(sock_fd,sendbuf,sendlen,0);
		    ret=recv(sock_fd,&c,1,0);
            //printf("recv ret = %d\n",errno);
		    recvbuf[recvlen]=c;
		    recvlen++;
	    }
	    //recvbuf[recvlen]='\0';
	    for(i=0;i<recvlen;i++)
        {
            printf("0x%x,\n",recvbuf[i]);
        }
        sleep(2);
    //}
	
	
}  
