	CC = gcc
all:
	$(CC) -Wall -g -ocpufre cpufre.c `pkg-config --cflags --libs gtk+-2.0 gthread-2.0` -export-dynamic
clean:
	rm -f count
