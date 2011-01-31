# Makefile for JamesM's kernel tutorials.

CSOURCES=$(shell find -name *.c)
COBJECTS=$(patsubst %.c, %.o, $(CSOURCES))
SSOURCES=$(shell find -name *.s)
SOBJECTS=$(patsubst %.s, %.o, $(SSOURCES))

CC=gcc
LD=ld
CFLAGS=-nostdlib -fno-builtin -m32 -Isrc/include/ -pipe
LDFLAGS=-melf_i386 -Tlink.ld
ASFLAGS=-felf

all: clean $(COBJECTS) $(SOBJECTS) link update

update:
	@echo Updating floppy image
	@mkdir mnt/
	@sudo mount -o loop floppy.img mnt/
	@sudo cp kernel mnt/kernel
	@sudo umount mnt/
	@sleep 1
	@rm -rf mnt/

clean:
	@echo Removing object files
	@-rm $(COBJECTS) $(SOBJECTS) kernel

link:
	@echo Linking
	@$(LD) $(LDFLAGS) -o kernel $(SOBJECTS) $(COBJECTS)

.s.o:
	@echo Assembling $<
	@nasm $(ASFLAGS) $<

.c.o:
	@echo Compiling $<
	@$(CC) $(CFLAGS) -o $@ -c $<

run: clean all
	qemu -fda floppy.img
