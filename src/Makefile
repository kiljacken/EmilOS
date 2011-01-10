# Makefile for JamesM's kernel tutorials.
# The C and C++ rules are already setup by default.
# The only one that needs changing is the assembler 
# rule, as we use nasm instead of GNU as.

SOURCES=init/boot.o init/main.o io/monitor.o system/common.o system/descriptor_tables.o \
	system/isr.o system/interrupt.o system/gdt.o system/timer.o mm/kheap.o mm/paging.o \
	ordered_array.o fs/fs.o fs/initrd.o system/task.o system/process.o system/syscall.o \
	io/printf.o elf.o system/setjmp.o system/longjmp.o

CFLAGS=-O3 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -Iincludes/
LDFLAGS=-Tlink.ld
ASFLAGS=-felf -iincludes/

all: clean $(SOURCES) link

clean:
	-rm kernel
	-find . -name "*.o" -delete

link:
	ld $(LDFLAGS) -o kernel $(SOURCES)

.s.o:
	nasm $(ASFLAGS) $<

test_program:
	make -C test_program/ clean
	make -C test_program/
	cp test_program/hello ../initrd/
	
run: all update_floppy_image
	qemu-kvm -k en-us -fda ../floppy.img&
	
debug: all update_floppy_image
	qemu-kvm -k en-us -S -s -fda ../floppy.img&
	

silly_syntax=../initrd/$(1) $(1)
initrd_contents = $(shell cd ../initrd; find -type f)
map = $(foreach file,$(initrd_contents),$(call silly_syntax,$(file)))
build_initrd: test_program
	../make_initrd $(map)
	
update_floppy_image: build_initrd
	sudo mount ../floppy.img ../mnt/ -o loop,rw
	sudo cp kernel ../mnt/kernel
	sudo cp initrd.img ../mnt/initrd
	sleep 1
	sudo umount ../mnt/

.PHONY : clean run debug link build_initrd update_floppy_image test_program
