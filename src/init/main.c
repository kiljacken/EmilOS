// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
//           Made for JamesM's tutorials <www.jamesmolloy.co.uk>
#define DEBUG
#include "io/monitor.h"
#include "system/descriptor_tables.h"
#include "system/timer.h"
#include "mm/paging.h"
#include "mm/kheap.h"
#include "multiboot.h"
#include "fs/fs.h"
#include "fs/initrd.h"
#include "system/task.h"
#include "system/syscall.h"
#include "io/printf.h"
#include "system/fpu.h"
#include "string.h"

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
        { "Enable FPU", setup_x87_fpu },
        { "Enable Paging", initialise_paging },
        { "Enable Tasking", initialise_tasking },
        { "Enable Timer", init_timer },
        { "Enable Syscalls", initialise_syscalls },
        { "Enable printf", init_printf },
        { NULL, NULL, }
};

typedef struct {
	char* name;
	char* value;
}  env_var;

env_var environ[256];

char* get_var(char* name) {
	int i=0;
	while (environ[i].name != NULL)
	{
		if(strcmp(environ[i].name, name)==0)
		{
			return environ[i].value;
		}
		i++;
	}
	return NULL;
}

void set_var(char* name, char* value) {
	int i=0;
	while (environ[i].name != NULL) // Scroll to the end of environment variable list
	{
		i++;
	}
	environ[i].name = name;
	environ[i].value = value;
	environ[i+1].name = NULL;
	environ[i+1].value = NULL;
	
	return 0;
}

int main(struct multiboot *mboot_ptr, u32int initial_stack)
{
    initial_esp = initial_stack;
    // Initialise the screen (by clearing it)
    monitor_clear();
    
    monitor_write("  _            _   __ \n");
    monitor_write(" |_ ._ _  o | / \\ (_  \n");
    monitor_write(" |_ | | | | | \\_/ __) \n");
    monitor_write("                      \n");
    monitor_write("Version 0.2a\n\n");
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
    
    #if 0
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
	printf("Testing floating point math...\n");
	float num;
	num=0.2;
	ASSERT(num/num==1);
	#endif
	
	#if 1
	set_var("SHELL", "bash");
	printf("SHELL = %s\n", get_var("SHELL"));
	#endif
	
	#if 0
	printf("Look mom! I can divide by zer.. OH SHIT!!\n");
	printf("%d\n" , 1/0);
	#endif
	
	#if 1
	switch_to_user_mode();
	syscall_monitor_write("Hello, user world!\n");
	printf("Hello, user world from %s!!\n", "printf");
	#endif

    return 0;
}
