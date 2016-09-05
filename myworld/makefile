all:
	make -s -C console
	make -s -C event
	gcc main.c console/console.o console/consolefb.o event/event.o event/eventepoll.o
clean:
	rm -f *.out *.o
	make -s -C console clean
	make -s -C event clean
