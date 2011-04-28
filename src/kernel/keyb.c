/* Basic, non-modularized access to the keyboard.
#include <kernel.h>
#include <memory.h>
#include <task.h>
*/

#include "keyb.h"
#include "types.h"
#include "x86/common.h"
#include "printk.h"
#include "x86/monitor.h"
#include "x86/idt.h"

#define kprintf printk

int is_shift=0, is_ctrl=0, is_alt=0, is_caps_lock=0;
void debugger_main();
static const unsigned char kbdus[] =
{
	/* 00 */0,	27,	'1',	'2',	'3',	'4',	'5',	'6',
	/* 08 */'7',	'8',	'9',	'0',	'-',	'=',	'\b',	'\t',
	/* 10 */'q',	'w',	'e',	'r',	't',	'y',	'u',	'i',
	/* 1Dh is left Ctrl */
	/* 18 */'o',	'p',	'[',	']',	'\n',	0,	'a',	's',
	/* 20 */'d',	'f',	'g',	'h',	'j',	'k',	'l',	';',
	/* 2Ah is left Shift */
	/* 28 */'\'',	'`',	0,	'\\',	'z',	'x',	'c',	'v',
	/* 36h is right Shift */
	/* 30 */'b',	'n',	'm',	',',	'.',	'/',	0,	0,
	/* 38h is left Alt, 3Ah is Caps Lock */
	/* 38 */0,	' ',	0,	KEY_F1,	KEY_F2,	KEY_F3,	KEY_F4,	KEY_F5,
	/* 45h is Num Lock, 46h is Scroll Lock */
	/* 40 */KEY_F6,	KEY_F7,	KEY_F8,	KEY_F9,	KEY_F10,0,	0,	KEY_HOME,
	/* 48 */KEY_UP,	KEY_PGUP,'-',	KEY_LFT,'5',	KEY_RT,	'+',	KEY_END,
	/* 50 */KEY_DN,	KEY_PGDN,KEY_INS,KEY_DEL,0,	0,	0,	KEY_F11,
	/* 58 */KEY_F12
};
static const unsigned char kbduss[] =
{
	/* 00? */0,	27,	'!',	'@',	'#',	'$',	'%',	'^',
	/* 08 */'&',	'*',	'(',	')',	'_',	'+',	'\b',	'\t',
	/* 10 */'Q',	'W',	'E',	'R',	'T',	'Y',	'U',	'I',
	/* 1Dh is left Ctrl */
	/* 18 */'O',	'P',	'{',	'}',	'\n',	0,	'A',	'S',
	/* 20 */'D',	'F',	'G',	'H',	'J',	'K',	'L',	':',
	/* 2Ah is left Shift */
	/* 28 */'"',	'~',	0,	'|',	'Z',	'X',	'C',	'V',
	/* 36h is right Shift */
	/* 30 */'B',	'N',	'M',	'<',	'>',	'?',	0,	0,
	/* 38h is left Alt, 3Ah is Caps Lock */
	/* 38 */0,	' ',	0,	KEY_F1,	KEY_F2,	KEY_F3,	KEY_F4,	KEY_F5,
	/* 45h is Num Lock, 46h is Scroll Lock */
	/* 40 */KEY_F6,	KEY_F7,	KEY_F8,	KEY_F9,	KEY_F10,0,	0,	KEY_HOME,
	/* 48 */KEY_UP,	KEY_PGUP,'-',	KEY_LFT,'5',	KEY_RT,	'+',	KEY_END,
	/* 50 */KEY_DN,	KEY_PGDN,KEY_INS,KEY_DEL,0,	0,	0,	KEY_F11,
	/* 58 */KEY_F12
};
static const unsigned char kbduscap[] =
{
	/* 00 */0,	27,	'1',	'2',	'3',	'4',	'5',	'6',
	/* 08 */'7',	'8',	'9',	'0',	'-',	'=',	'\b',	'\t',
	/* 10 */'Q',	'W',	'E',	'R',	'T',	'Y',	'U',	'I',
	/* 1Dh is left Ctrl */
	/* 18 */'O',	'P',	'[',	']',	'\n',	0,	'A',	'S',
	/* 20 */'D',	'F',	'G',	'H',	'J',	'K',	'L',	';',
	/* 2Ah is left Shift */
	/* 28 */'\'',	'`',	0,	'\\',	'Z',	'X',	'C',	'V',
	/* 36h is right Shift */
	/* 30 */'B',	'N',	'M',	',',	'.',	'/',	0,	0,
	/* 38h is left Alt, 3Ah is Caps Lock */
	/* 38 */0,	' ',	0,	KEY_F1,	KEY_F2,	KEY_F3,	KEY_F4,	KEY_F5,
	/* 45h is Num Lock, 46h is Scroll Lock */
	/* 40 */KEY_F6,	KEY_F7,	KEY_F8,	KEY_F9,	KEY_F10,0,	0,	KEY_HOME,
	/* 48 */KEY_UP,	KEY_PGUP,'-',	KEY_LFT,'5',	KEY_RT,	'+',	KEY_END,
	/* 50 */KEY_DN,	KEY_PGDN,KEY_INS,KEY_DEL,0,	0,	0,	KEY_F11,
	/* 58 */KEY_F12
};

