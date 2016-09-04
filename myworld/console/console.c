#include <stdarg.h>		//loge
#include <stdio.h>
#include "console.h"
#include<fcntl.h>	
#include<unistd.h>
#include <malloc.h>

#define CONSOLEMEMBER 10
//void fb_create(struct console *con);
static struct console *conregister[CONSOLEMEMBER];
struct console *consoleinit(struct console *con)
{	
	
	int i;
	conregister[0]=(struct console *)malloc(sizeof(struct console));	
	fb_create(conregister[0]);

	for(i=0;i<CONSOLEMEMBER;i++)
	{
		
		if(conregister[i]->id==con->id)
		{
			con=conregister[i];
			break;
		}
	}
	
	con->init();
	return con;
	
}

void consoleuninit(struct console *con)
{
	con->uninit();
}

void loge(const char *format, ...)
{
    va_list arg;

    va_start (arg, format);
	vfprintf (stdout, format, arg);
    va_end (arg);
	
}
