#ifndef _EVENT_H
#define _EVENT_H

#include "../window/window.h"

#define no_event 0xff
#define exit_event 0x1b

#define esc_key 0x1b
#define nofun_key 0xff
#define up_key 0x41
#define down_key 0x42
#define right_key 0x43
#define left_key 0x44
#define plus_key 0x2b
#define sub_key 0x2d

#define leftbuttonpress 0x01
#define rightbuttonpress 0x02
#define middlebuttonpress 0x03
#define middlefront 0x04
#define middleback 0x05
#define mousemove 0x06

#define leftbuttonrelease 0x11
#define rightbuttonrelease 0x12
#define middlebuttonrelease 0x13

#define keyboardpress 0x10

struct event{
	unsigned int id;
	void (*init)(struct window *);
	void (*uninit)();
	int (*getevent)(struct event *);
	int (*subevent)(struct event *);
	unsigned int type;
	unsigned int value;

};

struct event *eventinit(struct event *eve,struct window *);
void eventuninit(struct event *eve);

#endif
