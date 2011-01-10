%ifndef __TL_ASM_INC
%define	__TL_ASM_INC 1

; macros to handle leading underscores added by DOS/Windows compilers
%macro	IMP 1
%ifdef UNDERBARS
	EXTERN _%1
	%define %1 _%1
%else
	EXTERN %1
%endif
%endmacro

%macro	EXP	1
	GLOBAL $_%1
	$_%1:
	GLOBAL $%1
	$%1:
%endmacro

DS_MAGIC		equ	3544DA2Ah

MULTIBOOT_PAGE_ALIGN	equ 1<<0
MULTIBOOT_MEMORY_INFO	equ 1<<1
MULTIBOOT_AOUT_KLUDGE	equ 1<<16
MULTIBOOT_HEADER_MAGIC	equ 0x1BADB002
MULTIBOOT_HEADER_FLAGS	equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_AOUT_KLUDGE
MULTIBOOT_CHECKSUM	equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

%endif
