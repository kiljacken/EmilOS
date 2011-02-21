/* Link.ld -- Linker script for the kernel - ensure everything goes in the */
/*            Correct place.  */
/*            Original file taken from Bran's Kernel Development */
/*            tutorials: http://www.osdever.net/bkerndev/index.php. */

ENTRY(start)
SECTIONS
{

    .text 0x100000 :
    {
        code = .; _code = .; __code = .;
        *(.text)
        *(.gnu.linkonce.t*)
        . = ALIGN(4096);
    }
    
    .rodata :
    {
		start_ctors = .;
		*(.ctor*)
		end_ctors = .;

		start_dtors = .;
		*(.dtor*)
		end_dtors = .;

		*(.rodata*)
		*(.gnu.linkonce.r*)
		. = ALIGN(4096);
    }

    .data :
    {
        data = .; _data = .; __data = .;
        *(.data)
        *(.gnu.linkonce.d*)
        . = ALIGN(4096);
    }

    .bss :
    {
        bss = .; _bss = .; __bss = .;
        *(.bss)
        *(.gnu.linkonce.b*)
        . = ALIGN(4096);
    }
    
    /DISCARD/ :
    {
		*(.comment)
		*(.eh_frame)
    }

    end = .; _end = .; __end = .;
}
