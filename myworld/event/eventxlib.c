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
0xff,' ',0xff,0x50,	0x71,0x72,0x73,0x74,
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
static int motioncount=0;
static unsigned int value;

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

int xlibevent(struct event *eve)
{
	unsigned int type;
	XEvent ev;

	while(1)
	{
		XNextEvent(dsp, &ev);
		if(ev.type==Expose)
		{
			if (ev.xexpose.count == 0)
			{
				eve->type=0xff;
				eve->value=no_event;
				return;
			}
		}
		else if(ev.type==ClientMessage)
		{
			if (ev.xclient.data.l[0] == wmDelete)
			{
				eve->type=0xff;
				eve->value=exit_event;
				return;
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
			eve->type=0xff;
			eve->value=no_event;
			return;
		}
		else if(ev.type==MotionNotify)
		{
			//motioncount = (motioncount+1)%5;
			//if(motioncount != 0)continue;
			loge("%d,%d\n",ev.xbutton.x,ev.xbutton.y);
			value = (ev.xbutton.x<<16)+(ev.xbutton.y);
			//loge("value=%d\n",value);
			eve->type=mousemove;
			eve->value=value;
			return;
		}
		else if(ev.type==ButtonPress)
		{
			printf("buttonpress\n");
			if(ev.xbutton.button==Button4)	//'xyz fron'
			{
				printf("buttonpress 1\n");
				eve->type=middlefront;
				eve->value=ev.xbutton.y + (ev.xbutton.x<<16);
				return;
			}
			else if(ev.xbutton.button==Button5)	//'xyz down'
			{
				printf("buttonpress 2\n");
				eve->type=middleback;
				eve->value=ev.xbutton.y + (ev.xbutton.x<<16);
				return;
			}
			else if(ev.xbutton.button==Button1)//left button
			{
				printf("buttonpress l\n");
				eve->type=leftbuttonpress;
				eve->value=ev.xbutton.y + (ev.xbutton.x<<16);
				return;
			}
			else if(ev.xbutton.button==Button2)//middle button
			{
				printf("buttonpress m\n");
				eve->type=middlebuttonpress;
				eve->value=ev.xbutton.y + (ev.xbutton.x<<16);
				return;
			}
			else if(ev.xbutton.button==Button3)//right button
			{
				printf("buttonpress r\n");
				eve->type=rightbuttonpress;
				eve->value=ev.xbutton.y + (ev.xbutton.x<<16);
				return;
			}

			continue;
		}
		else if(ev.type==KeyPress)
		{
			char temp;
			//KeyCode keyQ = XKeysymToKeycode(dsp, XStringToKeysym("Q"));
			//if (ev.xkey.keycode == keyQ)break;
			//printf("%x\n",ev.xkey.keycode);

			//普通anscii码
			temp=xlib2anscii[ev.xkey.keycode];
			
			if(temp!=0)
			{
				eve->type=keyanscii;
				eve->value=temp;
				return 1;
			}

			//控制按键
			eve->type=keyconctl;
			eve->value=xlib2kbd[ev.xkey.keycode];
			loge("%x,%x\n",ev.xkey.keycode,eve->value);
			return 1;
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
