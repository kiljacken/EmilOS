;
; boot.s -- Kernel start location. Also defines multiboot header.
;           Based on Bran's kernel development tutorial file start.asm
;

MBOOT_PAGE_ALIGN    equ 1<<0    ; Load kernel and modules on a page boundary
MBOOT_MEM_INFO      equ 1<<1    ; Provide your kernel with memory info
MBOOT_HEADER_MAGIC  equ 0x1BADB002 ; Multiboot Magic value
; NOTE: We do not use MBOOT_AOUT_KLUDGE. It means that GRUB does not
; pass us a symbol table.
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

bits 32                         ; All instructions should be 32-bit.

global mboot                    ; Make 'mboot' accessible from C.

mboot:
    dd  MBOOT_HEADER_MAGIC      ; GRUB will search for this value on each
                                ; 4-byte boundary in your kernel file
    dd  MBOOT_HEADER_FLAGS      ; How GRUB should load your file / settings
    dd  MBOOT_CHECKSUM          ; To ensure that the above values are correct
    
global start:function end-start ; Kernel entry point.
extern main                     ; This is the entry point of our C code
extern start_ctors, end_ctors	; This is the place where the C++ 
								; constructors reside
extern start_dtors, end_dtors	; This is the place where the C++ 
								; deconstructors reside
								
start:
    push ebx                  	; Push a pointer to the multiboot info structure.
    
    mov ebx, start_ctors		; Fetch first C++ constructor
    jmp .test
    
.body:
	call [ebx]					; Call the constructor
	add ebx,4					; Go to the next one
 
.test:
	cmp ebx, end_ctors			; Have we reached the end?
	jb .body					; If not, continue to execute it

    mov ebp, 0                  ; Initialise the base pointer to zero so we can terminate stack traces
                                ; here.
                                
    cli                         ; Disable interrupts.
    call main                   ; call our main() function.
    
static_dtors_loop:
	mov ebx, start_dtors		; Fetch first C++ deconstructor
	jmp .test
 
.body:
	call [ebx]					; Call the deconstructor
	add ebx,4					; Go to the next one
 
.test:
	cmp ebx, end_dtors			; Have we reached the end?
	jb .body					; If not, continue to execute it
   
    hlt							; Halt machine should kernel return
    jmp $                       ; Incase machine doesn't halt
								; enter an infinite loop, to stop the processor
end:                           ; executing whatever rubbish is in the memory
                                ; after our kernel!
