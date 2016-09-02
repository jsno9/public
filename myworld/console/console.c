#include <stdarg.h>		//loge
#include <stdio.h>
#include "console.h"
#include<fcntl.h>	
#include<unistd.h>
static int signal=-1;

void consoleinit(struct console *con)
{	
	struct console conregister[10];
	
	fb_create(&conregister[1]);
	
	
	
}

void consoleuninit(struct console *con)
{
	
}

void loge(const char *format, ...)
{
    

}