static void write_kbd(unsigned adr, unsigned data)
{
	unsigned long timeout;
	unsigned stat;
	
	/* Linux code didn't have a timeout here... */
	for(timeout = 500000L; timeout != 0; timeout--)
	{
		stat = inb(0x64);
		/* loop until 8042 input buffer empty */
		if((stat & 0x02) == 0)
			break;
	}
	if(timeout != 0)
		outb(adr, data);
}

unsigned process(unsigned key)
{
	static unsigned short kbd_status, saw_break_code;
	unsigned char temp;
	/* check for break key (i.e. a key is released) */
	if(key >= 0x80)
	{
		saw_break_code = 1;
		key &= 0x7F;
	}
	/* the only break codes we're interested in are Shift, Ctrl, Alt */
	if(saw_break_code)
	{
		if(key == RAW1_LEFT_ALT || key == RAW1_RIGHT_ALT)
			kbd_status &= ~KBD_META_ALT;
		else if(key == RAW1_LEFT_CTRL || key == RAW1_RIGHT_CTRL)
			kbd_status &= ~KBD_META_CTRL;
		else if(key == RAW1_LEFT_SHIFT || key == RAW1_RIGHT_SHIFT)
			kbd_status &= ~KBD_META_SHIFT;
		saw_break_code = 0;
		return 0;
	}
	/* it's a make key: check the "meta" keys, as above */
	if(key == RAW1_LEFT_ALT || key == RAW1_RIGHT_ALT)
	{
		kbd_status |= KBD_META_ALT;
		return 0;
	}
	if(key == RAW1_LEFT_CTRL || key == RAW1_RIGHT_CTRL)
	{
		kbd_status |= KBD_META_CTRL;
		return 0;
	}
	if(key == RAW1_LEFT_SHIFT || key == RAW1_RIGHT_SHIFT)
	{
		kbd_status |= KBD_META_SHIFT;
		return 0;
	}
	/* Scroll Lock, Num Lock, and Caps Lock set the LEDs. These keys
	have on-off (toggle or XOR) action, instead of momentary action */
	if(key == RAW1_SCROLL_LOCK)
	{
		kbd_status ^= KBD_META_SCRL;
		goto LEDS;
	}
	if(key == RAW1_NUM_LOCK)
	{
		kbd_status ^= KBD_META_NUM;
		goto LEDS;
	}
	if(key == RAW1_CAPS_LOCK)
	{
		kbd_status ^= KBD_META_CAPS;
		LEDS:
		write_kbd(0x60, 0xED);	/* "set LEDs" command */
		temp = 0;
		if(kbd_status & KBD_META_SCRL)
			temp |= 1;
		if(kbd_status & KBD_META_NUM)
			temp |= 2;
		if(kbd_status & KBD_META_CAPS)
			temp |= 4;
		write_kbd(0x60, temp);	/* bottom 3 bits set LEDs */
		return 0;
	}
	/* ignore invalid scan codes */
	//if(key >= sizeof(set1_map) / sizeof(set1_map[0]))
	//	return 0;
	/* convert raw scancode in key to unshifted ASCII in temp */
	temp = kbdus[key];
	/* defective keyboard? non-US keyboard? more than 104 keys? */
	if(temp == 0)
		return temp;
	/* handle the three-finger salute */
	if((kbd_status & KBD_META_CTRL) && (kbd_status & KBD_META_ALT) && (temp == KEY_DEL))
	{
		kprintf("C^A^D\n");
		/* Possible handling techniques:
		1) Reboot
		2) Start Icmd on current terminal
		3) ...?
		*/
	}
	if(kbd_status & KBD_META_SHIFT)
		is_shift=1;
	else
		is_shift=0;
	if(kbd_status & KBD_META_CTRL)
		is_ctrl=1;
	else
		is_ctrl=0;
	if(kbd_status & KBD_META_ALT)
		is_alt=1;
	else
		is_alt=0;
	
	if(kbd_status & KBD_META_CAPS)
		is_caps_lock=1;
	else
		is_caps_lock=0;
	
	return temp;
}

