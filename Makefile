# Makefile for JamesM's kernel tutorials.

SUB_PROJECTS="libc"
INCLUDES="src/include/ libc/include/"
CSOURCES=$(shell find ./src/ -name *.c -not -name "\.*")
COBJECTS=$(patsubst %.c, %.o, $(CSOURCES))
SSOURCES=$(shell find ./src/ -name *.s -not -name "\.*")
SOBJECTS=$(patsubst %.s, %.o, $(SSOURCES))

ALLFILES=$(shell find . \( ! -regex '.*/\..*' \) -type f)

CC=gcc
LD=ld
CFLAGS=-Wall -g -nostdlib -nostdinc -fno-builtin -m32 -Isrc/include/ -Ilibc/include/ -pipe
LDFLAGS=-melf_i386 -Tlink.ld
ASFLAGS=-felf

all: clean subprojects $(COBJECTS) $(SOBJECTS) link update

update: build_initrd
	@echo Updating floppy image
	@-mkdir mnt/
	@sudo mount -o loop floppy.img mnt/
	@sudo cp kernel mnt/
	@sudo cp initrd.img mnt/
	@sleep 1
	@sudo umount mnt/
	@sleep 1
	@-rm -rf mnt/

initrd_contents = $(shell find ./initrd_contents/ -type f)
map = $(foreach file,$(initrd_contents),./initrd_contents/$(1) $(1))
build_initrd:
	@./tools/make_initrd $(map) > /dev/null
	
clean:
	@echo Removing object files
	@for file in $(COBJECTS) $(SOBJECTS) kernel kernel.sym initrd.img emilos.tgz; do if [ -f $$file ]; then rm $$file; fi; done
	@for proj in $(SUB_PROJECTS); do if [ -d "$$proj/" ]; then @$(MAKE) -C $$proj/ clean; fi; done

link:
	@echo " LD	*.o"
	@$(LD) $(LDFLAGS) -o kernel $(SOBJECTS) $(COBJECTS) libc/libc.a
	@objcopy --only-keep-debug kernel kernel.sym
	@objcopy --strip-debug kernel

.s.o:
	@echo " NASM	$<"
	@nasm $(ASFLAGS) $<

.c.o:
	@echo " CC	$<"
	@$(CC) $(CFLAGS) -o $@ -c $<

subprojects:
	@echo Building sub projects...
	@for proj in $(SUB_PROJECTS); do if [ -d "$$proj/" ]; then @$(MAKE) -C $$proj/; fi; done

run: clean all
	@echo Starting QEMU
	@qemu -fda floppy.img&

debug: clean all
	@echo Starting QEMU
	@qemu -s -S -fda floppy.img&
	@echo Starting GDB
	@echo To connect type: target remote localhost:1234
	@gdb --symbols=kernel.sym

srcdist:
	@tar czf emilos.tgz $(ALLFILES)

todos:
	-@for file in $(ALLFILES); do grep -H TODO $$file; done; true

fixmes:
	-@for file in $(ALLFILES); do grep -H FIXME $$file; done; true

help:
	@echo "Available make targets:"
	@echo
	@echo "all		- build kernel"
	@echo "run		- run the kernel in qemu"
	@echo "debug		- like run, but with debugging enabled and gdb started"
	@echo "clean		- remove all object files"
	@echo "update		- update floppy image"
	@echo "build_initrd	- build initrd image"
	@echo "subprojects	- build all related subprojects"
	@echo "srcdist		- build emilos.tgz (source tarball)"
	@echo "todos		- list all TODO comments in the sources"
	@echo "fixmes		- list all FIXME comments in the sources"
	@echo "help		- print this list"

.PHONY: all kernel run debug update build_initrd subprojects clean srcdist todos fixmes help
