#include "../window/window.h"
#include "../event/event.h"
#include "../console/console.h"
void menu_init()
{

}

int call_menu(struct window *win)
{
	int width,height,bpp;
	unsigned int* winbuf;
	winbuf=(unsigned int*)(win->screenbuf);

	width=*(win->width);
	height=*(win->height);
	//title
	rect(win,
		width/4,
		(height/4)&0xfffffff0,
		width*3/4,
		(height/4+16)&0xfffffff0,
		0x01234567,
		0xfedcba98
	);

	//body
	rect(win,
		width/4,
		(height/4+16)&0xfffffff0,
		width*3/4,
		height*3/4,
		0,
		0xffffffff
	);

	//button
	rect(win,
		(width*3/4) - 16,
		(height/4)&0xfffffff0,
		width*3/4,
		((height/4) + 16)&0xfffffff0,
		0xff0000,
		0
	);

	//string
	printstring(win,
		width/4,
		height/4 + 16,
		1,
		"hello word",
		0xffffffff,
		0
	);

	return 1;
}

int menu_update(struct window *win,struct event *eve,int *appid)
{
	loge("menu_update\n");
	
	return 2;
}














