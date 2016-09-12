#include <sys/epoll.h>//epoll
#include "event.h"
#include "../console/console.h"
#include<unistd.h>
#include <fcntl.h>
#include <linux/input.h>

struct epoll_event ev,events[99];
static int epollfd,nfds,len;
static unsigned char keybuff[10]={0};
static unsigned char mousebuff[5]={0};
static int mousefd;
static int fd[12];
static void init()
{
	int i;
	char buf[100]={0};
	ev.data.fd=STDIN_FILENO;
	ev.events=EPOLLIN|EPOLLET;

	epollfd=epoll_create(15);
	epoll_ctl(epollfd,EPOLL_CTL_ADD,STDIN_FILENO,&ev);


	mousefd=open("/dev/input/mice",O_RDWR);
	ev.data.fd=mousefd;
	ev.events=EPOLLIN;
	epoll_ctl(epollfd,EPOLL_CTL_ADD,mousefd,&ev);

	

}

static void uninit()
{
	
}

static int keyboardevent()
{
	unsigned char a;
	
	//if(nfds==1)
	//{
		
		len=read(STDIN_FILENO,keybuff,sizeof(keybuff));
		//loge("len=%x\n",len);
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
	//}
	
}

//鼠标事件，32位返回值
//24~31:01为左移事件，02为右移事件
//16~23:左右移动多少位置
//8~15:01为下移事件，02为上移事件
//0~7:上下移动位置
static int mouseevent()
{
	unsigned int a;
	int b;
	len=read(mousefd,keybuff,sizeof(keybuff));
	loge("%x,%x,%x\n",keybuff[0],keybuff[1],keybuff[2]);
	a=0;b=0;	
	if(keybuff[1]<=0x1f)
	{
		a=(0x02<<8)|keybuff[1];		
	}
	else
	{
		b=0xff-keybuff[1]+1;
		a=(0x01<<8)|b;
	}

	if(keybuff[2]<=0x1f)
	{
		a=(a<<16)|(0x02<<8)|keybuff[2];
	}
	else
	{
		b=0xff-keybuff[2]+1;
		a=(a<<16)|(0x01<<8)|b;
	}
loge("%x\n",a);
	return a;
	
	
}

int epollevent()
{
	int i;
	nfds=epoll_wait(epollfd,events,15,100);
	if(nfds==0)
	{
		return no_event;
	}
	else
	{
		for(i=0;i<nfds;i++)
		{
			if(events[i].events&EPOLLIN)
			{
				//loge("%x,%x\n",events[i].data.fd,STDIN_FILENO);
				if(events[i].data.fd==STDIN_FILENO)
				{
					return keyboardevent();
				}
				else
				{
					//read(events[i].data.fd,&ie,sizeof(struct input_event));
					return  mouseevent();
				}
			}
		}
	}
	
}

void epollevent_create(struct event *eve)
{

	eve->id=1;

	eve->init=init;	

	eve->uninit=uninit;
	
	
	eve->subevent=epollevent;
}


