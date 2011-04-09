# Makefile for JamesM's kernel tutorials.

CHEADERS=$(shell find -name *.h -not -wholename "*tools/*" -not -name "\.*")
CSOURCES=$(shell find -name *.c -not -wholename "*tools/*" -not -name "\.*")
COBJECTS=$(patsubst %.c, %.o, $(CSOURCES))
SSOURCES=$(shell find -name *.s -not -wholename "*tools/*" -not -name "\.*")
SOBJECTS=$(patsubst %.s, %.o, $(SSOURCES))

ALLFILES=$(shell find . \( ! -regex '.*/\..*' \) -type f)

CC=gcc
LD=ld
CFLAGS=-nostdlib -fno-builtin -m32 -Isrc/include/ -pipe
LDFLAGS=-melf_i386 -Tlink.ld
ASFLAGS=-felf

all: clean cog $(COBJECTS) $(SOBJECTS) link update

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
	
cog:
	@python tools/cog.py -r $(CSOURCES) $(SSOURCES) $(CHEADERS)

initrd_contents = $(shell find ./initrd_contents/ -type f)
map = $(foreach file,$(initrd_contents),./initrd_contents/$(1) $(1))
build_initrd: tools
	@./tools/make_initrd $(map) > /dev/null
	
clean:
	@echo Removing object files
	@for file in $(COBJECTS) $(SOBJECTS) kernel initrd.img emilos.tgz; do if [ -f $$file ]; then rm $$file; fi; done
	@make -C tools/ clean

link:
	@echo " LD	*.o"
	@$(LD) $(LDFLAGS) -o kernel $(SOBJECTS) $(COBJECTS)

.s.o:
	@echo " NASM	$<"
	@nasm $(ASFLAGS) $<

.c.o:
	@echo " CC	$<"
	@$(CC) $(CFLAGS) -o $@ -c $<

tools:
	@echo Building tools
	@make -C tools/ > /dev/null

run: clean all
	@echo Starting qemu
	@qemu -m 256 -fda floppy.img&

srcdist:
	@tar czf emilos.tgz $(ALLFILES)

todos:
	-@for file in $(ALLFILES); do grep -H TODO $$file; done; true

fixmes:
	-@for file in $(ALLFILES); do grep -H FIXME $$file; done; true

find:
	@find include/ src/ -name "*\.[ch]" -type f | xargs grep $$FIND

help:
	@echo "Available make targets:"
	@echo
	@echo "all		- build kernel"
	@echo "run		- run the kernel in qemu"
	@echo "clean		- remove all object files"
	@echo "update		- update floppy image"
	@echo "build_initrd	- build initrd image"
	@echo "tools		- build the tools that are used for various images"
	@echo "srcdist		- build emilos.tgz (source tarball)"
	@echo "todos		- list all TODO comments in the sources"
	@echo "fixmes		- list all FIXME comments in the sources"
	@echo "find		- find a phrase in the sources (Usage: FIND=\"phrase\" make find)"
	@echo "help		- print this list"
	@echo
	@echo "Any additional compiler flags you want to use can be passed as USERFLAGS"
	@echo "(Usage: USERFLAGS=\"flags\" make [...])."

.PHONY: all kernel run update build_initrd tools clean srcdist todos fixmes find help