void kb_int_handler()
{
	unsigned char scancode;
	/* Read from the keyboard's data buffer */
	scancode = inb(0x60);
	unsigned temp;
	do
	{
		temp = inb(0x64);
		if((temp & 0x01) != 0)
		{
			(void)inb(0x60);
			continue;
		}
	} while((temp & 0x02) != 0);
	
	/* Convert actually processes break codes, and ctrl and shift (and later alt) and LEDs */
	char ym = process(scancode);
	
	//s_enable_int=1;
	if (scancode & 0x80)
	{
		is_shift=0;
		is_ctrl=0;
		is_alt=0;
		
	}
	else
	{
		/* Handle multiple console switching */
		if(is_ctrl)
		{
			//if(ym >= '0' && ym <= '9')
			//	switch_console(ym-48);
			//task_uncritical();
			//if (ym == 'c')
			//{
			//	if(consoles[curcons].death) {
			//		consoles[curcons].reading=0;
			//		do_send_signal(consoles[curcons].death, SIGKILL, 1);
			//	}
			//}
			//super_sti();
			return;
		}
		/* Normal characters */
		unsigned char ct;
		if(!is_shift) {
			ct = kbdus[scancode];
		}
		else {
			ct = kbduss[scancode];
		}
		if(is_caps_lock)
		{
			if((kbduss[scancode] != kbduscap[scancode]) && is_shift)
				ct = kbduss[scancode];
			else if(!is_shift)
				ct = kbduscap[scancode];
			else
				ct = kbdus[scancode];
			
		}
		//if(ym && consoles[curcons].inpos < 255) {
		//	/* Add to stdin for the current console */
		//	
		//	consoles[curcons].input[consoles[curcons].inpos] = ct;
		//	consoles[curcons].inpos++;
		//}
		
		//if(ym && ct != '\b' && consoles[curcons].gtty.sg_flags & ECHO)
		//	consoles[curcons].putch(curcons, ct);
		monitor_put(ct);
	}
	//task_uncritical();
	//s_enable_int=1;
	//enable_int=1;
}

void init_keyboard() {
	printk("Keyboard installed to %d\n", IRQ1);
	register_interrupt_handler(IRQ1, &kb_int_handler);
	unsigned temp;
	do
	{
		temp = inb(0x64);
		if((temp & 0x01) != 0)
		{
			(void)inb(0x60);
			continue;
		}
	} while((temp & 0x02) != 0);
	//__super_sti();
}
