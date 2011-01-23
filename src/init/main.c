// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
//           Made for JamesM's tutorials <www.jamesmolloy.co.uk>
#define DEBUG
#include "io/monitor.h"
#include "system/descriptor_tables.h"
#include "system/timer.h"
#include "mm/paging.h"
#include "multiboot.h"
#include "fs/fs.h"
#include "fs/initrd.h"
#include "system/task.h"
#include "system/syscall.h"
#include "io/printf.h"

extern u32int read_eip();
extern u32int placement_address;
u32int initial_esp;

typedef void (*init_function)(void);

struct splash{
        char* action;
        init_function func;
};

void splasher(struct splash* actions) {
	int i = 0;
	while (actions[i].func!=NULL)
	{
		monitor_write(actions[i].action);
		monitor_write("... ");
		actions[i].func();
		monitor_write("Done!\n");
		i++;
	}
	monitor_write("\n");
}

struct splash actions[] = {
        { "Load Descriptor Tables", init_descriptor_tables },
        { "Enable Paging", initialise_paging },
        { "Enable Tasking", initialise_tasking },
        { "Enable Timer", init_timer },
        { "Enable Syscalls", initialise_syscalls },
        { "Enable printf", init_printf },
        { NULL, NULL, }
};

int main(struct multiboot *mboot_ptr, u32int initial_stack)
{
    initial_esp = initial_stack;
    // Initialise the screen (by clearing it)
    monitor_clear();
    
    monitor_write("  _            _   __ \n");
    monitor_write(" |_ ._ _  o | / \\ (_  \n");
    monitor_write(" |_ | | | | | \\_/ __) \n");
    monitor_write("                      \n");
    monitor_write("Version 1.0\n\n");
    monitor_write("Load initrd... ");

    // Find the location of our initial ramdisk.
    ASSERT(mboot_ptr->mods_count > 0);
    u32int initrd_location = *((u32int*)mboot_ptr->mods_addr);
    u32int initrd_end = *(u32int*)(mboot_ptr->mods_addr+4);
    // Don't trample our module with placement accesses, please!
    placement_address = initrd_end;
    
    // Initialise the initial ramdisk, and set it as the filesystem root.
    fs_root = initialise_initrd(initrd_location);
    monitor_write("Done!\n");
    
    asm volatile("sti");
    splasher(actions);
    
    #if 1
    // list the contents of /
	int i = 0;
	struct dirent *node = 0;
	while ( (node = readdir_fs(fs_root, i)) != 0)
	{
		monitor_write("Found file ");
		monitor_write(node->name);
		fs_node_t *fsnode = finddir_fs(fs_root, node->name);

		if ((fsnode->flags&0x7) == FS_DIRECTORY)
			monitor_write("\n\t(directory)\n");
		else
		{
			monitor_write("\n\t contents: \"");
			char buf[256];
			u32int sz = read_fs(fsnode, 0, 256, buf);
			int j;
			for (j = 0; j < sz; j++)
				monitor_put(buf[j]);

			monitor_write("\"\n");
		}
		i++;
	}
	#endif
    
    #if 1
	printf("Hello, from %s!!\n", "printf");
	#endif
	
	#if 1
	switch_to_user_mode();
	syscall_monitor_write("Hello, user world!\n");
	printf("Hello, user world from %s!!\n", "printf");
	#endif

    return 0;
}
