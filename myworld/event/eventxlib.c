#include <sys/epoll.h>//epoll
#include "event.h"
#include "../console/console.h"
#include "../window/window.h"
#include<unistd.h>
#include <fcntl.h>
#include <X11/Xlib.h>

static char xlib2anscii[0x80]={
0,0,0,0,		0,0,0,0,
0,0,'1','2',		'3','4','5','6',	//0
'7','8','9','0',	'-','=',0x8,0x9,
'q','w','e','r',	't','y','u','i',	//0x10
'o','p','[',']',	0xd,0,'a','s',
'd','f','g','h',	'j','k','l',';',	//0x20
'\'','`',0,'\\',	'z','x','c','v',
'b','n','m',',',	'.','/',0,'*',		//0x30
0,' ',0,0,		0,0,0,0,
0,0,0,0,		0,0,0,'7',		//0x40
'8','9','-','4',	'5','6','+','1',
'2','3','0','.',	0,0,0,0,		//0x50
0,0,0,0,		0,0,0,0,
0,0,0,0,		0,0,0,0,		//0x60,0x6f
0,0,0,0,		0,0,0,0,
0,0,0,0,		0,0,0,0,		//0x70,0x7f
};
static char xlib2kbd[0x80]={
0,0,0,0,		0,0,0,0,
0,0x1b,'1','2',		'3','4','5','6',	//0
'7','8','9','0',	'-','=',0x8,0x9,
'q','w','e','r',	't','y','u','i',	//0x10
'o','p','[',']',	0xd,0xff,'a','s',
'd','f','g','h',	'j','k','l',';',	//0x20
'\'','`',0xff,'\\',	'z','x','c','v',
'b','n','m',',',	'.','/',0xff,'*',	//0x30
0xff,' ',0xff,0x70,	0x71,0x72,0x73,0x74,
0x75,0x76,0x77,0x78,	0x79,0xff,0xff,'7',	//0x40
'8','9','-','4',	'5','6','+','1',
'2','3','0','.',	0xff,0xff,0xff,0xff,	//0x50
0xff,0xff,0xff,0xff,	0,0,0,0,
0,0,0,0,		0,0,0,0x26,		//0x60,0x6f
0,0x25,0x27,0,		0x28,0,0,0,
0,0,0,0,		0,0,0,0,		//0x70,0x7f
};

static Display* dsp;
static Visual *visual;
static XImage* ximage;
extern Atom wmDelete;

static int width;
static int height;
static unsigned char* userpixel;

static void init(struct window *win)
{
loge("xlibevent init\n");
	dsp=win->dsp;
loge("dsp1=%x\n",dsp);
	visual=win->visual;
	ximage=win->ximage;
	width=*(win->width);
	height=*(win->height);
	userpixel=win->screenbuf;
}

static void uninit()
{

}

int xlibevent()
{
	XEvent ev;

	while(1)
	{
		XNextEvent(dsp, &ev);
		if(ev.type==Expose)
		{
			if (ev.xexpose.count == 0)
			{
				return no_event;
			}
		}
		else if(ev.type==ClientMessage)
		{
			if (ev.xclient.data.l[0] == wmDelete)
			{
				
				return exit_event;
			}
		}
		else if(ev.type==ConfigureNotify)
		{
			int x = ev.xconfigure.width;
			int y = ev.xconfigure.height;
			//printf("%d,%d\n",x,y);

			if( (x==width) && (y==height) )continue;
			width=x;
			height=y;

			ximage=XCreateImage(
				dsp,visual,24,ZPixmap,0,
				userpixel,x,y,32,0
			);

			return no_event;
		}
		
	}
}

void xlibevent_create(struct event *eve)
{

	eve->id=2;

	eve->init=init;	

	eve->uninit=uninit;
	
	
	eve->subevent=xlibevent;
}
