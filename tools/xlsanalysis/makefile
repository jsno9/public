CC = gcc

ARCH = $(shell getconf LONG_BIT)
ifeq ($(ARCH), 32)
  LIBPATH = lib
else
  LIBPATH = lib64
endif

CFLAGS = -I include_c -L $(LIBPATH) -lxl -Wl,-rpath,$(LIBPATH)

all:thermal

thermal: thermal.c
	$(CC) -o thermal thermal.c $(CFLAGS)
