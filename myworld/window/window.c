#include <stdio.h>
#include "window.h"
#include "../console/console.h"
#include <malloc.h>

#define WINDOWMEMBER 10
struct window *winregister[WINDOWMEMBER];


struct window *windowinit(struct window *win)
{
	int i;
	winregister[0]=(struct window *)malloc(sizeof(struct window));	
	//fb_create(conregister[0]);

	for(i=0;i<WINDOWMEMBER;i++)
	{
		
		if(winregister[i]->id==win->id)
		{
			win=winregister[i];
			break;
		}
	}
	
	win->init();
	return win;
}

void windowuninit(struct window *win)
{
	win->uninit();
}
