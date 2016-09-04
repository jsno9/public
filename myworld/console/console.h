#ifndef _CONSOLE_H
#define _CONSOLE_H
	
struct console{
	unsigned int id;
	void (*init)();
	void (*uninit)();

};

void loge(const char *format, ...);
struct console *consoleinit(struct console *con);
void consoleuninit(struct console *con);



#endif
