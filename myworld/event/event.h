#ifndef _EVENT_H
#define _EVENT_H

#define no_event 0xff
#define exit_event 0x1b

#define esc_key 0x1b
#define nofun_key 0xff
#define up_key 0x41
#define down_key 0x42
#define right_key 0x43
#define left_key 0x44

struct event{
	unsigned int id;
	void (*init)();
	void (*uninit)();
	int (*getevent)();
	int (*subevent)();

};

struct event *eventinit(struct event *eve);
void eventuninit(struct event *eve);

#endif
