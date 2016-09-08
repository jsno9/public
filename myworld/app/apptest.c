
#include "../console/console.h"
#include "../event/event.h"
#include "app.h"
#include <stdio.h>
//#include<fcntl.h>	
//#include<unistd.h>


static void init(struct window *win)
{
		
}

static void uninit(struct window *win)
{

}

static void start(struct window *win)
{
	unsigned int x,y,j,k,color,temp;
	int width,height,bpp;
	unsigned int* winbuf;

	winbuf=(unsigned int*)(win->screenbuf);
	width=*(win->width);
	height=*(win->height);
	bpp=*(win->bpp);	
	color=0xff000000;

	for(x=0;x<256;x++)
	{
		for(y=0;y<256;y++)
		{
			for(j=0;j<4;j++)
			{
				for(k=0;k<4;k++)
				{
					winbuf[((x*4+j)*width) + (y*4+k)]=color;
				}
			}
			color+=0x01;						
		}
		color=(color&0xffff0000)+(x<<8);	
	}
	
	temp=winbuf[512*width+512];
	winbuf[512*width+512]=0xffffffff;
	rect(win,1256,0,1512,256,temp,temp);	
	printhexadecimal(win,1256,257,3,temp,0xffffffff,0xff000000);

}

static int update(struct window *win,unsigned int type)
{
	unsigned int x,y,j,k,pointcolor;
	static unsigned int color=0xff010000;
	static unsigned int pointx=512;
	static unsigned int pointy=512;
	int width,height,bpp;
	unsigned int* winbuf;	
	unsigned int temp=0x00010000;
	winbuf=(unsigned int*)(win->screenbuf);
	width=*(win->width);
	height=*(win->height);
	bpp=*(win->bpp);


	switch (type)
	{
		case plus_key:		
			if((color&0xffff0000)==0xffff0000)
			{
				
				break;				
			}
			color=color+temp;break;
		case sub_key:	
			if((color|0xff000000)==0xff000000)
			{
				break;				
			}
			color=color-temp;break;
		case down_key:
			{
				if(pointy==1023)
				{
					break;
				}
				pointy++;	
				break;
			}
		case up_key:
			{
				if(pointy==0)
				{
					break;
				}
				pointy--;	
				break;
			}
		case left_key:
			{
				if(pointx==0)
				{
					break;
				}
				pointx--;	
				break;
			}
		case right_key:
			{
				if(pointx==1023)
				{
					break;
				}
				pointx++;	
				break;
			}
		default :return;
	}	
	color=color&0xffff0000;
	for(x=0;x<256;x++)
	{
		for(y=0;y<256;y++)
		{
			for(j=0;j<4;j++)
			{
				for(k=0;k<4;k++)
				{
					winbuf[((x*4+j)*width) + (y*4+k)]=color;
				}
			}
			color+=0x01;						
		}
		color=(color&0xffff0000)+(x<<8);	
	}
	color=color&0xffff0000;
	
	pointcolor=winbuf[pointy*width+pointx];
	winbuf[pointy*width+pointx]=0xffffffff;
	rect(win,1256,0,1512,256,pointcolor,pointcolor);	
	printhexadecimal(win,1256,257,3,pointcolor,0xffffffff,0xff000000);

	return;

}

void test_create(struct appworld *app)
{

	app->id=1;
	app->init=init;
	app->uninit=uninit;
	app->start=start;
	app->update=update;
	//con.write=write;
}
