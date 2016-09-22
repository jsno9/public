
#include "../console/console.h"
#include "../event/event.h"
#include "app.h"
#include <stdio.h>
//#include<fcntl.h>	
//#include<unistd.h>
static char data[9];
static int turn=0;
static int winner=0;
static int scorex=0;
static int scoreo=0;

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
	int min = (width<height) ? width:height;

	scorex=0;
	scoreo=0;
	
	for(j=0;j<9;j++)data[j]=0;
loge("app_ox start\n");
	backgroundcolor(win,0);
        line(win,min/16,    min  /  3,      min *15 /16,    min   /   3,    0xffffffff);
        line(win,min/16,    min *2 /3,      min *15 /16,    min  *2  /3,    0xffffffff);
        line(win,min/3,     min  / 16,      min   /   3,    min *15 /16,    0xffffffff);
        line(win,min*2/3,   min  / 16,      min  *2  /3,    min *16 /16,    0xffffffff);

	printdecimal(win,1280,512,5,scorex,0xffffffff,0);
	printdecimal(win,1280,612,5,scoreo,0xffffffff,0);
	
	printstring(win,1180,512,5,"x:",0xffffffff,0);
	printstring(win,1180,612,5,"o:",0xffffffff,0);
}

static void pointrange(int *x,int *y,int xmin,int xmax,int ymin,int ymax)
{
	
	if(*x<=xmin)
	{
		*x=xmin;
	}
	if(*x>=xmax)
	{
		*x=xmax;
	}
	if(*y>=ymax)
	{
		*y=ymax;
	}
	if(*y<=ymin)
	{
		*y=ymin;
	}
}

static int winnerupdate(struct window *win, int winner, int min, int scorex, int scoreo)
{
	int i;
	backgroundcolor(win,0);
    line(win,min/16,    min  /  3,      min *15 /16,    min   /   3,    0xffffffff);
    line(win,min/16,    min *2 /3,      min *15 /16,    min  *2  /3,    0xffffffff);
    line(win,min/3,     min  / 16,      min   /   3,    min *15 /16,    0xffffffff);
    line(win,min*2/3,   min  / 16,      min  *2  /3,    min *16 /16,    0xffffffff);
	for(i=0;i<9;i++)
		data[i]=0;

	printdecimal(win,1280,512,5,scorex,0xffffffff,0);
	printdecimal(win,1280,612,5,scoreo,0xffffffff,0);
	printstring(win,1180,512,5,"x:",0xffffffff,0);
	printstring(win,1180,612,5,"o:",0xffffffff,0);
}

static int update(struct window *win,struct event *eve)
{
	unsigned int x,y,i,j,k,pointcolor;
	static unsigned int color=0xff010000;
	static  int pointx=512;
	static  int pointy=512;
	
	int width,height,bpp;
	unsigned int* winbuf;	
	unsigned int temp=0x00010000;
	winbuf=(unsigned int*)(win->screenbuf);
	width=*(win->width);
	height=*(win->height);
	bpp=*(win->bpp);
	int min = (width<height) ? width:height;
	if(eve->id==1)
	{
		if(eve->type==0x02)
		{
			
		}//if eve->type==0x02
		else if(eve->type==0x01)
		{
			
		
		}
	}//eventid==1
	else if(eve->id==2)
	{
		loge("id2event=%d\n",eve->type);
		if(eve->type==mousemove)
		{
			
			
			
		}//eve->type==mousemove
		else if(eve->type==leftbuttonpress)
		{
			pointx=(eve->value>>16)&0xffff;
			pointy=eve->value&0xffff;
			if(pointx>min) return;
			if(pointy>min) return;		

			pointx = pointx*3/min;
			pointy = pointy*3/min;

			if(data[pointy*3+pointx]!=0) return;
			
			if((turn&0x01)==0x01)temp='o';
			else temp='x';

			data[pointy*3+pointx]=temp;
			turn++;



		}//eve->type=leftbuttonpress
		else if(eve->type==leftbuttonrelease)
		{
			
		}//eve->type=leftbuttonrelease
		else if(eve->type==middlefront)
		{
			
			
		}//eve->type=middlefront
		else if(eve->type==middleback)
		{
			
		}//eve->type=middleback
		//loge("haha=%d,%d\n",pointx,pointy);
	}

	for(y=0;y<3;y++)
	{
		for(x=0;x<3;x++)
		{
			if(data[3*y + x] == 'o')
			{
				circleframe(win,
					(2*x+1)*min/6,
					(2*y+1)*min/6,
					min/12,
					0xff
				);
			}
			else if(data[3*y + x] == 'x')
			{
				line(win,
					(4*x+1)*min/12,
					(4*y+1)*min/12,
					(4*x+3)*min/12,
					(4*y+3)*min/12,
					0xff0000
				);
				line(win,
					(4*x+3)*min/12,
					(4*y+1)*min/12,
					(4*x+1)*min/12,
					(4*y+3)*min/12,
					0xff0000
				);
			}
		}//forx
	}//fory

	for(i=0,j=0;i<9;i++)
	{
		if((data[i]=='x')||(data[i]=='o'))
		{	
			j++;
		}	
	}
	
	if(j==9)
	{
		winner=3;
	}

	for(y=0;y<3;y++)//横
	{
		if((data[3*y+0]=='o')&&(data[3*y+1]=='o')&&(data[3*y+2]=='o'))
		{
			winner=2;
			break;
		}
		else if((data[3*y+0]=='x')&&(data[3*y+1]=='x')&&(data[3*y+2]=='x'))
		{
			winner=1;
			break;
		}
	}



	for(x=0;x<3;x++)//竖
	{
		if((data[3*0+x]=='o')&&(data[3*1+x]=='o')&&(data[3*2+x]=='o'))
		{
			winner=2;
			break;
		}
		else if((data[3*0+x]=='x')&&(data[3*1+x]=='x')&&(data[3*2+x]=='x'))
		{
			winner=1;
			break;
		}
	}

	for(x=1,y=1;x<2;x++)//右斜
	{
		if((data[3*y+x]=='o')&&(data[3*(y+1)+x-1]=='o')&&(data[3*(y-1)+x+1]=='o'))
		{
			winner=2;
			break;
		}
		else if((data[3*y+x]=='x')&&(data[3*(y+1)+x-1]=='x')&&(data[3*(y-1)+x+1]=='x'))
		{
			winner=1;
			break;
		}
	}

	for(x=1,y=1;x<2;x++)//左斜
	{
		if((data[3*y+x]=='o')&&(data[3*(y+1)+x+1]=='o')&&(data[3*(y-1)+x-1]=='o'))
		{
			winner=2;
			break;
		}
		else if((data[3*y+x]=='x')&&(data[3*(y+1)+x+1]=='x')&&(data[3*(y-1)+x-1]=='x'))
		{
			winner=1;
			break;
		}
	}

	
	
	if(winner==2)
	{
		scoreo++;
		winnerupdate(win,winner,min,scorex,scoreo);
		winner=0;
		return;
	}
	else if(winner==1)
	{
		scorex++;
		winnerupdate(win,winner,min,scorex,scoreo);
		winner=0;
		return;
	}
	else if(winner==3)
	{
		winnerupdate(win,winner,min,scorex,scoreo);
		winner=0;
		return;
	}
	
	return;

}

void ox_create(struct appworld *app)
{

	app->id=2;
	app->init=init;
	app->uninit=uninit;
	app->start=start;
	app->update=update;
	//con.write=write;
}


