CSOURCES=$(shell find . -name "*.c" -not -name "\.*")
COBJECTS=$(patsubst %.c, %.o, $(CSOURCES))

CC=gcc
CFLAGS=-Wall -g -nostdlib -nostdinc -fno-builtin -m32 -I../src/include/ -Iinclude/ -pipe
ASFLAGS=-felf

all: $(COBJECTS) link

link:
	@echo " AR	*.o"
	@ar cr libc.a $(COBJECTS)

.c.o:
	@echo " CC	$<"
	@$(CC) $(CFLAGS) -o $@ -c $<
