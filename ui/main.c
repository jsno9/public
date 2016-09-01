#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>		//log

#include<fcntl.h>		//	open
#include<unistd.h>		//	close
#include<linux/fb.h>		//	framebuffer
#include<termios.h>		//	termios,getchar
#include <sys/epoll.h>//epoll
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

#define esc_key 0x1b
#define nofun_key 0xff
#define up_key 0x41
#define down_key 0x42
#define right_key 0x43
#define left_key 0x44


#define no_event 0xff
#define exit_event 0x1b
//physical info
static char* rawspace=0;
static u64 fbaddr=0;
static int fbtotalbyte=0;
static int fboneline=0;

//virtual info
static int xmax=0;
static int ymax=0;
static int bpp=0;

//自己的画板
static int fb=-1;
static char* screenbuf;
//static int width=1024;
//static int height=768;
///epoll
struct epoll_event ev,events;
int epollfd,nfds,len;

//输入
static u8 keybuff[10]={0};
static int signal=-1;
static struct termios old;
static struct termios new;


//
__attribute__((aligned(0x1000))) static u8 pixbuf[2048*1024*32];
static char pixfmt[8] = {'b','g','r','a','8','8','8','8'};
static int width=512;
static int height=512;
///



void log(const char *format, ...)
{
    va_list arg;

    va_start (arg, format);
	vfprintf (stdout, format, arg);
    va_end (arg);

}

int keyboardevent()
{
	unsigned char a;
	nfds=epoll_wait(epollfd,&events,1,100);
	if(nfds!=0)
	{
		len=read(STDIN_FILENO,keybuff,sizeof(keybuff));
		switch(len)
		{
			case 1:if(keybuff[0]==0x1b)
					{
						return esc_key;
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

void init()
{
	int size=4*0x400000;
	pthread_t keythread;
	rawspace=malloc(size+0x1000);	

	signal=tcgetattr(STDIN_FILENO,&old);
	new=old;
	new.c_lflag&=~(ICANON|ECHO);
	tcsetattr(STDIN_FILENO,TCSANOW,&new);
	fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
	

	ev.data.fd=STDIN_FILENO;
	ev.events=EPOLLIN|EPOLLET;

	epollfd=epoll_create(2);
	epoll_ctl(epollfd,EPOLL_CTL_ADD,STDIN_FILENO,&ev);

}

void uninit()
{
	free(rawspace);
}

void windowcreate()
{
	fb=open("/dev/fb0",O_RDWR);
	if(fb<0)
	{
		    log("can not open /dev/fb0\n");
		    exit(-1);
	}
	
	struct fb_fix_screeninfo finfo;
	if(ioctl(fb,FBIOGET_FSCREENINFO,&finfo))
	{
		log("can not get finfo!\n");
		exit(-1);
	}
	
	fbaddr=finfo.smem_start;
	fbtotalbyte=finfo.smem_len;
	fboneline=finfo.line_length;
	log("fbaddr=0x%llx,fbtotalbyte=0x%x,fboneline=0x%x\n"
		,fbaddr,fbtotalbyte,fboneline);
	
	struct fb_var_screeninfo vinfo;
	if(ioctl(fb,FBIOGET_VSCREENINFO,&vinfo))
	{
		log("can not get vinfo\n");
		exit(-1);
	}

	xmax=vinfo.xres;
	ymax=vinfo.yres;
	bpp=vinfo.bits_per_pixel;
	printf("xmax=%d,ymax=%d,bpp=%d\n",xmax,ymax,bpp);
	
	

}

void windowstart(char* addr, char* pixfmt, int x, int y)
{
	screenbuf = addr;
	width = x;
	height = y;
}

void windowwrite()
{
	//
	int x,y,ret;

	//5,6,5
	if(bpp==16)
	{
		for(x=0;x<width*height;x++)
		{
			*(unsigned short*)(screenbuf+x*2)=
				    (screenbuf[x*4+0]>>3)
				+ ( (screenbuf[x*4+1]>>2) <<  5 )
				+ ( (screenbuf[x*4+2]>>3) << 11 );
		}
	}
	//
	for(y=0;y<height;y++)
	{
		ret=lseek(fb , y*fboneline , SEEK_SET);
		ret=write(fb , screenbuf + y*width*bpp/8 , width*bpp/8);
	}
}

int getevent()
{
	int type;
	type=keyboardevent();
	if(type==0xff)
	{
		return no_event;	
	}
	else
	{
		return type;
	}


}

void updatepanel(int type)
{
	int i;
	for(i=0;i<width*height*bpp/8;i=i+4)
	{
		//log("for\n");
		if(type==up_key)
		{
			if((pixbuf[i]+4)<=0xff)
			{			
				pixbuf[i]=pixbuf[i]+4;
			}
			else if((pixbuf[i+1]+4)<=0xff)
			{
				pixbuf[i+1]=pixbuf[i+1]+4;
			}
			else if((pixbuf[i+2]+4)<=0xff)
			{
				pixbuf[i+2]=pixbuf[i+2]+4;
			}
			
		}
		else if(type==down_key)
		{
			if((pixbuf[i+2]-4)>=0)
			{			
				pixbuf[i+2]=pixbuf[i+2]-4;
			}
			else if((pixbuf[i+1]-4)>=0)
			{
				pixbuf[i+1]=pixbuf[i+1]-4;
			}
			else if((pixbuf[i]-4)>=0)
			{
				pixbuf[i]=pixbuf[i]-4;
			}	

		}
		else if(type==right_key)
		{
			if((pixbuf[i+3]+4)<=0xff)
			{			
				pixbuf[i+3]=pixbuf[i+3]+4;
			}
		}
		else if(type==left_key)
		{
			if((pixbuf[i+3]-4)>=0)
			{			
				pixbuf[i+3]=pixbuf[i+3]-4;
			}
		}
	

	}
	
}

void windowdelete()
{
	//close(inputfp);
	old.c_lflag|=(ICANON|ECHO);
	if(signal!=-1)tcsetattr(STDIN_FILENO,TCSANOW,&old);

	//
	if(fb!=-1)close(fb);
}

void main()
{
	log("hello world\n");
	int i,j,type;
	init();
	windowcreate(); 
	
	windowstart(pixbuf, pixfmt, xmax, ymax);

	for(i=0;i<width*height*bpp/8;i=i+4)
	{
		//log("for\n");
		pixbuf[i]=0;
		pixbuf[i+1]=0;
		pixbuf[i+2]=0;
		pixbuf[i+3]=0xff;
	}

	while(1)
	{
		
		
		windowwrite();
		type=getevent();
		if(type==exit_event)
		{
			break;
		}
		updatepanel(type);
		
		
	}
	windowdelete();
	log("end\n");
}



























