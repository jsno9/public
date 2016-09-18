#include<unistd.h>
#include<fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "window.h"
#include <X11/Xlib.h>


Display* dsp;
Visual *visual;
XImage* ximage;
Window xlibwin;
GC gc;
Atom wmDelete;

//static char* userpixel=0;
static int width=512;
static int height=512;
static int bpp=32;
__attribute__((aligned(0x1000))) static unsigned char userpixel[2048*1024*32];

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

static void init(struct window *win)
{
	loge("winxlib_init\n");
	dsp = XOpenDisplay(NULL);

	visual = DefaultVisual(dsp,0);
	if(visual->class!=TrueColor)
	{
		loge("Cannot handle non true color visual ...\n");
		XCloseDisplay(dsp);
	}

	//pixel,ximage,window,gc
	xlibwin = XCreateSimpleWindow(dsp,RootWindow(dsp,0),0,0,width,height,1,0,0);
	gc = XCreateGC(dsp,xlibwin,0,NULL);

	// intercept window delete event 
	wmDelete=XInternAtom(dsp, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(dsp, xlibwin, &wmDelete, 1);

	XSelectInput
	(
		dsp, xlibwin,
		KeyPressMask|KeyReleaseMask|
		ButtonPressMask|ButtonReleaseMask|
		ButtonMotionMask|
		ExposureMask|
		StructureNotifyMask
	);
	XMapWindow(dsp, xlibwin);

	width=*(win->width);
	height=*(win->height);
	XResizeWindow(dsp, xlibwin, width, height);
	ximage=XCreateImage(
		dsp, visual, 24, ZPixmap, 0,
		userpixel, width, height, 32, 0);

	
	
}

static void uninit()
{
	XDestroyWindow(dsp, xlibwin);
	XCloseDisplay(dsp);
}

static void windowwrite(struct window *win)
{
	//loge("xlib write 1\n");
	XPutImage(dsp, xlibwin, gc, ximage, 0, 0, 0, 0, width, height); 
	//loge("xlib write 2\n");
}

void winxlib_create(struct window *win)
{
	loge("winxlib_create\n");
	win->id=2;
	win->init=init;
	win->uninit=uninit;
	win->write=windowwrite;
	win->screenbuf=userpixel;
	win->width=&width;
	win->height=&height;
	win->bpp=&bpp;
}
