#include <stdio.h>
#include "app.h"
#include<fcntl.h>	
#include<unistd.h>


static void init()
{	

}

static void uninit()
{
}


void test_create(struct appworld *app)
{

	app->id=1;
	app->init=init;
	app->uninit=uninit;
	//con.write=write;
}
