#include <stdarg.h>		//loge
#include <stdio.h>
#include "app.h"
#include<fcntl.h>	
#include<unistd.h>
#include <malloc.h>
#include "../window/window.h"
#include "../console/console.h"
#define APPWORLDMEMBER 10
static struct appworld *appregister[APPWORLDMEMBER];

struct appworld *appworldinit(struct appworld *app)
{	
	
	int i;
	appregister[0]=(struct appworld *)malloc(sizeof(struct appworld));	
	test_create(appregister[0]);
	
	appregister[1]=(struct appworld *)malloc(sizeof(struct appworld));
	ox_create(appregister[1]);
	for(i=0;i<APPWORLDMEMBER;i++)
	{
		
		if(appregister[i]->id==app->id)
		{
			loge("appworldinit\n");
			app=appregister[i];
			break;
		}
	}
	
	//app->init();	
	loge("appworldinit %llx\n",app);
	return app;
	
}

void appworldstart(struct window *win,struct appworld *app)
{
	
	app->init(win);

	app->start(win);
	
}

void appworldchangeapp(struct window *win,struct appworld *app,int appid)
{

	int i;
	for(i=0;i<APPWORLDMEMBER;i++)
	{		
		if(appregister[i]->id==appid)
		{
			app=appregister[i];
			break;
		}
	}
	
	app->init(win);
	app->start(win);

}

void appworlduninit(struct window *win,struct appworld *app)
{
	int i;
	for(i=0;i<APPWORLDMEMBER;i++)
	{		
		if(appregister[i]->id!=0)
		{
			app=appregister[i];
			app->uninit(win);
		}	
	}
}
