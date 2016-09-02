#include "include/jsno9.h"
#include "console/console.h"
#include <stdio.h>
void main()
{
	loge("hello world\n");
	struct console console;
	consoleinit(&console);
	consoleuninit(&console);
}



