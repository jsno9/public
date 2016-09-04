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

struct event *eventinit(struct event *eve)
{	
	
	int i;
	everegister[0]=(struct event *)malloc(sizeof(struct event));	
	epollevent_create(everegister[0]);

	for(i=0;i<EVENTMEMBER;i++)
	{
		
		if(everegister[i]->id==eve->id)
		{
			eve=everegister[i];
			break;
		}
	}



	eve->getevent=getevent;
	eve->init();
	return eve;
	
}

void eventuninit(struct event *eve)
{
	eve->uninit();
}




