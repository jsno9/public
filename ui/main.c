#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

static char* rawspace=0;

void log(const char *format, ...)
{
    va_list arg;

    va_start (arg, format);
	vfprintf (stdout, format, arg);
    va_end (arg);

}

void init()
{
	int size=4*0x400000;

	rawspace=malloc(size+0x1000);	
}

void main()
{
	log("hello world\n");

	init();
 
}
