#include <stdio.h>
#include "event.h"
#include "../console/console.h"
#include <malloc.h>


#define EVENTMEMBER 10

static struct event *everegister[EVENTMEMBER];

int getevent(struct event *eve)
{
	int type;
	type=eve->subevent();
	if(type==0xff)
	{
		return no_event;	
	}
	else
	{
		return type;
	}


}

struct event *eventinit(struct event *eve,struct window *win)
{	
	
	int i;
loge("eventinit 0\n");
	everegister[0]=(struct event *)malloc(sizeof(struct event));	
	epollevent_create(everegister[0]);
	loge("eventinit 0.1\n");
	everegister[1]=(struct event *)malloc(sizeof(struct event));	
	xlibevent_create(everegister[1]);	

	for(i=0;i<EVENTMEMBER;i++)
	{
		
		if(everegister[i]->id==eve->id)
		{
			eve=everegister[i];
			break;
		}
	}

loge("eventinit 1\n");

	eve->getevent=getevent;
loge("eventinit 2\n");
	eve->init(win);
loge("eventinit 3\n");
	return eve;
	
}

void eventuninit(struct event *eve)
{
	eve->uninit();
}




