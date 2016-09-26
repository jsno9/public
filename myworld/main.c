#include "include/jsno9.h"
#include "console/console.h"
#include "event/event.h"
#include "window/window.h"
#include "app/app.h"
#include <stdio.h>
#include <malloc.h>
#include "draw/draw.h"

//static unsigned int value;

void main()
{
	loge("hello world\n");
	int menuflag=0;
	int menucount=0;	
	int *appid;
	struct console *console=NULL;
	console=(struct console *)malloc(sizeof(struct console));
	console->id=1;
	console=consoleinit(console);

	struct window *window=NULL;
	window=(struct window *)malloc(sizeof(struct window));
	window->id=2;
	window=windowinit(window);
	loge("hello world 1\n");

	struct event *event=NULL;
	event=(struct event *)malloc(sizeof(struct event));
	event->id=2;
	event=eventinit(event,window);

	

	struct appworld *app=NULL;
	app=(struct appworld *)malloc(sizeof(struct appworld));
	app->id=2;
	app=appworldinit(app);
	appworldstart(window,app);
	loge("hello app 1\n");
	while(1)
	{
		event->getevent(event);
		//loge("type=%x\n",type);
		if(event->value==exit_event)
		{
			break;
		}
		
		if(event->value==f1_event)
		{
			if(menucount==0)
			{
				menuflag=1;
				menucount=1;
			}
			else if(menucount==1)
			{
				menuflag=4;
				menucount=0;
			}
		}

		if(menuflag==1)
		{
			call_menu(window);
			menuflag=2;
			loge("menu1=%d\n",menuflag);
		}
		else if(menuflag==2)
		{
			menuflag=menu_update(window,event,appid);
			loge("menu=%d\n",menuflag);
		}
		else if(menuflag==3)
		{
			appworldchangeapp(window,app,&appid);
			call_menu(window);
		}
		else if(menuflag==4)
		{
			app->start(window);
			app->update(window,event);
			menuflag=0;
		}
		else
		{
			app->update(window,event);
		}
		window->write(window);
		

	}


	consoleuninit(console);
	eventuninit(event);
	windowuninit(window);
	//windowuninit(app);
	
	free(console);
	free(event);
	free(window);
	free(app);
}



