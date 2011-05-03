// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
//           Made for JamesM's tutorials <www.jamesmolloy.co.uk>

#include "types.h"
#include "multiboot.h"
#include "x86/monitor.h"
#include "x86/gdt.h"
#include "x86/idt.h"
#include "x86/timer.h"
#include "x86/fpu.h"
#include "mm/vmm.h"
#include "mm/pmm.h"
#include "mm/heap.h"
#include "elf.h"
#include "printk.h"
#include "panic.h"
#include "keyb.h"

elf_t kernel_elf;

int kmain(multiboot_t *mboot_ptr)
{
  monitor_clear();
  
  printk("8888888888               d8b 888  .d88888b.   .d8888b.\n");
  printk("888                      Y8P 888 d88P\" \"Y88b d88P  Y88b\n");
  printk("888                          888 888     888 Y88b.\n");
  printk("8888888    88888b.d88b.  888 888 888     888  \"Y888b.\n");
  printk("888        888 \"888 \"88b 888 888 888     888     \"Y88b.\n");
  printk("888        888  888  888 888 888 888     888       \"888\n");
  printk("888        888  888  888 888 888 Y88b. .d88P Y88b  d88P\n");
  printk("8888888888 888  888  888 888 888  \"Y88888P\"   \"Y8888P\"\n");
  
  init_gdt ();
  init_idt ();
  init_keyboard();
  setup_x87_fpu ();
  init_timer (20);
  init_pmm (mboot_ptr->mem_upper);
  init_vmm ();
  init_heap ();

  // Find all the usable areas of memory and inform the physical memory manager about them.
  uint32_t i = mboot_ptr->mmap_addr;
  while (i < mboot_ptr->mmap_addr + mboot_ptr->mmap_length)
  {
    mmap_entry_t *me = (mmap_entry_t*) i;

    // Does this entry specify usable RAM?
    if (me->type == 1)
    {
      uint32_t j;
      // For every page in this entry, add to the free page stack.
      for (j = me->base_addr_low; j < me->base_addr_low+me->length_low; j += 0x1000)
      {
        pmm_free_page (j);
      }
    }

    // The multiboot specification is strange in this respect - the size member does not include "size" itself in its calculations,
    // so we must add sizeof (uint32_t).
    i += me->size + sizeof (uint32_t);
  }

  kernel_elf = elf_from_multiboot (mboot_ptr);

  asm volatile ("sti");

  panic ("Testing panic mechanism");
  for (;;);

  return 0xdeadbeef;
}
