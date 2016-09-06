#ifndef _WINDOW_H
#define _WINDOW_H
	
struct window{
	unsigned int id;
	void (*init)(struct window *);
	void (*uninit)();
	void (*write)(struct window *);
	char* screenbuf;
	int* width;
	int* height;
	int* bpp;

};


struct window *windowinit(struct window *win);
void windowuninit(struct window *win);



#endif
