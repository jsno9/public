#include "include/jsno9.h"
#include "console/console.h"
#include "event/event.h"
#include "window/window.h"
#include <stdio.h>
#include <malloc.h>
#include "draw/draw.h"

static unsigned int type;

void main()
{
	loge("hello world\n");
///////////

unsigned int width;
unsigned int height;
unsigned int bpp;
int i;
////////////////
	struct console *console=NULL;
	console=(struct console *)malloc(sizeof(struct console));
	console->id=1;
	console=consoleinit(console);

	struct event *event=NULL;
	event=(struct event *)malloc(sizeof(struct event));
	event->id=1;
	event=eventinit(event);

	struct window *window=NULL;
	window=(struct window *)malloc(sizeof(struct window));
	window->id=1;
	window=windowinit(window);
//////////////////////////
width=*(window->width);

height=*(window->height);
bpp=*(window->bpp);
loge("width=%d\n",width);
	for(i=0;i<width*height*bpp/8;i=i+4)
	{
		//loge("for\n");
		window->screenbuf[i]=0x0;
		window->screenbuf[i+1]=0;
		window->screenbuf[i+2]=0;
		window->screenbuf[i+3]=0xff;
	}
window->write(window);
//line(window,10,10,200,200,0xffffff);
loge("hello world3\n");
//backgroundcolor(window,0x567853);
//////////////////////////

	while(1)
	{
		type=event->getevent(event);
		if(type==exit_event)
		{
			break;
		}
		window->write(window);
		printascii(window,500,500,10,'a',0xffffff,0);

	}


	consoleuninit(console);
	eventuninit(event);
	free(console);
}



