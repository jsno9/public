#include "include/jsno9.h"
#include "console/console.h"
#include "event/event.h"
#include "window/window.h"
#include "app/app.h"
#include <stdio.h>
#include <malloc.h>
#include "draw/draw.h"

static unsigned int type;

void main()
{
	loge("hello world\n");

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
	window->id=2;
	window=windowinit(window);
	loge("hello world 1\n");

	struct appworld *app=NULL;
	app=(struct appworld *)malloc(sizeof(struct appworld));
	app->id=1;
	app=appworldinit(app);
loge("hello world 2\n");
	appworldstart(window,app);
loge("hello world 3\n");
	while(1)
	{
		type=event->getevent(event);
		loge("type=%x\n",type);
		if(type==exit_event)
		{
			break;
		}
		app->update(window,type);
		window->write(window);
		

	}


	consoleuninit(console);
	eventuninit(event);
	windowuninit(window);
	windowuninit(app);
	
	free(console);
	free(event);
	free(window);
	free(app);
}



