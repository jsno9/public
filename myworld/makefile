obj=main.c
obj+=console/console.o
obj+=console/consolefb.o
obj+=event/event.o
obj+=event/eventepoll.o
obj+=event/eventxlib.o
obj+=window/window.o
obj+=window/windowfb.o
obj+=window/windowxlib.o
obj+=app/app.o
obj+=app/apptest.o
obj+=app/app_ox.o
obj+=draw/draw.o
obj+=draw/draw_menu.o
all:
	make -s -C console
	make -s -C event
	make -s -C window
	make -s -C draw
	make -s -C app
	gcc -Wall -lX11 $(obj)
clean:
	rm -f *.out *.o
	make -s -C console clean
	make -s -C event clean
	make -s -C window clean
	make -s -C draw clean
	make -s -C app clean
