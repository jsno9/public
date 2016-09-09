#include <sys/epoll.h>//epoll
#include "event.h"
#include "../console/console.h"
#include<unistd.h>

struct epoll_event ev,events;
static int epollfd,nfds,len;
static unsigned char keybuff[10]={0};
static unsigned char mousebuff[5]={0};
static int mousefd;
static void init()
{
	

	ev.data.fd=STDIN_FILENO;
	ev.events=EPOLLIN|EPOLLET;

	epollfd=epoll_create(2);
	epoll_ctl(epollfd,EPOLL_CTL_ADD,STDIN_FILENO,&ev);

	fd=open("/dev/input/mice",O_RDWR);
	if(fd<0)
	{
		loge("can not open /dev/input/mice\n");
	}
	epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&ev);

}

static void uninit()
{
	
}

static int keyboardevent(int nfds)
{
	unsigned char a;
	
	if(nfds==1)
	{
		len=read(STDIN_FILENO,keybuff,sizeof(keybuff));
		switch(len)
		{
			case 1:if(keybuff[0]==0x1b)
					{
						return esc_key;
					}
					else if(keybuff[0]==0x2b)
					{
						return plus_key;
					}
					else if(keybuff[0]==0x2d)
					{
						return sub_key;
					}
					else
					{
						return nofun_key;
					}
			case 2:return nofun_key;
			case 3:if(keybuff[0]==0x1b)
					{
						if(keybuff[1]==0x5b)
						{
							if(keybuff[2]==0x41)
							{
								return up_key;
							}
							else if(keybuff[2]==0x42)
							{
								return down_key;
							}
							else if(keybuff[2]==0x43)
							{
								return right_key;
							}
							else if(keybuff[2]==0x44)
							{
								return left_key;
							}
							else
							{
								return nofun_key;
							}
						}
						else//0x5b
						{
							return nofun_key;	
						}
					}
					else
					{
						return nofun_key;
					}
			default:return nofun_key;
		}
	}
	
}

static int mouseevent(int nfds)
{
	if(nfds==2)
	{
		len=read(STDIN_FILENO,mousebuff,sizeof(mousebuff));
		
	}
}

int epollevent()
{
	int a;
	nfds=epoll_wait(epollfd,&events,1,100);
	a=keyboardevent(nfds);
	return a;
}

void epollevent_create(struct event *eve)
{

	eve->id=1;

	eve->init=init;	

	eve->uninit=uninit;
	
	
	eve->subevent=epollevent;
}


