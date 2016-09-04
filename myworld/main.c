#include "include/jsno9.h"
#include "console/console.h"
#include "event/event.h"
#include <stdio.h>
#include <malloc.h>

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


	while(1)
	{
		type=event->getevent(event);
		if(type==exit_event)
		{
			break;
		}
	}


	consoleuninit(console);
	eventuninit(event);
	free(console);
}



