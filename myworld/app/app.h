#ifndef _APP_H
#define _APP_H

#include "../window/window.h"	
#include "../event/event.h"	
struct appworld{
	unsigned int id;
	void (*init)(struct window *);
	void (*uninit)(struct window *);
	void (*start)(struct window *);
	int  (*update)(struct window *,struct event *);
};

struct appworld *appworldinit(struct appworld *app);
void appworldstart(struct window *win,struct appworld *app);
void appworldchangeapp(struct window *win,struct appworld *app,int appid);
void appworlduninit(struct window *win,struct appworld *app);



#endif
