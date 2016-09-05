#ifndef _WINDOW_H
#define _WINDOW_H
	
struct window{
	unsigned int id;
	void (*init)();
	void (*uninit)();
	char* screenbuf;
	int width;
	int height;

};


struct window *windowinit(struct window *win);
void windowuninit(struct window *win);



#endif
