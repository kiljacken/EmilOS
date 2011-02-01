# Makefile for JamesM's kernel tutorials.

CSOURCES=$(shell find -name *.c -not -wholename "*tools/*")
COBJECTS=$(patsubst %.c, %.o, $(CSOURCES))
SSOURCES=$(shell find -name *.s -not -wholename "*tools/*")
SOBJECTS=$(patsubst %.s, %.o, $(SSOURCES))

CC=gcc
LD=ld
CFLAGS=-nostdlib -fno-builtin -m32 -Isrc/include/ -pipe
LDFLAGS=-melf_i386 -Tlink.ld
ASFLAGS=-felf

all: clean $(COBJECTS) $(SOBJECTS) link update

update: build_initrd
	@echo Updating floppy image
	@-mkdir mnt/
	@sudo mount -o loop floppy.img mnt/
	@sudo cp kernel mnt/kernel
	@sudo cp initrd.img mnt/initrd
	@sleep 1
	@sudo umount mnt/
	@sleep 1
	@-rm -rf mnt/

silly_syntax=./initrd_contents/$(1) $(1)
initrd_contents = $(shell find ./initrd_contents/ -type f)
map = $(foreach file,$(initrd_contents),$(call silly_syntax,$(file)))
build_initrd: tools
	@./tools/make_initrd $(map) > /dev/null
	

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

tools:
	@echo Building tools
	@make -C tools/ > /dev/null

run: clean all
	@echo Starting qemu
	@qemu -m 256 -fda floppy.img&

.PHONY : clean run link build_initrd update tools
