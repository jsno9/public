#include <stdio.h>
#include "window.h"
#include "../console/console.h"
#include <malloc.h>

#define WINDOWMEMBER 10
static struct window *winregister[WINDOWMEMBER];


struct window *windowinit(struct window *win)
{

	int i;

loge("window init \n");

	winregister[0]=(struct window *)malloc(sizeof(struct window));	
	winfb_create(winregister[0]);

	winregister[1]=(struct window *)malloc(sizeof(struct window));	
	winxlib_create(winregister[1]);

	for(i=0;i<WINDOWMEMBER;i++)
	{
		
		if(winregister[i]->id==win->id)
		{
			win=winregister[i];
			break;
		}
	}

	loge("window init end\n");
	win->init(win);
	return win;
}

void windowuninit(struct window *win)
{
	win->uninit();
}
