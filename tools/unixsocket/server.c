#include <stdio.h>  
#include <string.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <stdlib.h>  
#include <syslog.h>  
#include <errno.h> 
#include <sys/un.h> 
#include <stddef.h>
#include <sys/epoll.h>

#define UNIX_DOMAIN "server.sock" 
#define MAX_connect 3

int listen_sock=0;
int client_sock=0;
int len;
pthread_t athread;
pthread_t acceptthd;
int ret;
struct sockaddr_un un;
int epoll_fd;

void *status;

typedef struct Rtk_Socket_Data
{
    unsigned char            type;      //hci,other,inner
    //unsigned short        opcode;
    unsigned char        opcodeh;
    unsigned char        opcodel;
    unsigned char         parameter_len;
    unsigned char         parameter[0];
}Rtk_Socket_Data;

typedef struct Rtk_Service_Data
{
    uint16_t        opcode;
    uint8_t         parameter_len;
    uint8_t         *parameter;
    void            (*complete_cback)(void *);
}Rtk_Service_Data;

void accept_request(int sock)
{
    unsigned char sendbuf[100];
    unsigned char recvbuf[100];
    unsigned char c;
    unsigned short opcode;
    unsigned char opcodeh;
    unsigned char opcodel;
    unsigned short type;
    int i;
    int recvlen,sendlen;
    int client_sock=sock;
    Rtk_Socket_Data *p_buf=NULL;
    Rtk_Service_Data *data_buf = (Rtk_Service_Data *)malloc(sizeof(Rtk_Service_Data));
    unsigned char *abuf = NULL;
    printf("accept_request\n");
    //char *a=(char *)malloc(sizeof(char));
    //printf("a addr = %u\n",a);
    //free(a);
   
    //Rtk_Service_Data *RtkData;
     //while(1)
     //{
        /*
        recvlen=0;
        c=1;
        while(c!='\0')
        {
            recv(client_sock,&c,1,0);
            recvbuf[recvlen]=c;
            recvlen++;
            printf("%x\n",c);
        }*/
        //recvbuf[recvlen]='\0';
        recvlen=0;
        c=1;
        recv(client_sock,&type,1,0);
        //printf()
        if(type==0x01)
        {
            recv(client_sock,&opcodeh,1,0);
            recv(client_sock,&opcodel,1,0);
            recv(client_sock,&c,1,0);
            printf("opcode=%x,len=%d\n",opcode,c);
        }
        p_buf=(Rtk_Socket_Data *)malloc(sizeof(Rtk_Socket_Data)+sizeof(char)*c);
        p_buf->type=type;
        p_buf->opcodeh=opcode;
        p_buf->parameter_len=c;
        printf("type=%d,opcode=%x,parameter_len=%d\n",p_buf->type,p_buf->opcodeh,p_buf->parameter_len);
        abuf=(unsigned char *)malloc(sizeof(char)*p_buf->parameter_len);
        ret=read(client_sock,abuf,p_buf->parameter_len);
        
        printf("recvlen=%d\n",ret);
       /* for(i=0;i<p_buf->parameter_len;i++)
        {
            ret=read(client_sock,&p_buf->parameter[i],1);
            printf("recvlen=%d\n",ret);
        }
        for(i=0;i<p_buf->parameter_len;i++)
        {
            printf("0x%x,\n",p_buf->parameter[i]);
        }
        printf("done %u\n",p_buf);
        */
        data_buf->opcode = opcodeh<<8 | opcodel;
        printf("opcode = %x\n",data_buf->opcode);
        data_buf->parameter_len=p_buf->parameter_len;
        data_buf->parameter = abuf;
        //free(p_buf);
        free(p_buf);
        //free(p_buf->parameter);
        p_buf=NULL;
        for(i=0;i<data_buf->parameter_len;i++)
        {
            printf("0x%x,\n",data_buf->parameter[i]);
        }
        free(data_buf->parameter);
        printf("done2\n");
        //RtkData = (Rtk_Service_Data *)recvbuf;
        //printf("%x,length=%d,end=%c,len=%d\n",RtkData->type,RtkData->cmdlength,RtkData->end,strlen(RtkData));
        sprintf(sendbuf,"host has got %s",recvbuf);

        sendlen=strlen(sendbuf)+1;
        sendbuf[sendlen]='\0';
        
        write(client_sock,sendbuf,sendlen);
        printf("%s\n",sendbuf);
        //close(client_sock);
    // }

}

void accept_thread()
{
    struct epoll_event events[64];
    struct epoll_event event;
    int nfds=0;
    int i=0;
    
    while(1)
    {
        nfds=epoll_wait(epoll_fd,events,32,1000);
        //printf("aaa=%d\n",nfds);
        if(nfds>0)
        {
            for(i=0;i<nfds;i++)
            {
                printf("events[i].events = %x,events[i].data.fd=%d\n",events[i].events,events[i].data.fd);
                if(events[i].data.fd == listen_sock && events[i].events&EPOLLIN)
                {
                    client_sock=accept(events[i].data.fd, (struct sockaddr *)&un, &len);
                    if(client_sock<0)
                    {
                        printf("accept failed\n");
                        pthread_exit(0);
                    }
                    //pthread_create(&connectthread,NULL,(void *)accept_request_thread,&client_sock);

                    event.data.fd=client_sock;
                    event.events=EPOLLIN | EPOLLHUP | EPOLLRDHUP | EPOLLERR;
                    //list_add(client_sock);
                    epoll_ctl(epoll_fd,EPOLL_CTL_ADD,client_sock,&event);
                }
                else if(events[i].events&(EPOLLIN | EPOLLHUP | EPOLLRDHUP | EPOLLERR))
                {
                    accept_request(events[i].data.fd);
                }
            }
        }
        printf("ok\n");
    }
}

int main(int argc , char* argv[])  
{
    struct epoll_event event;

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

    epoll_fd = epoll_create(64);
    event.data.fd=listen_sock;
    event.events=EPOLLIN;
    epoll_ctl(epoll_fd,EPOLL_CTL_ADD,listen_sock,&event);
    pthread_create(&acceptthd,NULL,(void *)accept_thread,NULL);
     sleep(5);
     pthread_join(acceptthd,&status);
        //pthread_cancel(acceptthd);
    /*while(1)
    {
       

        client_sock=accept(listen_sock, (struct sockaddr *)&un, &len);
        if(client_sock<0)
        {
            printf("accept failed\n");
            exit(1);
        }
        pthread_create(&athread,NULL,(void *)accept_request,&client_sock);


    }*/
    //return 0;

}

