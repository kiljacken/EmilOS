# Makefile for JamesM's kernel tutorials.


CC=gcc
CFLAGS=-O3 -pipe
LDFLAGS=-Tlink.ld
ASFLAGS=-felf -iincludes/

all: clean
	@$(CC) $(CFLAGS) make_initrd.c -o make_initrd
	@chmod +x make_initrd

clean:
	@-rm make_initrd
