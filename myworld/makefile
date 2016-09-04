all:
	make -C console
	make -C event
	gcc main.c console/console.o console/consolefb.o event/event.o event/eventepoll.o
