#include<unistd.h>
#include<fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "window.h"
#include <X11/Xlib.h>


static Display* dsp;
static Visual *visual;
static XImage* ximage;
static Window xlibwin;
static GC gc;
Atom wmDelete;

//static char* userpixel=0;
static int width=1920;
static int height=1080;
static int bpp=32;
__attribute__((aligned(0x1000))) static unsigned char userpixel[2048*1024*32];

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

	
	win->dsp=dsp;

	win->visual=visual;
	win->ximage=ximage;
	
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
