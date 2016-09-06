obj=main.c
obj+=console/console.o
obj+=console/consolefb.o
obj+=event/event.o
obj+=event/eventepoll.o
obj+=window/window.o
obj+=window/windowfb.o
obj+=draw/draw.o

all:
	make -s -C console
	make -s -C event
	make -s -C window
	make -s -C draw
	gcc -Wall $(obj)
clean:
	rm -f *.out *.o
	make -s -C console clean
	make -s -C event clean
	make -s -C window clean
	make -s -C draw clean
