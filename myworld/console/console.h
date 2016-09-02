#ifndef _CONSOLE_H
#define _CONSOLE_H
#include <termios.h>
	
struct console{
	unsigned int id;
	struct termios bak;
	struct termios local;
	void (*init)(struct console);
	void (*uninit)(struct console);
	void (*write)(const char *format, ...);
};

void loge(const char *format, ...);
void consoleinit(struct console *con);
void consoleuninit(struct console *con);


#endif
