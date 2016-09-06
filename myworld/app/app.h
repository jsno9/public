#ifndef _APP_H
#define _APP_H
	
struct appworld{
	unsigned int id;
	void (*init)();
	void (*uninit)();

};

void loge(const char *format, ...);
struct appworld *appworldinit(struct appworld *app);
void appworlduninit(struct appworld *app);



#endif
