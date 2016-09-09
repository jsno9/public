#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>//epoll
#include <linux/input.h>
#include <fcntl.h>


struct epoll_event ev,events[99];
static int epollfd,nfds,len;
static unsigned char keybuff[10]={0};
static unsigned char mousebuff[5]={0};
static int fd[12];
//int fb;
int main()
{
	int i;
	char buf[100]={0};
  	

	epollfd=epoll_create(12);
	
	struct input_event ie;

	for(i=0;i<12;i++)
	{
		sprintf(buf,"/dev/input/event%d",i);
		fd[i]=open(buf,O_RDWR);
		if(fd[i]<0)
		{
			printf("can not open /dev/input/event%d\n",i);
		}
		ev.data.fd=fd[i];
		ev.events=EPOLLIN;
		epoll_ctl(epollfd,EPOLL_CTL_ADD,fd[i],&ev);
	}

	while(1)
	{
		nfds=epoll_wait(epollfd,events,12,-1);
		//printf("aaa=%d\n",nfds);
			for(i=0;i<nfds;i++)
			{
				if(events[i].events&EPOLLIN)
				{
					read(events[i].data.fd,&ie,sizeof(struct input_event));
					printf("time %ld.%06ld\ttype %d\tcode %d   \tvalue %d\n",
		           		ie.time.tv_sec, ie.time.tv_usec, ie.type, ie.code, ie.value);
					
				}
			}
	}
}
