#include <stdarg.h>		//loge
#include <stdio.h>
#include "app.h"
#include<fcntl.h>	
#include<unistd.h>
#include <malloc.h>

#define APPWORLDMEMBER 10
static struct appworld *appregister[APPWORLDMEMBER];

struct appworld *appworldinit(struct appworld *app)
{	
	
	int i;
	appregister[0]=(struct appworld *)malloc(sizeof(struct appworld));	
	fb_create(appregister[0]);

	for(i=0;i<APPWORLDMEMBER;i++)
	{
		
		if(appregister[i]->id==app->id)
		{
			app=appregister[i];
			break;
		}
	}
	
	app->init();
	return app;
	
}

void appworlduninit(struct appworld *app)
{
	app->uninit();
}
