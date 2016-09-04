#include <stdio.h>
#include "console.h"
#include<fcntl.h>	
#include<unistd.h>
#include<termios.h>		//	termios,getchar

static struct termios bak;
static struct termios local;
static int signal=-1;

static void init()
{	
	signal=tcgetattr(STDIN_FILENO,&bak);
	local=bak;
	local.c_lflag&=~(ICANON|ECHO);
	tcsetattr(STDIN_FILENO,TCSANOW,&local);
	fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);

}

static void uninit()
{
	bak.c_lflag|=(ICANON|ECHO);
	if(signal!=-1)tcsetattr(STDIN_FILENO,TCSANOW,&bak);
}


void fb_create(struct console *con)
{

	con->id=1;
	con->init=init;
	con->uninit=uninit;
	//con.write=write;
}






